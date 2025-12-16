#include <iostream>
#include <string>
#include <vector>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <windows.h> // WinAPI
#include <direct.h>  // _getcwd

#include "common_boost.h"

namespace bip = boost::interprocess;

bip::mapped_region* pRegion = nullptr;
bip::shared_memory_object* pShm = nullptr;
bip::named_mutex* pMutex = nullptr;
bip::named_semaphore* pSemEmpty = nullptr;
bip::named_semaphore* pSemFull = nullptr;

// Храним информацию о запущенных процессах (WinAPI)
std::vector<PROCESS_INFORMATION> vSenderProcesses;

void Cleanup() {
    std::cout << "\nCleanup initiated..." << std::endl;

    if (pRegion) delete pRegion;
    if (pShm) delete pShm;
    if (pMutex) delete pMutex;
    if (pSemEmpty) delete pSemEmpty;
    if (pSemFull) delete pSemFull;

    // Завершаем процессы WinAPI
    for (auto& pi : vSenderProcesses) {
        // Проверяем, жив ли процесс, и если да - убиваем
        DWORD exitCode = 0;
        if (GetExitCodeProcess(pi.hProcess, &exitCode) && exitCode == STILL_ACTIVE) {
            TerminateProcess(pi.hProcess, 0);
        }
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    vSenderProcesses.clear();

    try {
        bip::shared_memory_object::remove(COMMON_MMF_NAME);
        bip::named_semaphore::remove(COMMON_SEM_FULL_NAME);
        bip::named_semaphore::remove(COMMON_SEM_EMPTY_NAME);
        bip::named_mutex::remove(COMMON_MUTEX_NAME);
        std::cout << "Global IPC objects removed." << std::endl;
    } catch (const bip::interprocess_exception& e) {
        std::cerr << "Warning during cleanup: " << e.what() << std::endl;
    }
}

int main() {
    // Предварительная очистка на случай некорректного завершения прошлого раза
    try {
        bip::shared_memory_object::remove(COMMON_MMF_NAME);
        bip::named_semaphore::remove(COMMON_SEM_FULL_NAME);
        bip::named_semaphore::remove(COMMON_SEM_EMPTY_NAME);
        bip::named_mutex::remove(COMMON_MUTEX_NAME);
    } catch (...) {}

    std::string fileName_dummy; // Имя файла нам не нужно для shared_memory_object (оно в ОЗУ), но оставим ввод для совместимости логики
    size_t max_records;
    int sender_count;

    std::cout << "--- RECEIVER (Boost IPC + WinAPI Process) ---" << std::endl;
    std::cout << "Enter logic name (just press enter or type anything): ";
    std::cin >> fileName_dummy;
    std::cout << "Enter max records (FIFO size): ";
    std::cin >> max_records;
    std::cout << "Enter sender count: ";
    std::cin >> sender_count;

    const size_t MMF_SIZE = sizeof(SharedHeader) + max_records * sizeof(Message);

    try {
        // 1. Создаем Shared Memory
        pShm = new bip::shared_memory_object(bip::create_only, COMMON_MMF_NAME, bip::read_write);
        pShm->truncate(MMF_SIZE);
        pRegion = new bip::mapped_region(*pShm, bip::read_write);

        // 2. Создаем именованные мьютекс и семафоры
        pMutex = new bip::named_mutex(bip::create_only, COMMON_MUTEX_NAME);
        pSemEmpty = new bip::named_semaphore(bip::create_only, COMMON_SEM_EMPTY_NAME, max_records);
        pSemFull = new bip::named_semaphore(bip::create_only, COMMON_SEM_FULL_NAME, 0);

        // 3. Инициализация заголовка
        SharedHeader* pHeader = static_cast<SharedHeader*>(pRegion->get_address());
        pHeader->max_records = max_records;
        pHeader->write_pos = 0;
        pHeader->read_pos = 0;

    } catch (const bip::interprocess_exception& e) {
        std::cerr << "FATAL ERROR during IPC object creation: " << e.what() << std::endl;
        Cleanup();
        return 1;
    }

    SharedHeader* pHeader = static_cast<SharedHeader*>(pRegion->get_address());
    Message* pMessages = reinterpret_cast<Message*>(pHeader + 1);

    // --- ЗАПУСК ПРОЦЕССОВ ЧЕРЕЗ WINAPI ---
    char currentDirBuffer[MAX_PATH];
    _getcwd(currentDirBuffer, MAX_PATH);
    std::string currentDir = currentDirBuffer;
    std::string senderExePath = currentDir + "\\sender_boost.exe";

    std::cout << "Launching " << sender_count << " Sender processes..." << std::endl;

    for (int i = 0; i < sender_count; ++i) {
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        // Формируем командную строку: "sender_boost.exe <ID>"
        // Важно: CreateProcess может менять строку аргументов, поэтому используем буфер
        std::string cmdLineStr = "\"" + senderExePath + "\" " + std::to_string(i);
        std::vector<char> cmdLine(cmdLineStr.begin(), cmdLineStr.end());
        cmdLine.push_back('\0');

        BOOL success = CreateProcessA(
            NULL,               // Application Name (null -> берем из cmdLine)
            cmdLine.data(),     // Command Line
            NULL, NULL,         // Security Attributes
            FALSE,              // Inherit Handles
            CREATE_NEW_CONSOLE, // Создать новое окно консоли для Senders!
            NULL,               // Environment
            NULL,               // Current Directory
            &si,
            &pi
        );

        if (!success) {
            std::cerr << "Failed to launch Sender " << i << ". Error: " << GetLastError() << std::endl;
        } else {
            vSenderProcesses.push_back(pi);
        }
    }

    std::cout << "Start interaction loop." << std::endl;

    std::string command;
    while (true) {
        std::cout << "\nEnter command ('read' or 'exit'): ";
        std::cin >> command;

        if (command == "exit") {
            break;
        }

        if (command == "read") {
            std::cout << "Waiting for message... (Waiting on SemFull)" << std::endl;

            // Ждем сообщения (если семафор > 0, уменьшаем его)
            pSemFull->wait();

            // Блокируем доступ к памяти
            bip::scoped_lock<bip::named_mutex> lock(*pMutex);

            Message msg = pMessages[pHeader->read_pos];
            pHeader->read_pos = (pHeader->read_pos + 1) % pHeader->max_records;

            std::cout << "Received: " << msg.data << std::endl;

            // lock.unlock() - автоматически при выходе из скоупа
            // Увеличиваем счетчик пустых мест
            pSemEmpty->post();
        }
    }

    Cleanup();
    return 0;
}
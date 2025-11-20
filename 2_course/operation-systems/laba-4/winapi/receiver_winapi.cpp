#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <sstream>
#include <iomanip>
#include "common_winapi.h"

// --- Глобальные переменные для очистки ---
HANDLE hMapFile = NULL;
LPVOID pSharedMem = NULL;
HANDLE hMutex = NULL;
HANDLE hSemEmpty = NULL;
HANDLE hSemFull = NULL;
std::vector<HANDLE> hEvents;
std::vector<HANDLE> hProcessHandles;
HANDLE hShutdownEvent = NULL;

void Cleanup() {
    if (pSharedMem) UnmapViewOfFile(pSharedMem);
    if (hMapFile) CloseHandle(hMapFile);
    if (hMutex) CloseHandle(hMutex);
    if (hSemEmpty) CloseHandle(hSemEmpty);
    if (hSemFull) CloseHandle(hSemFull);
    for (HANDLE h : hEvents) CloseHandle(h);
    for (HANDLE h : hProcessHandles) CloseHandle(h);
    if (hShutdownEvent) CloseHandle(hShutdownEvent);
}

// -------------------------------------------------------------------
// ТЕСТОВЫЙ РЕЖИМ RECEIVER (для CTest)
// receiver_winapi.exe <file> <max_records> <sender_count> <msgs_to_read>
// -------------------------------------------------------------------
int RunTestMode(const std::string& fileName, size_t max_records, int sender_count, int messages_to_read) {
    std::cout << "--- RECEIVER (Test Mode) ---" << std::endl;
    std::cout << "FIFO Size: " << max_records << ", Senders: " << sender_count << ", Reads: " << messages_to_read << std::endl;

    // --- 1. Создаем объекты синхронизации ---
    hMutex = CreateMutex(NULL, FALSE, COMMON_MUTEX_NAME);
    hSemEmpty = CreateSemaphore(NULL, (LONG)max_records, (LONG)max_records, COMMON_SEM_EMPTY_NAME);
    hSemFull = CreateSemaphore(NULL, 0, (LONG)max_records, COMMON_SEM_FULL_NAME);
    hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, COMMON_SHUTDOWN_EVENT_NAME);

    if (!hMutex || !hSemEmpty || !hSemFull || !hShutdownEvent) {
        std::cerr << "FATAL: Failed to create IPC objects: " << GetLastErrorAsString() << std::endl;
        Cleanup();
        return 1;
    }

    // --- 2. Создаем MMF ---
    size_t MMF_SIZE = sizeof(SharedHeader) + max_records * sizeof(Message);
    HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) { Cleanup(); return 1; }
    SetLastError(0);
    hMapFile = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, (DWORD)MMF_SIZE, COMMON_MMF_NAME);
    CloseHandle(hFile);

    if (hMapFile == NULL) {
        std::cerr << "FATAL: Could not create file mapping: " << GetLastErrorAsString() << std::endl;
        Cleanup();
        return 1;
    }

    pSharedMem = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, MMF_SIZE);
    if (pSharedMem == NULL) { Cleanup(); return 1; }

    SharedHeader* pHeader = (SharedHeader*)pSharedMem;
    pHeader->max_records = max_records;
    pHeader->write_pos = 0;
    pHeader->read_pos = 0;
    Message* pMessages = (Message*)(pHeader + 1);

    // --- 3. Запускаем Sender'ов в ТЕСТОВОМ РЕЖИМЕ ---
    char currentDirBuffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, currentDirBuffer);
    std::string senderBaseCommand = std::string(currentDirBuffer) + "\\sender_winapi.exe ";

    for (int i = 0; i < sender_count; ++i) {
        std::string eventName = COMMON_EVENT_PREFIX + std::to_string(i);
        HANDLE hReadyEvent = CreateEvent(NULL, FALSE, FALSE, eventName.c_str());
        hEvents.push_back(hReadyEvent);

        // ФОРМИРУЕМ КОМАНДУ ДЛЯ ТЕСТОВОГО РЕЖИМА SENDER: <ID> <message_text>
        std::string testMsg = "TEST_MSG_" + std::to_string(i + 1);
        std::string cmdLine = senderBaseCommand + std::to_string(i) + " " + testMsg;

        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        ZeroMemory(&pi, sizeof(&pi));
        si.cb = sizeof(si);

        std::vector<char> cmdBuffer(cmdLine.begin(), cmdLine.end());
        cmdBuffer.push_back('\0');

        // Используем 0 вместо CREATE_NEW_CONSOLE для запуска в фоновом режиме в CTest
        if (!CreateProcessA(NULL, cmdBuffer.data(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            std::cerr << "FATAL ERROR: Failed to launch Sender " << i << " (" << GetLastErrorAsString() << ")." << std::endl;
            Cleanup();
            return 1;
        }

        hProcessHandles.push_back(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    // --- 4. Ждем готовности и Читаем сообщения ---
    if (sender_count > 0) {
        std::cout << "Waiting for " << sender_count << " senders to be ready..." << std::endl;
        if (WaitForMultipleObjects(sender_count, hEvents.data(), TRUE, 5000) == WAIT_TIMEOUT) {
            std::cerr << "FATAL: Timeout waiting for Senders to be ready." << std::endl;
            Cleanup();
            return 2;
        }
        std::cout << "All senders are ready!" << std::endl;
    }

    std::cout << "Starting read cycle, waiting for " << messages_to_read << " messages..." << std::endl;
    for (int i = 0; i < messages_to_read; ++i) {
        // Ждем, пока семафор "Полные" > 0 (Блокировка на пустоте)
        DWORD waitResult = WaitForSingleObject(hSemFull, 5000);

        if (waitResult != WAIT_OBJECT_0) {
            std::cerr << "FATAL: Read timeout or error (Message " << i + 1 << " of " << messages_to_read << ")." << std::endl;
            Cleanup();
            return 2;
        }

        WaitForSingleObject(hMutex, INFINITE);
        Message msg = pMessages[pHeader->read_pos];
        pHeader->read_pos = (pHeader->read_pos + 1) % pHeader->max_records;
        ReleaseMutex(hMutex);
        ReleaseSemaphore(hSemEmpty, 1, NULL);

        std::cout << "R: Received: " << msg.data << std::endl;
    }

    // --- 5. Сигнализируем о завершении работы и очистка ---
    if (sender_count > 0) {
        std::cout << "Signaling Shutdown Event..." << std::endl;
        SetEvent(hShutdownEvent);
        // Ждем завершения процессов Sender'ов
        WaitForMultipleObjects((DWORD)sender_count, hProcessHandles.data(), TRUE, 5000);
    }

    std::cout << "Test Mode finished. Cleaning up resources..." << std::endl;
    Cleanup();
    return 0; // Успех!
}


// --- ОСНОВНАЯ ФУНКЦИЯ main() (Интерактивный режим) ---
int main(int argc, char* argv[]) {
    // --- 1. Проверка на тестовый режим (для CTest) ---
    if (argc == 5) {
        try {
            return RunTestMode(argv[1], std::stoul(argv[2]), std::stoi(argv[3]), std::stoi(argv[4]));
        } catch (const std::exception& e) {
            std::cerr << "FATAL: Invalid arguments in Test Mode: " << e.what() << std::endl;
            Cleanup();
            return 1;
        }
    }

    // --- ИНТЕРАКТИВНЫЙ РЕЖИМ ---
    std::string fileName;
    size_t max_records;
    int sender_count;

    // --- 1. Ввод данных ---
    std::cout << "--- RECEIVER (WinAPI FIFO) ---" << std::endl;
    std::cout << "Enter file name (for MMF backing): ";
    std::cin >> fileName;
    std::cout << "Enter max records (FIFO size): ";
    std::cin >> max_records;
    std::cout << "Enter sender count: ";
    std::cin >> sender_count;

    // --- 2. Создаем объекты синхронизации ---
    hMutex = CreateMutex(NULL, FALSE, COMMON_MUTEX_NAME);
    hSemEmpty = CreateSemaphore(NULL, (LONG)max_records, (LONG)max_records, COMMON_SEM_EMPTY_NAME);
    hSemFull = CreateSemaphore(NULL, 0, (LONG)max_records, COMMON_SEM_FULL_NAME);
    hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, COMMON_SHUTDOWN_EVENT_NAME);

    if (!hMutex || !hSemEmpty || !hSemFull || !hShutdownEvent) {
        std::cerr << "FATAL: Failed to create IPC objects: " << GetLastErrorAsString() << std::endl;
        Cleanup();
        return 1;
    }

    // --- 3. Создаем MMF ---
    size_t MMF_SIZE = sizeof(SharedHeader) + max_records * sizeof(Message);
    HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) { Cleanup(); return 1; }
    SetLastError(0);
    hMapFile = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, (DWORD)MMF_SIZE, COMMON_MMF_NAME);
    CloseHandle(hFile);
    if (hMapFile == NULL) {
        std::cerr << "FATAL: Could not create file mapping: " << GetLastErrorAsString() << std::endl;
        Cleanup();
        return 1;
    }

    pSharedMem = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, MMF_SIZE);
    if (pSharedMem == NULL) { Cleanup(); return 1; }

    SharedHeader* pHeader = (SharedHeader*)pSharedMem;
    pHeader->max_records = max_records;
    pHeader->write_pos = 0;
    pHeader->read_pos = 0;
    Message* pMessages = (Message*)(pHeader + 1);

    // --- 4. Запускаем Sender'ов (в интерактивном режиме - 1 аргумент) ---
    char currentDirBuffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, currentDirBuffer);
    std::string senderBaseCommand = std::string(currentDirBuffer) + "\\sender_winapi.exe ";

    for (int i = 0; i < sender_count; ++i) {
        std::string eventName = COMMON_EVENT_PREFIX + std::to_string(i);
        HANDLE hReadyEvent = CreateEvent(NULL, FALSE, FALSE, eventName.c_str());
        hEvents.push_back(hReadyEvent);

        // Передаем только ID, чтобы Sender запустился в интерактивном режиме
        std::string cmdLine = senderBaseCommand + std::to_string(i);

        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        ZeroMemory(&pi, sizeof(&pi));
        si.cb = sizeof(si);

        std::vector<char> cmdBuffer(cmdLine.begin(), cmdLine.end());
        cmdBuffer.push_back('\0');

        if (!CreateProcessA(NULL, cmdBuffer.data(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            std::cerr << "\nFATAL ERROR: Failed to launch Sender " << i << " (" << GetLastErrorAsString() << ")." << std::endl;
            Cleanup();
            return 1;
        }

        hProcessHandles.push_back(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    // --- 5. Ждем сигнал на готовность от всех Sender'ов ---
    std::cout << "Waiting for all " << sender_count << " senders to be ready..." << std::endl;
    WaitForMultipleObjects(sender_count, hEvents.data(), TRUE, INFINITE);
    std::cout << "All senders are ready! Starting interactive cycle." << std::endl;

    // --- 6. ЦИКЛ КОМАНД (Чтение) ---
    std::string command;
    while (true) {
        std::cout << "\nEnter command ('read' or 'exit'): ";
        std::cin >> command;

        if (command == "exit") {
            break;
        }

        if (command == "read") {
            std::cout << "Waiting for message... (Waiting on hSemFull)" << std::endl;
            WaitForSingleObject(hSemFull, INFINITE);
            WaitForSingleObject(hMutex, INFINITE);

            Message msg = pMessages[pHeader->read_pos];
            pHeader->read_pos = (pHeader->read_pos + 1) % pHeader->max_records;

            std::cout << "Received: " << msg.data << std::endl;

            ReleaseMutex(hMutex);
            ReleaseSemaphore(hSemEmpty, 1, NULL);
        }
    }

    // --- 7. Очистка ---
    std::cout << "Signaling Shutdown Event to all senders..." << std::endl;
    SetEvent(hShutdownEvent);
    // Ждем завершения процессов Sender'ов
    WaitForMultipleObjects((DWORD)sender_count, hProcessHandles.data(), TRUE, INFINITE);

    std::cout << "Receiver shutting down. Cleaning up resources..." << std::endl;
    Cleanup();
    return 0;
}
#include <iostream>
#include <string>
#include <windows.h>
#include <sstream>
#include "common_winapi.h"

// --- Глобальные переменные для очистки ---
HANDLE hMapFile = NULL;
LPVOID pSharedMem = NULL;
HANDLE hMutex = NULL;
HANDLE hSemEmpty = NULL;
HANDLE hSemFull = NULL;
HANDLE hReadyEvent = NULL;

void Cleanup() {
    if (pSharedMem) UnmapViewOfFile(pSharedMem);
    if (hMapFile) CloseHandle(hMapFile);
    if (hMutex) CloseHandle(hMutex);
    if (hSemEmpty) CloseHandle(hSemEmpty);
    if (hSemFull) CloseHandle(hSemFull);
    if (hReadyEvent) CloseHandle(hReadyEvent);
}

int main(int argc, char* argv[]) {
    // --- 1. Обработка аргументов ---
    if (argc < 2) {
        std::cerr << "Usage: sender_winapi.exe [senderID]" << std::endl;
        return 1;
    }
    std::string senderIdStr = argv[1];
    int senderId = std::stoi(senderIdStr);

    std::cout << "--- SENDER " << senderId << " (WinAPI FIFO) ---" << std::endl;

    // --- 2. Открытие существующих объектов ---
    hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, COMMON_MUTEX_NAME);
    hSemEmpty = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, COMMON_SEM_EMPTY_NAME);
    hSemFull = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, COMMON_SEM_FULL_NAME);

    std::string eventName = COMMON_EVENT_PREFIX + senderIdStr;
    hReadyEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, eventName.c_str());

    if (!hMutex || !hSemEmpty || !hSemFull || !hReadyEvent) {
        std::cerr << "FATAL: Could not open IPC objects (" << GetLastError() << "): " << GetLastErrorAsString() << std::endl;
        Cleanup();
        return 1;
    }

    // --- 3. Открытие MMF ---
    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, COMMON_MMF_NAME);
    if (hMapFile == NULL) {
        std::cerr << "FATAL: Could not open file mapping (" << GetLastError() << "): " << GetLastErrorAsString() << std::endl;
        Cleanup();
        return 1;
    }

    pSharedMem = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (pSharedMem == NULL) {
        std::cerr << "FATAL: Could not map view of file (" << GetLastError() << "): " << GetLastErrorAsString() << std::endl;
        Cleanup();
        return 1;
    }

    // Получаем указатели на данные
    SharedHeader* pHeader = (SharedHeader*)pSharedMem;
    Message* pMessages = (Message*)(pHeader + 1);
    size_t max_records = pHeader->max_records; // Читаем размер из MMF

    // --- 4. Отправить Receiver'у сигнал о готовности ---
    SetEvent(hReadyEvent);

    // --- 5. Цикл команд (Запись) ---
    std::string command;
    std::cin.ignore();

    while (true) {
        std::cout << "\n(Sender " << senderId << ") Enter command ('write' or 'exit'): ";
        std::getline(std::cin, command);

        if (command == "exit") {
            break;
        }

        if (command == "write") {
            std::string msgText;
            std::cout << "(Sender " << senderId << ") Enter message (max 20 chars): ";
            std::getline(std::cin, msgText);

            Message msg;
            std::string fullMsg = "(S" + senderIdStr + ") " + msgText;
            strncpy(msg.data, fullMsg.c_str(), MESSAGE_LENGTH - 1);
            msg.data[MESSAGE_LENGTH - 1] = '\0';

            std::cout << "Waiting for free slot... (Waiting on hSemEmpty)" << std::endl;

            // 1. Ждем, пока семафор "Пустые" > 0 (Блокировка на полноте)
            WaitForSingleObject(hSemEmpty, INFINITE);

            // 2. Блокируем Mutex
            WaitForSingleObject(hMutex, INFINITE);

            // 3. Пишем и сдвигаем write_pos
            pMessages[pHeader->write_pos] = msg;
            pHeader->write_pos = (pHeader->write_pos + 1) % max_records;

            std::cout << "Message sent." << std::endl;

            // 4. Освобождаем Mutex
            ReleaseMutex(hMutex);

            // 5. Сигналим, что добавился один "Полный" слот
            ReleaseSemaphore(hSemFull, 1, NULL);
        }
    }

    // Очистка
    std::cout << "Sender " << senderId << " shutting down..." << std::endl;
    Cleanup();
    return 0;
}
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
HANDLE hShutdownEvent = NULL;

void Cleanup() {
    if (pSharedMem) UnmapViewOfFile(pSharedMem);
    if (hMapFile) CloseHandle(hMapFile);
    if (hMutex) CloseHandle(hMutex);
    if (hSemEmpty) CloseHandle(hSemEmpty);
    if (hSemFull) CloseHandle(hSemFull);
    if (hReadyEvent) CloseHandle(hReadyEvent);
    if (hShutdownEvent) CloseHandle(hShutdownEvent);
}

// -------------------------------------------------------------------
// ТЕСТОВЫЙ РЕЖИМ SENDER (для CTest)
// sender_winapi.exe <senderID> <message_text>
// -------------------------------------------------------------------
int RunTestMode(int senderId, const std::string& msgText) {
    std::string senderIdStr = std::to_string(senderId);
    std::cout << "--- SENDER " << senderId << " (Test Mode) ---" << std::endl;

    // --- 1. Открытие существующих объектов ---
    hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, COMMON_MUTEX_NAME);
    hSemEmpty = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, COMMON_SEM_EMPTY_NAME);
    hSemFull = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, COMMON_SEM_FULL_NAME);
    hShutdownEvent = OpenEvent(SYNCHRONIZE, FALSE, COMMON_SHUTDOWN_EVENT_NAME);

    std::string eventName = COMMON_EVENT_PREFIX + senderIdStr;
    hReadyEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, eventName.c_str());

    if (!hMutex || !hSemEmpty || !hSemFull || !hReadyEvent || !hShutdownEvent) {
        std::cerr << "FATAL: Could not open IPC objects (" << GetLastError() << "): " << GetLastErrorAsString() << std::endl;
        Cleanup();
        return 1;
    }

    // --- 2. Открытие MMF ---
    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, COMMON_MMF_NAME);
    if (hMapFile == NULL) { Cleanup(); return 1; }
    pSharedMem = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (pSharedMem == NULL) { Cleanup(); return 1; }

    SharedHeader* pHeader = (SharedHeader*)pSharedMem;
    Message* pMessages = (Message*)(pHeader + 1);

    // --- 3. Отправить Receiver'у сигнал о готовности ---
    SetEvent(hReadyEvent);
    std::cout << "Ready signal sent." << std::endl;

    // --- 4. Запись сообщения (одна итерация) ---
    Message msg;
    std::string fullMsg = "(S" + senderIdStr + ") " + msgText;
    strncpy(msg.data, fullMsg.c_str(), MESSAGE_LENGTH - 1);
    msg.data[MESSAGE_LENGTH - 1] = '\0';

    // Ждем либо свободного слота (hSemEmpty), либо сигнала выключения (hShutdownEvent)
    HANDLE waitHandles[2] = {hSemEmpty, hShutdownEvent};
    DWORD dwWaitResult = WaitForMultipleObjects(2, waitHandles, FALSE, INFINITE);

    if (dwWaitResult == WAIT_OBJECT_0 + 1) { // Индекс 1: hShutdownEvent
        std::cout << "Shutdown signal received while trying to write. Exiting." << std::endl;
        Cleanup();
        return 0;
    } else if (dwWaitResult != WAIT_OBJECT_0) {
        std::cerr << "FATAL: WaitForMultipleObjects failed: " << GetLastErrorAsString() << std::endl;
        Cleanup();
        return 1;
    }
    // Если WAIT_OBJECT_0, мы захватили hSemEmpty

    WaitForSingleObject(hMutex, INFINITE);
    pMessages[pHeader->write_pos] = msg;
    pHeader->write_pos = (pHeader->write_pos + 1) % pHeader->max_records;
    ReleaseMutex(hMutex);
    ReleaseSemaphore(hSemFull, 1, NULL);

    std::cout << "Message '" << fullMsg << "' sent. Exiting." << std::endl;

    // Очистка
    Cleanup();
    return 0; // Успех!
}


// --- ОСНОВНАЯ ФУНКЦИЯ main() (Интерактивный режим) ---
int main(int argc, char* argv[]) {
    // --- 1. Проверка на тестовый режим ---
    if (argc == 3) {
        try {
            return RunTestMode(std::stoi(argv[1]), argv[2]);
        } catch (const std::exception& e) {
            std::cerr << "FATAL: Invalid arguments in Test Mode: " << e.what() << std::endl;
            Cleanup();
            return 1;
        }
    }

    // --- ИНТЕРАКТИВНЫЙ РЕЖИМ ---
    if (argc < 2) {
        std::cerr << "Usage (Interactive): sender_winapi.exe [senderID]" << std::endl;
        return 1;
    }
    std::string senderIdStr = argv[1];
    int senderId = std::stoi(senderIdStr);

    std::cout << "--- SENDER " << senderId << " (WinAPI FIFO) ---" << std::endl;

    // --- 2. Открытие существующих объектов ---
    hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, COMMON_MUTEX_NAME);
    hSemEmpty = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, COMMON_SEM_EMPTY_NAME);
    hSemFull = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, COMMON_SEM_FULL_NAME);
    hShutdownEvent = OpenEvent(SYNCHRONIZE, FALSE, COMMON_SHUTDOWN_EVENT_NAME);

    std::string eventName = COMMON_EVENT_PREFIX + senderIdStr;
    hReadyEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, eventName.c_str());

    if (!hMutex || !hSemEmpty || !hSemFull || !hReadyEvent || !hShutdownEvent) {
        std::cerr << "FATAL: Could not open IPC objects (" << GetLastError() << "): " << GetLastErrorAsString() << std::endl;
        Cleanup();
        return 1;
    }

    // --- 3. Открытие MMF ---
    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, COMMON_MMF_NAME);
    if (hMapFile == NULL) { Cleanup(); return 1; }

    pSharedMem = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (pSharedMem == NULL) { Cleanup(); return 1; }

    SharedHeader* pHeader = (SharedHeader*)pSharedMem;
    Message* pMessages = (Message*)(pHeader + 1);

    // --- 4. Отправить Receiver'у сигнал о готовности ---
    SetEvent(hReadyEvent);

    // --- 5. Цикл команд (Запись) ---
    std::string command;
    // Очищаем буфер после std::stoi(argv[1]), если он был
    if (std::cin.peek() == '\n') std::cin.ignore();

    while (true) {
        // Проверка на сигнал выключения в начале цикла (Неблокирующая)
        if (WaitForSingleObject(hShutdownEvent, 0) == WAIT_OBJECT_0) {
            std::cout << "\nReceiver sent Shutdown signal. Exiting..." << std::endl;
            break;
        }

        std::cout << "\n(Sender " << senderId << ") Enter command ('write' or 'exit'): ";
        // Блокирующая операция ввода
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

            std::cout << "Waiting for free slot... (Waiting on hSemEmpty or hShutdownEvent)" << std::endl;

            // Ждем либо hSemEmpty, либо hShutdownEvent
            HANDLE waitHandles[2] = {hSemEmpty, hShutdownEvent};
            DWORD dwWaitResult = WaitForMultipleObjects(2, waitHandles, FALSE, INFINITE);

            if (dwWaitResult == WAIT_OBJECT_0 + 1) { // Индекс 1: hShutdownEvent
                std::cout << "Shutdown signal received while waiting for slot. Exiting write operation." << std::endl;
                break;
            } else if (dwWaitResult != WAIT_OBJECT_0) {
                std::cerr << "FATAL: WaitForMultipleObjects failed: " << GetLastErrorAsString() << std::endl;
                break;
            }
            // Если WAIT_OBJECT_0, мы захватили hSemEmpty

            WaitForSingleObject(hMutex, INFINITE);
            pMessages[pHeader->write_pos] = msg;
            pHeader->write_pos = (pHeader->write_pos + 1) % pHeader->max_records;

            std::cout << "Message sent." << std::endl;

            ReleaseMutex(hMutex);
            ReleaseSemaphore(hSemFull, 1, NULL);
        }
    }

    // Очистка
    std::cout << "Sender " << senderId << " shutting down..." << std::endl;
    Cleanup();
    return 0;
}
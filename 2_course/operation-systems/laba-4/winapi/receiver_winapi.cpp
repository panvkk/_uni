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

// Функция для корректной очистки всех ресурсов
void Cleanup() {
    if (pSharedMem) UnmapViewOfFile(pSharedMem);
    if (hMapFile) CloseHandle(hMapFile);
    if (hMutex) CloseHandle(hMutex);
    if (hSemEmpty) CloseHandle(hSemEmpty);
    if (hSemFull) CloseHandle(hSemFull);
    for (HANDLE h : hEvents) CloseHandle(h);
    for (HANDLE h : hProcessHandles) CloseHandle(h);
}

int main() {
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

    // --- 2. Создаем объекты синхронизации (Mutex и Семафоры) ---

    // Mutex для защиты индексов write_pos/read_pos
    hMutex = CreateMutex(NULL, FALSE, COMMON_MUTEX_NAME);
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        std::cout << "Warning: Mutex existed. Reusing." << std::endl;
    }

    // Семафор "Пустые"
    hSemEmpty = CreateSemaphore(NULL, max_records, max_records, COMMON_SEM_EMPTY_NAME);
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        std::cout << "Warning: Semaphore Empty existed. Reusing." << std::endl;
    }

    // Семафор "Полные"
    hSemFull = CreateSemaphore(NULL, 0, max_records, COMMON_SEM_FULL_NAME);
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        std::cout << "Warning: Semaphore Full existed. Reusing." << std::endl;
    }

    if (!hMutex || !hSemEmpty || !hSemFull) {
        std::cerr << "FATAL: Failed to create IPC objects: " << GetLastErrorAsString() << std::endl;
        Cleanup();
        return 1;
    }

    // --- 3. Создаем бинарный файл и MMF (С усиленной проверкой) ---
    size_t MMF_SIZE = sizeof(SharedHeader) + max_records * sizeof(Message);

    // Проверка на большие размеры
    if (MMF_SIZE > 0xFFFFFFFF) {
        std::cerr << "FATAL: MMF_SIZE exceeds 4GB limit for this simple call." << std::endl;
        return 1;
    }

    // Файл на диске
    HANDLE hFile = CreateFile(
        fileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "FATAL: Could not create or open backing file (" << GetLastError() << "): " << GetLastErrorAsString() << std::endl;
        return 1;
    }

    // MMF: Явно разбиваем размер на High (0) и Low
    DWORD dwSizeLow = (DWORD)MMF_SIZE;
    DWORD dwSizeHigh = 0;

    // Сбрасываем GetLastError перед вызовом CreateFileMapping, чтобы поймать 183
    SetLastError(0);

    hMapFile = CreateFileMapping(
        hFile, NULL, PAGE_READWRITE, dwSizeHigh, dwSizeLow, COMMON_MMF_NAME
    );

    DWORD error_after_mmf = GetLastError();
    CloseHandle(hFile); // Закрываем хендл файла-подложки

    // Проверка MMF
    if (hMapFile == NULL) {
        std::cerr << "FATAL: Could not create file mapping '" << COMMON_MMF_NAME << "' (" << error_after_mmf << "): " << GetLastErrorAsString() << std::endl;
        Cleanup();
        return 1;
    }

    // Если MMF уже существовал, мы просто продолжаем и переинициализируем данные
    if (error_after_mmf == ERROR_ALREADY_EXISTS) {
        std::cout << "Warning: MMF existed. Reusing and REINITIALIZING data." << std::endl;
    }

    pSharedMem = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, MMF_SIZE);
    if (pSharedMem == NULL) {
        std::cerr << "FATAL: Could not map view of file (" << GetLastError() << "): " << GetLastErrorAsString() << std::endl;
        Cleanup();
        return 1;
    }

    // Инициализируем заголовок в общей памяти
    SharedHeader* pHeader = (SharedHeader*)pSharedMem;
    pHeader->max_records = max_records;
    pHeader->write_pos = 0;
    pHeader->read_pos = 0;

    // Указатель на массив сообщений (сразу после заголовка)
    Message* pMessages = (Message*)(pHeader + 1);

    // --- 4. Подготовка к запуску процессов (с отладкой пути) ---
    std::vector<PROCESS_INFORMATION> vSenderProcesses(sender_count);

    char currentDirBuffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, currentDirBuffer);
    std::string currentDir = currentDirBuffer;

    std::string senderBaseCommand = currentDir + "\\sender_winapi.exe ";

    std::cout << "DEBUG: Base command path: " << senderBaseCommand << std::endl;

    // --- 5. Запускаем Sender'ов ---
    for (int i = 0; i < sender_count; ++i) {
        std::string eventName = COMMON_EVENT_PREFIX + std::to_string(i);
        HANDLE hReadyEvent = CreateEvent(NULL, FALSE, FALSE, eventName.c_str());
        hEvents.push_back(hReadyEvent);

        std::string cmdLine = senderBaseCommand + std::to_string(i);

        STARTUPINFOA si;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);

        char cmdBuffer[256];
        strncpy(cmdBuffer, cmdLine.c_str(), 255);
        cmdBuffer[255] = '\0';

        if (!CreateProcessA(
            NULL, cmdBuffer, NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, NULL, &si, &vSenderProcesses[i]
        )) {
            // !!! БЛОК ВЫВОДА ОШИБКИ С FLUSH !!!
            std::cerr << "\nFATAL ERROR: Failed to launch Sender " << i << "." << std::endl;
            std::cerr << "Command line used: " << cmdBuffer << std::endl;
            std::cerr << "WinAPI Error Code: " << GetLastError() << std::endl;
            std::cerr << "WinAPI Error Message: " << GetLastErrorAsString() << std::endl;
            std::cerr << std::flush;
            Cleanup();
            return 1;
        }

        hProcessHandles.push_back(vSenderProcesses[i].hProcess);
        CloseHandle(vSenderProcesses[i].hThread);
    }

    // --- 6. Ждем сигнал на готовность от всех Sender'ов ---
    std::cout << "Waiting for all " << sender_count << " senders to be ready..." << std::endl;
    WaitForMultipleObjects(sender_count, hEvents.data(), TRUE, INFINITE);
    std::cout << "All senders are ready!" << std::endl;

    // --- 7. Цикл команд (Чтение) ---
    std::string command;
    while (true) {
        std::cout << "\nEnter command ('read' or 'exit'): ";
        std::cin >> command;

        if (command == "exit") {
            break;
        }

        if (command == "read") {
            std::cout << "Waiting for message... (Waiting on hSemFull)" << std::endl;

            // 1. Ждем, пока семафор "Полные" > 0 (Блокировка на пустоте)
            WaitForSingleObject(hSemFull, INFINITE);

            // 2. Блокируем Mutex
            WaitForSingleObject(hMutex, INFINITE);

            // 3. Читаем и сдвигаем read_pos
            Message msg = pMessages[pHeader->read_pos];
            pHeader->read_pos = (pHeader->read_pos + 1) % pHeader->max_records;

            std::cout << "Received: " << msg.data << std::endl;

            // 4. Освобождаем Mutex
            ReleaseMutex(hMutex);

            // 5. Сигналим, что освободился один "Пустой" слот
            ReleaseSemaphore(hSemEmpty, 1, NULL);
        }
    }

    // Очистка
    std::cout << "Receiver shutting down. Cleaning up resources..." << std::endl;
    Cleanup();
    return 0;
}
#pragma once
#include <windows.h>
#include <string>
#include <sstream>

#define MESSAGE_LENGTH 21 // 20 символов + '\0'

// --- Имена глобальных объектов ядра ---
// Используем "Global\" префикс для явного создания в глобальном пространстве имен
#define COMMON_MMF_NAME "Global\\IPC_FIFO_MMF"
#define COMMON_MUTEX_NAME "Global\\IPC_FIFO_Mutex"
#define COMMON_SEM_EMPTY_NAME "Global\\IPC_FIFO_SemEmpty"
#define COMMON_SEM_FULL_NAME "Global\\IPC_FIFO_SemFull"
#define COMMON_EVENT_PREFIX "Global\\IPC_FIFO_Event_"
// Событие выключения, необходимое для корректного завершения в тестах
#define COMMON_SHUTDOWN_EVENT_NAME "Global\\IPC_FIFO_Shutdown"

// Сообщение
struct Message {
    char data[MESSAGE_LENGTH];
};

// Контрольный блок в начале общей памяти
struct SharedHeader {
    size_t max_records;
    size_t write_pos; // Индекс, куда пишет Sender
    size_t read_pos;  // Индекс, откуда читает Receiver
};

// Вспомогательная функция для перевода кода ошибки WinAPI в читаемую строку
inline std::string GetLastErrorAsString() {
    DWORD errorMessageID = GetLastError();
    if (errorMessageID == 0) return std::string();

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);
    LocalFree(messageBuffer);

    size_t end = message.find_last_not_of(" \n\r\t");
    if (std::string::npos != end) {
        message = message.substr(0, end + 1);
    }
    return message;
}
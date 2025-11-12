#pragma once
#include <windows.h>
#include <string>
#include <sstream>

#define MESSAGE_LENGTH 21

#define COMMON_MMF_NAME "Global\\IPC_FIFO_MMF"
#define COMMON_MUTEX_NAME "Global\\IPC_FIFO_Mutex"
#define COMMON_SEM_EMPTY_NAME "Global\\IPC_FIFO_SemEmpty"
#define COMMON_SEM_FULL_NAME "Global\\IPC_FIFO_SemFull"
#define COMMON_EVENT_PREFIX "Global\\IPC_FIFO_Event_"

struct Message {
    char data[MESSAGE_LENGTH];
};

struct SharedHeader {
    size_t max_records;
    size_t write_pos;
    size_t read_pos;
};

inline std::string GetLastErrorAsString() {
    DWORD errorMessageID = GetLastError();
    if (errorMessageID == 0)
        return std::string();

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
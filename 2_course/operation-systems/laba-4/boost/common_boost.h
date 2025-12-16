#pragma once
#include <windows.h>
#include <string>
#include <sstream>

#define MESSAGE_LENGTH 21

// Имена глобальных объектов синхронизации
#define COMMON_MMF_NAME "Global\\IPC_FIFO_MMF"
#define COMMON_MUTEX_NAME "Global\\IPC_FIFO_Mutex"
#define COMMON_SEM_EMPTY_NAME "Global\\IPC_FIFO_SemEmpty"
#define COMMON_SEM_FULL_NAME "Global\\IPC_FIFO_SemFull"

struct Message {
    char data[MESSAGE_LENGTH];
};

struct SharedHeader {
    size_t max_records;
    size_t write_pos;
    size_t read_pos;
    // Мьютексы и семафоры здесь хранить НЕ надо,
    // так как мы используем именованные объекты ОС.
};
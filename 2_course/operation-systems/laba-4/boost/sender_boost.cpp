#include <iostream>
#include <string>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

namespace bip = boost::interprocess;

struct Message {
    char data[21];
};

struct SharedData {
    // В Sender'е нам не нужен конструктор, т.к. мы открываем существующую память
    bip::interprocess_mutex mutex;
    bip::interprocess_semaphore sem_empty;
    bip::interprocess_semaphore sem_full;
    size_t max_records;
    size_t write_pos;
    size_t read_pos;
    size_t ready_count;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: Missing file name." << std::endl;
        return 1;
    }
    std::string fileName = argv[1];

    // 1. Открываем существующий MMF
    bip::file_mapping f_map(fileName.c_str(), bip::read_write);
    // Маппим весь файл
    bip::mapped_region region(f_map, bip::read_write);

    // Получаем указатели
    SharedData* pData = static_cast<SharedData*>(region.get_address());
    Message* pMessages = (Message*)(pData + 1);
    size_t max_records = pData->max_records;

    // 2. Сигнал о готовности (атомарно)
    {
        bip::scoped_lock<bip::interprocess_mutex> lock(pData->mutex);
        pData->ready_count++;
    }

    // 3. Цикл команд
    std::string command;
    while (true) {
        std::cout << "(Sender) Enter command ('write' or 'exit'): ";
        std::cin >> command;

        if (command == "exit") {
            break;
        }

        if (command == "write") {
            std::string msgText;
            std::cout << "Enter message (max 20 chars): ";
            std::cin.ignore();
            std::getline(std::cin, msgText);

            Message msg;
            strncpy(msg.data, msgText.c_str(), 20);
            msg.data[20] = '\0';

            std::cout << "Waiting for free slot..." << std::endl;
            pData->sem_empty.wait(); // Ждем, если полно
            bip::scoped_lock<bip::interprocess_mutex> lock(pData->mutex);
            
            pMessages[pData->write_pos] = msg;
            pData->write_pos = (pData->write_pos + 1) % max_records;

            lock.unlock();
            pData->sem_full.post(); // Сигналим, что сообщение добавлено
            
            std::cout << "Message sent." << std::endl;
        }
    }
    return 0;
}
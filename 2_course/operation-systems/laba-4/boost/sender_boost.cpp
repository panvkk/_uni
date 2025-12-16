#include <iostream>
#include <string>
#include <vector>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include "common_boost.h"

namespace bip = boost::interprocess;

int main(int argc, char* argv[]) {
    // Получаем ID сендера для красоты вывода (передали через аргументы)
    std::string senderId = (argc > 1) ? argv[1] : "Unknown";

    // Меняем заголовок окна консоли, чтобы различать их
    std::string title = "Sender " + senderId;
    SetConsoleTitleA(title.c_str());

    std::cout << "--- SENDER " << senderId << " ---" << std::endl;

    try {
        // 1. Открываем существующую Shared Memory
        bip::shared_memory_object shm(bip::open_only, COMMON_MMF_NAME, bip::read_write);
        bip::mapped_region region(shm, bip::read_write);

        // 2. Открываем существующие объекты синхронизации
        bip::named_mutex mutex(bip::open_only, COMMON_MUTEX_NAME);
        bip::named_semaphore semEmpty(bip::open_only, COMMON_SEM_EMPTY_NAME);
        bip::named_semaphore semFull(bip::open_only, COMMON_SEM_FULL_NAME);

        // 3. Получаем указатели
        SharedHeader* pHeader = static_cast<SharedHeader*>(region.get_address());
        Message* pMessages = reinterpret_cast<Message*>(pHeader + 1);

        std::string command;
        while (true) {
            std::cout << "[" << senderId << "] Enter command ('write' or 'exit'): ";
            std::cin >> command;

            if (command == "exit") {
                break;
            }

            if (command == "write") {
                std::string msgText;
                std::cout << "Enter message (max 20 chars): ";
                std::cin.ignore();
                std::getline(std::cin, msgText);

                // Подготовка сообщения
                Message msg;
                strncpy_s(msg.data, msgText.c_str(), 20);
                msg.data[20] = '\0';

                std::cout << "Waiting for free slot..." << std::endl;

                // Ждем свободное место
                semEmpty.wait();

                // Заходим в критическую секцию
                {
                    bip::scoped_lock<bip::named_mutex> lock(mutex);

                    pMessages[pHeader->write_pos] = msg;
                    pHeader->write_pos = (pHeader->write_pos + 1) % pHeader->max_records;

                    std::cout << "Message written to index " << (pHeader->write_pos - 1 + pHeader->max_records) % pHeader->max_records << std::endl;
                } // Mutex разблокируется здесь

                // Сигнализируем, что появилось новое сообщение
                semFull.post();
            }
        }

    } catch (const bip::interprocess_exception& e) {
        std::cerr << "Sender Error: " << e.what() << std::endl;
        std::cout << "Make sure Receiver is running first!" << std::endl;
        system("pause");
        return 1;
    }

    return 0;
}
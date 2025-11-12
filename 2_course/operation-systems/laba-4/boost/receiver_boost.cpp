#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <system_error>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/process/spawn.hpp>
#include <boost/process/child.hpp>
#include <boost/process/io.hpp>
#include <boost/process/error.hpp>
#include <windows.h>
#include <direct.h>

#include "common_boost.h"

namespace bip = boost::interprocess;
namespace bp = boost::process;

using namespace boost::process;
using boost::process::std_out;
using boost::process::null;

bip::mapped_region* pRegion = nullptr;
bip::shared_memory_object* pShm = nullptr;
bip::named_mutex* pMutex = nullptr;
bip::named_semaphore* pSemEmpty = nullptr;
bip::named_semaphore* pSemFull = nullptr;
std::vector<bp::child> vSenderProcesses;


void Cleanup() {
    std::cout << "\nCleanup initiated..." << std::endl;

    if (pRegion) delete pRegion;
    if (pShm) delete pShm;
    if (pMutex) delete pMutex;
    if (pSemEmpty) delete pSemEmpty;
    if (pSemFull) delete pSemFull;

    for (auto& child : vSenderProcesses) {
        if (child.running()) {
            child.terminate();
        }
    }
    vSenderProcesses.clear();

    try {
        bip::shared_memory_object::remove(COMMON_MMF_NAME);
        bip::named_semaphore::remove(COMMON_SEM_FULL_NAME);
        bip::named_semaphore::remove(COMMON_SEM_EMPTY_NAME);
        bip::named_mutex::remove(COMMON_MUTEX_NAME);
        std::cout << "Global IPC objects removed." << std::endl;
    } catch (const bip::interprocess_exception& e) {
        std::cerr << "Warning during cleanup (may be normal if object was already gone): " << e.what() << std::endl;
    }
}


int main() {
    std::string fileName;
    size_t max_records;
    int sender_count;

    try {
        bip::shared_memory_object::remove(COMMON_MMF_NAME);
        bip::named_semaphore::remove(COMMON_SEM_FULL_NAME);
        bip::named_semaphore::remove(COMMON_SEM_EMPTY_NAME);
        bip::named_mutex::remove(COMMON_MUTEX_NAME);
    } catch (const bip::interprocess_exception& e) {
    }

    std::cout << "--- RECEIVER (Boost FIFO) ---" << std::endl;
    std::cout << "Enter file name (for MMF backing): ";
    std::cin >> fileName;
    std::cout << "Enter max records (FIFO size): ";
    std::cin >> max_records;
    std::cout << "Enter sender count: ";
    std::cin >> sender_count;

    const size_t MMF_SIZE = sizeof(SharedHeader) + max_records * sizeof(Message);

    try {
        pShm = new bip::shared_memory_object(
            bip::create_only,
            COMMON_MMF_NAME,
            bip::read_write
        );
        pShm->truncate(MMF_SIZE);

        pRegion = new bip::mapped_region(*pShm, bip::read_write);

        pMutex = new bip::named_mutex(bip::create_only, COMMON_MUTEX_NAME);

        pSemEmpty = new bip::named_semaphore(bip::create_only, COMMON_SEM_EMPTY_NAME, max_records);
        pSemFull = new bip::named_semaphore(bip::create_only, COMMON_SEM_FULL_NAME, 0);

    } catch (const bip::interprocess_exception& e) {
        std::cerr << "FATAL ERROR during IPC object creation: " << e.what() << std::endl;
        Cleanup();
        return 1;
    }

    void* addr = pRegion->get_address();
    SharedHeader* pHeader = static_cast<SharedHeader*>(addr);

    pHeader->max_records = max_records;
    pHeader->write_pos = 0;
    pHeader->read_pos = 0;

    Message* pMessages = reinterpret_cast<Message*>(pHeader + 1);


    char currentDirBuffer[MAX_PATH];
    _getcwd(currentDirBuffer, MAX_PATH);
    std::string currentDir = currentDirBuffer;

    std::string senderBaseCommand = currentDir + "\\sender_boost.exe";
    std::cout << "DEBUG: Base command path: " << senderBaseCommand << std::endl;
    std::cout << "Launching " << sender_count << " Sender processes..." << std::endl;

    for (int i = 0; i < sender_count; ++i) {
        try {
            bp::child sender_child(
                senderBaseCommand,
                std::to_string(i),
                bp::std_out > bp::null,
                bp::throw_on_error
            );
            vSenderProcesses.push_back(std::move(sender_child));
        } catch (const bp::process_error& e) {
            std::cerr << "FATAL ERROR: Failed to launch Sender " << i << " (" << e.what() << ")" << std::endl;
            Cleanup();
            return 1;
        }
    }

    std::cout << "All Senders launched. Starting interaction loop." << std::endl;

    std::string command;
    while (true) {
        // ... (остальной код main, без изменений)
        std::cout << "\nEnter command ('read' or 'exit'): ";
        std::cin >> command;

        if (command == "exit") {
            break;
        }

        if (command == "read") {
            std::cout << "Waiting for message... (Waiting on hSemFull)" << std::endl;

            // 1. Ждем, пока семафор "Полные" > 0 (Блокировка на пустоте)
            pSemFull->wait();

            // 2. Блокируем Mutex
            bip::scoped_lock<bip::named_mutex> lock(*pMutex);

            // 3. Читаем и сдвигаем read_pos
            Message msg = pMessages[pHeader->read_pos];
            pHeader->read_pos = (pHeader->read_pos + 1) % pHeader->max_records;

            std::cout << "Received: " << msg.data << std::endl;

            // 4. Освобождаем Mutex (Деструктор lock делает это автоматически)

            // 5. Сигналим, что освободился один "Пустой" слот
            pSemEmpty->post();
        }
    }

    // Очистка
    Cleanup();
    return 0;
}
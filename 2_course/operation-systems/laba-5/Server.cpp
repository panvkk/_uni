#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <thread>
#include <cstring>
#include <shared_mutex>
#include <mutex>
#include <algorithm>
#include "Common.h"

class FileStore {
    std::string filename;
    std::vector<employee> records;
    std::map<int, std::unique_ptr<std::shared_mutex>> record_mutexes;
    std::mutex file_mutex;

public:
    FileStore(std::string name) : filename(name) {}

    void addRecord(employee emp) {
        records.push_back(emp);
        record_mutexes[emp.num] = std::make_unique<std::shared_mutex>();
    }

    void saveToFile() {
        std::ofstream out(filename, std::ios::binary);
        out.write(reinterpret_cast<char*>(records.data()), records.size() * sizeof(employee));
    }

    void printAll() {
        std::cout << "\n--- Current File Content ---\n";
        for (const auto& e : records) {
            std::cout << "ID: " << e.num << ", Name: " << e.name << ", Hours: " << e.hours << std::endl;
        }
    }

    bool handleRequest(const PipeRequest& req, PipeResponse& res) {
        auto it = std::find_if(records.begin(), records.end(), [&](const employee& e) { return e.num == req.employee_id; });
        if (it == records.end()) {
            res.success = false;
            strncpy(res.message, "Employee not found", sizeof(res.message) - 1);
            return false;
        }

        if (req.type == RequestType::READ_START) {
            record_mutexes[req.employee_id]->lock_shared();
            res.data = *it;
            res.success = true;
        } 
        else if (req.type == RequestType::WRITE_START) {
            record_mutexes[req.employee_id]->lock();
            res.data = *it;
            res.success = true;
        }
        else if (req.type == RequestType::READ_END) {
            record_mutexes[req.employee_id]->unlock_shared();
            res.success = true;
        }
        else if (req.type == RequestType::WRITE_END) {
            *it = req.data;
            saveToFile();
            record_mutexes[req.employee_id]->unlock();
            res.success = true;
        }
        return true;
    }
};

void clientHandler(HANDLE hPipe, FileStore& store) {
    PipeRequest req;
    DWORD bytesRead, bytesWritten;

    while (ReadFile(hPipe, &req, sizeof(req), &bytesRead, NULL) && bytesRead > 0) {
        PipeResponse res = {};
        store.handleRequest(req, res);
        WriteFile(hPipe, &res, sizeof(res), &bytesWritten, NULL);
    }
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
}

int main() {
    std::string filename;
    int empCount, clientCount;

    std::cout << "Enter filename: "; std::cin >> filename;
    FileStore store(filename);

    std::cout << "Enter number of employees: "; std::cin >> empCount;
    for (int i = 0; i < empCount; ++i) {
        employee e;
        std::cout << "ID Name Hours: ";
        std::cin >> e.num >> e.name >> e.hours;
        store.addRecord(e);
    }
    store.saveToFile();
    store.printAll();

    std::cout << "Enter number of clients: "; std::cin >> clientCount;

    std::vector<std::thread> threads;
    for (int i = 0; i < clientCount; ++i) {
        STARTUPINFOA si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        std::string cmd = ".\\Client.exe";
        CreateProcessA(NULL, (LPSTR)cmd.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        HANDLE hPipe = CreateNamedPipeA(PIPE_NAME.c_str(), PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES, sizeof(PipeResponse), sizeof(PipeRequest), 0, NULL);
        
        ConnectNamedPipe(hPipe, NULL);
        threads.emplace_back(clientHandler, hPipe, std::ref(store));
    }

    for (auto& t : threads) { if(t.joinable()) t.join(); }

    std::cout << "\nFinal file state:";
    store.printAll();
    
    std::cout << "\nPress any key to exit...";
    char c; std::cin >> c;
    return 0;
}
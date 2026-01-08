#include <iostream>
#include <windows.h>
#include "Common.h"

int main() {
    HANDLE hPipe = CreateFileA(PIPE_NAME.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hPipe == INVALID_HANDLE_VALUE) return 1;

    while (true) {
        std::cout << "\n1. Read Record\n2. Modify Record\n3. Exit\nChoice: ";
        int choice; std::cin >> choice;
        if (choice == 3) break;

        int id;
        std::cout << "Enter Employee ID: "; std::cin >> id;

        PipeRequest req = {};
        req.employee_id = id;
        req.type = (choice == 1) ? RequestType::READ_START : RequestType::WRITE_START;

        DWORD bytes;
        PipeResponse res;
        WriteFile(hPipe, &req, sizeof(req), &bytes, NULL);
        ReadFile(hPipe, &res, sizeof(res), &bytes, NULL);

        if (!res.success) {
            std::cout << "Error: " << res.message << std::endl;
            continue;
        }

        std::cout << "Data: " << res.data.name << ", Hours: " << res.data.hours << std::endl;

        if (choice == 2) {
            std::cout << "Enter new Name and Hours: ";
            std::cin >> res.data.name >> res.data.hours;
            req.type = RequestType::WRITE_END;
            req.data = res.data;
        } else {
            std::cout << "Press enter to finish reading...";
            std::string dummy; std::getline(std::cin >> std::ws, dummy);
            req.type = RequestType::READ_END;
        }

        WriteFile(hPipe, &req, sizeof(req), &bytes, NULL);
        ReadFile(hPipe, &res, sizeof(res), &bytes, NULL);
    }

    CloseHandle(hPipe);
    return 0;
}
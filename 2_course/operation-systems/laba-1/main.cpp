#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include "common/employee.h"
#include <direct.h>

void startProcess(const std::string& command) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char* cmdLine = _strdup(command.c_str());

    if (!CreateProcessA(NULL,
                        cmdLine,
                        NULL,
                        NULL,
                        FALSE,
                        0,
                        NULL,
                        NULL,
                        &si,
                        &pi)) {
        std::cerr << "CreateProcess failed (" << GetLastError() << ")." << std::endl;
        free(cmdLine);
        return;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    free(cmdLine);
}

void printBinaryFile(const std::string& fileName) {
    std::ifstream inFile(fileName, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening binary file to read: " << fileName << std::endl;
        return;
    }
    std::cout << "\n--- Contents of " << fileName << " ---" << std::endl;
    employee emp;
    while(inFile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        std::cout << "ID: " << emp.num << ", Name: " << emp.name << ", Hours: " << emp.hours << std::endl;
    }
    inFile.close();
}

void printReportFile(const std::string& fileName) {
    std::ifstream inFile(fileName);
    if (!inFile) {
        std::cerr << "Error opening report file to read: " << fileName << std::endl;
        return;
    }

    char bom[3];
    inFile.read(bom, 3);
    if (!(bom[0] == '\xEF' && bom[1] == '\xBB' && bom[2] == '\xBF')) {
        inFile.seekg(0); // Если это не BOM, возвращаемся к началу файла
    }

    std::cout << "\n--- Report from " << fileName << " ---" << std::endl;
    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << line << std::endl;
    }
    inFile.close();
    std::cout << "--------------------------------\n" << std::endl;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    char buffer[1024];
    _getcwd(buffer, 1024);
    std::cout << "Current working directory: " << buffer << std::endl;

    std::string binaryFileName;
    int recordCount;

    std::cout << "Enter binary file name: ";
    std::cin >> binaryFileName;
    std::cout << "Enter number of records: ";
    std::cin >> recordCount;

    std::string creatorCommand = std::string(buffer) + "\\Creator.exe " + binaryFileName + " " + std::to_string(recordCount);
    std::cout << "\nStarting Creator process..." << std::endl;
    startProcess(creatorCommand);
    std::cout << "Creator process finished." << std::endl;

    printBinaryFile(binaryFileName);

    std::string reportFileName;
    double hourlyRate;
    std::cout << "Enter report file name: ";
    std::cin >> reportFileName;
    std::cout << "Enter hourly rate: ";
    std::cin >> hourlyRate;

    std::string reporterCommand = std::string(buffer) + "\\Reporter.exe " + binaryFileName + " " + reportFileName + " " + std::to_string(hourlyRate);
    std::cout << "\nStarting Reporter process..." << std::endl;
    startProcess(reporterCommand);
    std::cout << "Reporter process finished." << std::endl;

    printReportFile(reportFileName);

    std::cout << "Main process finished." << std::endl;
    return 0;
}
#include <iostream>
#include <vector>
#include <windows.h>

#include "creator_utils.h"
#include "../common/employee.h"
#include "common/file_utils.h"

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    if (!validateCreatorArguments(argc, argv)) {
        std::cerr << "Usage: " << argv[0] << " <binary_file_name> <number_of_records>" << std::endl;
        return 1;
    }

    const char* fileName = argv[1];
    int recordCount = std::stoi(argv[2]);

    std::vector<employee> employees;
    std::cout << "Enter " << recordCount << " employee records:" << std::endl;

    for (int i = 0; i < recordCount; ++i) {
        std::cout << "Record " << i + 1 << ":" << std::endl;
        employees.push_back(inputEmployee());
    }

    if (createEmployeeBinaryFile(fileName, employees)) {
        std::cout << "Binary file '" << fileName << "' created successfully." << std::endl;
        return 0;
    } else {
        std::cerr << "Error creating binary file: " << fileName << std::endl;
        return 1;
    }
}

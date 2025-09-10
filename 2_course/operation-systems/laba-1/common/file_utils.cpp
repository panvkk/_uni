#include "file_utils.h"
#include <fstream>
#include <stdexcept>

std::vector<employee> readEmployeeBinaryFile(const std::string& fileName) {
    std::vector<employee> employees;
    std::ifstream inFile(fileName, std::ios::binary);
    
    if (!inFile) {
        throw std::runtime_error("Cannot open file: " + fileName);
    }
    
    employee emp;
    while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        employees.push_back(emp);
    }
    
    inFile.close();
    return employees;
}

bool createEmployeeBinaryFile(const std::string& fileName, 
                              const std::vector<employee>& employees) {
    std::ofstream outFile(fileName, std::ios::binary);
    if (!outFile) {
        return false;
    }
    
    for (const auto& emp : employees) {
        outFile.write(reinterpret_cast<const char*>(&emp), sizeof(employee));
    }
    
    outFile.close();
    return true;
}
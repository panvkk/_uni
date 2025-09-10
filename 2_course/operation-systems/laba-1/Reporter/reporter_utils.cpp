#include "reporter_utils.h"
#include "../common/file_utils.h"
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>

bool generateReport(const std::string& binaryFileName, 
                    const std::string& reportFileName, 
                    double hourlyRate) {
    try {
        std::vector<employee> employees = readEmployeeBinaryFile(binaryFileName);
        std::sort(employees.begin(), employees.end(), compareEmployees);
        
        std::ofstream outFile(reportFileName);
        if (!outFile) {
            return false;
        }
        
        // Добавляем BOM для UTF-8
        outFile << "\xEF\xBB\xBF";
        
        outFile << createReportHeader(binaryFileName) << "\n\n";
        
        outFile << std::left
                << std::setw(10) << "Номер" 
                << std::setw(15) << "Имя" 
                << std::setw(10) << "Часы" 
                << std::setw(12) << "Зарплата" 
                << "\n";
        
        outFile << std::string(47, '-') << "\n";
        
        outFile << std::fixed << std::setprecision(2);
        
        for (const auto& emp : employees) {
            outFile << formatReportLine(emp, hourlyRate) << "\n";
        }
        
        outFile.close();
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

bool validateReporterArguments(int argc, char* argv[]) {
    return argc == 4;
}

std::string formatReportLine(const employee& emp, double hourlyRate) {
    std::ostringstream oss;
    double salary = emp.hours * hourlyRate;
    
    oss << std::fixed << std::setprecision(2);

    oss << std::left
        << std::setw(10) << emp.num 
        << std::setw(15) << emp.name 
        << std::setw(10) << emp.hours 
        << std::setw(12) << salary;
    
    return oss.str();
}

std::string createReportHeader(const std::string& binaryFileName) {
    return "Отчет по файлу \"" + binaryFileName + "\"";
}
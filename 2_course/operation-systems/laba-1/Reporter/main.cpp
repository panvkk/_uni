#include <iostream>
#include <windows.h>
#include "reporter_utils.h"

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    if (!validateReporterArguments(argc, argv)) {
        std::cerr << "Usage: " << argv[0] << " <binary_file_name> <report_file_name> <hourly_rate>" << std::endl;
        return 1;
    }

    const char* binaryFileName = argv[1];
    const char* reportFileName = argv[2];
    double hourlyRate = std::stod(argv[3]);

    if (generateReport(binaryFileName, reportFileName, hourlyRate)) {
        std::cout << "Report file '" << reportFileName << "' created successfully." << std::endl;
        return 0;
    } else {
        std::cerr << "Error creating report file: " << reportFileName << std::endl;
        return 1;
    }
}
//
// Created by panov on 10.09.2025.
//

#ifndef LABA_1_REPORTER_UTILS_H
#define LABA_1_REPORTER_UTILS_H

#endif //LABA_1_REPORTER_UTILS_H
#pragma once
#include "../common/employee.h"
#include <vector>
#include <string>

bool generateReport(const std::string& binaryFileName,
                    const std::string& reportFileName,
                    double hourlyRate);

bool validateReporterArguments(int argc, char* argv[]);

std::string formatReportLine(const employee& emp, double hourlyRate);

std::string createReportHeader(const std::string& binaryFileName);
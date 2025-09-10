#ifndef LABA_1_FILE_UTILS_H
#define LABA_1_FILE_UTILS_H

#endif //LABA_1_FILE_UTILS_H
#pragma once
#include "employee.h"
#include <vector>
#include <string>

std::vector<employee> readEmployeeBinaryFile(const std::string& fileName);

bool createEmployeeBinaryFile(const std::string& fileName,
                              const std::vector<employee>& employees);
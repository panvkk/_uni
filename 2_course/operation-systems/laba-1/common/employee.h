#ifndef LABA_1_EMPLOYEE_H
#define LABA_1_EMPLOYEE_H

#endif //LABA_1_EMPLOYEE_H

#pragma once

struct employee {
    int num;         // id
    char name[10];   // имя сотрудника
    double hours;    // количество отработанных часов
};

inline bool compareEmployees(const employee& a, const employee& b) {
    return a.num < b.num;
}
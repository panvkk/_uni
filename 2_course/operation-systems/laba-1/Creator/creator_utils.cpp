#include "creator_utils.h"
#include <iostream>

employee inputEmployee() {
    employee emp;
    std::cout << "  ID: ";
    std::cin >> emp.num;
    std::cout << "  Name (up to 9 chars): ";
    std::cin >> emp.name;
    std::cout << "  Hours: ";
    std::cin >> emp.hours;
    return emp;
}

bool validateCreatorArguments(int argc, char* argv[]) {
    return argc == 3;
}
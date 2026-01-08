#pragma once
#include <windows.h>
#include <string>

struct employee {
    int num;
    char name[10];
    double hours;
};

enum class RequestType {
    READ_START,
    WRITE_START,
    READ_END,
    WRITE_END
};

struct PipeRequest {
    RequestType type;
    int employee_id;
    employee data;
};

struct PipeResponse {
    bool success;
    employee data;
    char message[256];
};

const std::string PIPE_NAME = "\\\\.\\pipe\\EmployeePipe";
cmake_minimum_required(VERSION 3.14)
project(WinApiThreadMarker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT WIN32)
    message(FATAL_ERROR "This project is for Windows only.")
endif()

add_executable(winapi_marker
        src/main_winapi.cpp
        src/marker_logic_winapi.cpp
)

enable_testing()

add_executable(winapi_tests
        tests/test_winapi.cpp
        src/marker_logic_winapi.cpp
)

add_test(NAME WinApiVersionCleanupTest COMMAND winapi_tests)
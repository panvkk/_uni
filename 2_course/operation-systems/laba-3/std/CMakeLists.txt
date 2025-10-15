cmake_minimum_required(VERSION 3.14)
project(StdThreadMarker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(std_marker
        src/main_std.cpp
        src/marker_logic_std.cpp
)

enable_testing()

add_executable(std_tests
        tests/test_std.cpp
        src/marker_logic_std.cpp
)

add_test(NAME StdVersionCleanupTest COMMAND std_tests)
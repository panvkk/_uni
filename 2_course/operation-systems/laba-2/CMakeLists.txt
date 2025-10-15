cmake_minimum_required(VERSION 4.0)
project(laba_2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(laba_2_std main_std.cpp)
add_executable(laba_2_winapi main_winapi.cpp)

find_package(Catch2 3 REQUIRED)
add_executable(tests test/tests.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

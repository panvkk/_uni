cmake_minimum_required(VERSION 3.10)
project(BoostThreadMarkers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.71.0 REQUIRED COMPONENTS thread system)

if(Boost_FOUND)
    add_executable(boost_markers main_boost.cpp)

    target_include_directories(boost_markers PRIVATE ${Boost_INCLUDE_DIRS})

    target_link_libraries(boost_markers PRIVATE ${Boost_LIBRARIES})
else()
    message(FATAL_ERROR "Библиотека Boost не найдена.")
endif()
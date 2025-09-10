cmake_minimum_required(VERSION 4.0)
project(laba_1)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Собираем утилиту Creator с всеми необходимыми исходными файлами
add_executable(Creator
        Creator/main.cpp
        Creator/creator_utils.cpp
        common/file_utils.cpp
)
target_include_directories(Creator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Собираем утилиту Reporter с всеми необходимыми исходными файлами
add_executable(Reporter
        Reporter/main.cpp
        Reporter/reporter_utils.cpp
        common/file_utils.cpp
)
target_include_directories(Reporter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Собираем главную программу Main
add_executable(Main main.cpp)
target_include_directories(Main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Для Windows
if (WIN32)
    target_link_libraries(Main kernel32)
endif()

# Подключаем тесты, только если папка test существует
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt)
    add_subdirectory(test)
else()
    message(WARNING "Test directory not found, skipping tests")
endif()
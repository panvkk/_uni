add_executable(
        ProcessLabTests
        unit/test_creator.cpp
        unit/test_reporter.cpp
        unit/test_main.cpp
        ../Creator/creator_utils.cpp
        ../Reporter/reporter_utils.cpp
        ../common/file_utils.cpp
)

target_include_directories(ProcessLabTests PUBLIC ${CMAKE_SOURCE_DIR})

target_link_libraries(ProcessLabTests gtest_main)

include(GoogleTest)
gtest_discover_tests(ProcessLabTests)
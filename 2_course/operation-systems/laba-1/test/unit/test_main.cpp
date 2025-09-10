#include "gtest/gtest.h"
#include <windows.h>
#include <string>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class MainTest : public ::testing::Test {
protected:
    void SetUp() override {
        testDir = fs::temp_directory_path() / "ProcessLabTests";
        fs::create_directories(testDir);
    }
    
    void TearDown() override {
        fs::remove_all(testDir);
    }
    
    fs::path testDir;
};

TEST_F(MainTest, StartProcess) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    
    BOOL success = CreateProcessA(
        NULL,
        (LPSTR)"calc.exe",
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    );
    
    if (success) {
        EXPECT_TRUE(success);
        WaitForSingleObject(pi.hProcess, 1000); // Ждем немного
        TerminateProcess(pi.hProcess, 0); // Завершаем процесс
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        GTEST_SKIP() << "Calculator not available, skipping process test";
    }
}

TEST_F(MainTest, FileOperations) {
    std::string testFile = (testDir / "test.txt").string();
    std::ofstream outFile(testFile);
    
    EXPECT_TRUE(outFile.is_open());
    outFile << "Test content";
    outFile.close();
    
    std::ifstream inFile(testFile);
    EXPECT_TRUE(inFile.is_open());
    
    std::string content;
    std::getline(inFile, content);
    EXPECT_EQ(content, "Test content");
    
    inFile.close();
}
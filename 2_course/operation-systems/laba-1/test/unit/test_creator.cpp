#include "gtest/gtest.h"
#include "../../Creator/creator_utils.h"
#include "../../common/employee.h"
#include "../../common/file_utils.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class CreatorTest : public ::testing::Test {
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

TEST_F(CreatorTest, ValidateArguments) {
    char* validArgs[] = {(char*)"creator", (char*)"test.bin", (char*)"5"};
    EXPECT_TRUE(validateCreatorArguments(3, validArgs));
    
    char* invalidArgs[] = {(char*)"creator", (char*)"test.bin"};
    EXPECT_FALSE(validateCreatorArguments(2, invalidArgs));
}

TEST_F(CreatorTest, CreateAndReadBinaryFile) {
    std::vector<employee> testEmployees = {
        {1, "John", 40.0},
        {2, "Alice", 35.5},
        {3, "Bob", 42.0}
    };
    
    std::string testFile = (testDir / "test_employees.bin").string();
    
    EXPECT_TRUE(createEmployeeBinaryFile(testFile, testEmployees));
    
    auto readEmployees = readEmployeeBinaryFile(testFile);
    
    ASSERT_EQ(testEmployees.size(), readEmployees.size());
    
    for (size_t i = 0; i < testEmployees.size(); ++i) {
        EXPECT_EQ(testEmployees[i].num, readEmployees[i].num);
        EXPECT_STREQ(testEmployees[i].name, readEmployees[i].name);
        EXPECT_DOUBLE_EQ(testEmployees[i].hours, readEmployees[i].hours);
    }
}

TEST_F(CreatorTest, ReadNonExistentFile) {
    EXPECT_THROW(readEmployeeBinaryFile("nonexistent.bin"), std::runtime_error);
}
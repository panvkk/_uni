#include "gtest/gtest.h"
#include "../../Reporter/reporter_utils.h"
#include "../../common/employee.h"
#include "../../common/file_utils.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <regex>

namespace fs = std::filesystem;

class ReporterTest : public ::testing::Test {
protected:
    void SetUp() override {
        testDir = fs::temp_directory_path() / "ProcessLabTests";
        fs::create_directories(testDir);

        testEmployees = {
            {2, "Alice", 35.5},
            {1, "John", 40.0},
            {3, "Bob", 42.0}
        };

        binaryFile = (testDir / "test_employees.bin").string();
        createEmployeeBinaryFile(binaryFile, testEmployees);
    }

    void TearDown() override {
        fs::remove_all(testDir);
    }

    fs::path testDir;
    std::vector<employee> testEmployees;
    std::string binaryFile;

    // Вспомогательная функция для поиска чисел с плавающей точкой
    bool containsFloat(const std::string& str, double value, double tolerance = 0.01) {
        std::regex pattern("\\b\\d+\\.\\d{2}\\b");
        std::sregex_iterator it(str.begin(), str.end(), pattern);
        std::sregex_iterator end;

        while (it != end) {
            double foundValue = std::stod(it->str());
            if (std::abs(foundValue - value) < tolerance) {
                return true;
            }
            ++it;
        }
        return false;
    }
};

TEST_F(ReporterTest, ValidateArguments) {
    char* validArgs[] = {(char*)"reporter", (char*)"input.bin", (char*)"output.txt", (char*)"15.5"};
    EXPECT_TRUE(validateReporterArguments(4, validArgs));

    char* invalidArgs[] = {(char*)"reporter", (char*)"input.bin", (char*)"output.txt"};
    EXPECT_FALSE(validateReporterArguments(3, invalidArgs));
}

TEST_F(ReporterTest, GenerateReport) {
    std::string reportFile = (testDir / "test_report.txt").string();
    double hourlyRate = 15.5;

    EXPECT_TRUE(generateReport(binaryFile, reportFile, hourlyRate));

    std::ifstream inFile(reportFile);
    ASSERT_TRUE(inFile.is_open());

    std::string content((std::istreambuf_iterator<char>(inFile)),
                       std::istreambuf_iterator<char>());
    inFile.close();

    EXPECT_NE(content.find("1"), std::string::npos);
    EXPECT_NE(content.find("John"), std::string::npos);

    EXPECT_TRUE(containsFloat(content, 40.0));
    EXPECT_TRUE(containsFloat(content, 620.0));
}

TEST_F(ReporterTest, FormatReportLine) {
    employee emp = {1, "John", 40.0};
    double hourlyRate = 15.5;

    std::string line = formatReportLine(emp, hourlyRate);

    EXPECT_NE(line.find("1"), std::string::npos);
    EXPECT_NE(line.find("John"), std::string::npos);

    EXPECT_TRUE(containsFloat(line, 40.0)); // Часы
    EXPECT_TRUE(containsFloat(line, 620.0)); // Зарплата
}

TEST_F(ReporterTest, CreateReportHeader) {
    std::string header = createReportHeader("test.bin");
    EXPECT_NE(header.find("test.bin"), std::string::npos);
}
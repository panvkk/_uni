#include <catch2/catch_all.hpp>
#include <vector>
#include <numeric>
#include <algorithm>

int findMin(const std::vector<int>& arr) {
    return *std::min_element(arr.begin(), arr.end());
}

int findMax(const std::vector<int>& arr) {
    return *std::max_element(arr.begin(), arr.end());
}

double computeAverage(const std::vector<int>& arr) {
    if (arr.empty()) return 0.0;
    long long sum = std::accumulate(arr.begin(), arr.end(), 0LL);
    return static_cast<double>(sum) / arr.size();
}

void replaceMinMaxWithAverage(std::vector<int>& arr, double avg) {
    if (arr.empty()) return;
    int minVal = findMin(arr);
    int maxVal = findMax(arr);
    int avgInt = static_cast<int>(avg);

    for (int& v : arr) {
        if (v == minVal || v == maxVal) {
            v = avgInt;
        }
    }
}

TEST_CASE("Find min and max in array", "[minmax]") {
    std::vector<int> arr = {5, 2, 9, -1, 7};
    REQUIRE(findMin(arr) == -1);
    REQUIRE(findMax(arr) == 9);
}

TEST_CASE("Compute average", "[average]") {
    SECTION("Positive numbers") {
        std::vector<int> arr = {2, 4, 6, 8};
        REQUIRE(computeAverage(arr) == Catch::Approx(5.0));
    }
    SECTION("Mixed numbers") {
        std::vector<int> arr = {-2, 2};
        REQUIRE(computeAverage(arr) == Catch::Approx(0.0));
    }
    SECTION("Empty array") {
        std::vector<int> arr;
        REQUIRE(computeAverage(arr) == Catch::Approx(0.0));
    }
}

TEST_CASE("Replace min and max with average", "[replace]") {
    std::vector<int> arr = {1, 10, 5};
    double avg = computeAverage(arr);
    replaceMinMaxWithAverage(arr, avg);

    REQUIRE(arr[0] == 5);
    REQUIRE(arr[1] == 5);
    REQUIRE(arr[2] == 5);
}

TEST_CASE("Replace works with duplicates", "[replace]") {
    std::vector<int> arr = {3, 3, 7, 7, 5};
    double avg = computeAverage(arr);
    replaceMinMaxWithAverage(arr, avg);

    // Все 3 и 7 заменяются на 5
    for (int v : arr) {
        REQUIRE(v == 5);
    }
}

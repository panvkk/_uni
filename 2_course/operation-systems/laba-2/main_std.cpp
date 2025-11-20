#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <windows.h>

struct SharedData {
    std::vector<int> arr;
    int minVal;
    int maxVal;
    double average;
    std::mutex mtx;
};

void minMaxThread(SharedData* data) {
    if (data->arr.empty()) return;

    int localMin = data->arr[0];
    int localMax = data->arr[0];

    for (size_t i = 1; i < data->arr.size(); ++i) {
        if (data->arr[i] < localMin) localMin = data->arr[i];
        if (data->arr[i] > localMax) localMax = data->arr[i];
    }

    {
        std::lock_guard<std::mutex> lock(data->mtx);
        data->minVal = localMin;
        data->maxVal = localMax;
        std::cout << "[min_max] min = " << localMin
                  << ", max = " << localMax << std::endl;
    }
}

void averageThread(SharedData* data) {
    if (data->arr.empty()) return;

    long long sum = 0;
    for (int v : data->arr) {
        sum += v;
    }

    {
        std::lock_guard<std::mutex> lock(data->mtx);
        data->average = static_cast<double>(sum) / data->arr.size();
        std::cout << "[average] avg = " << data->average << std::endl;
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    SharedData data;

    std::cout << "Введите размер массива: ";
    int n;
    std::cin >> n;

    data.arr.resize(n);
    for (int i = 0; i < n; ++i) {
        std::cout << "arr[" << i << "] = ";
        std::cin >> data.arr[i];
    }

    std::thread t1(minMaxThread, &data);
    std::thread t2(averageThread, &data);

    t1.join();
    t2.join();

    if (!data.arr.empty()) {
        int idxMin = 0, idxMax = 0;
        for (size_t i = 0; i < data.arr.size(); ++i) {
            if (data.arr[i] < data.arr[idxMin]) idxMin = i;
            if (data.arr[i] > data.arr[idxMax]) idxMax = i;
        }
        int avgInt = static_cast<int>(data.average);
        data.arr[idxMin] = avgInt;
        data.arr[idxMax] = avgInt;
    }

    std::cout << "Результирующий массив: ";
    for (int v : data.arr) std::cout << v << " ";
    std::cout << std::endl;

    return 0;
}

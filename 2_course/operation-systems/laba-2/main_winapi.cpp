#include <windows.h>
#include <iostream>
#include <vector>

struct SharedData {
    std::vector<int> arr;
    int minVal;
    int maxVal;
    double average;
};

DWORD WINAPI MinMaxThread(LPVOID lpParam) {
    SharedData* data = static_cast<SharedData*>(lpParam);
    if (data->arr.empty()) return 0;

    int localMin = data->arr[0];
    int localMax = data->arr[0];

    for (size_t i = 1; i < data->arr.size(); ++i) {
        if (data->arr[i] < localMin) localMin = data->arr[i];
        if (data->arr[i] > localMax) localMax = data->arr[i];
        Sleep(7);
    }

    data->minVal = localMin;
    data->maxVal = localMax;

    std::cout << "[min_max] min = " << localMin
              << ", max = " << localMax << std::endl;
    return 0;
}

DWORD WINAPI AverageThread(LPVOID lpParam) {
    SharedData* data = static_cast<SharedData*>(lpParam);
    if (data->arr.empty()) return 0;

    long long sum = 0;
    for (int v : data->arr) {
        sum += v;
        Sleep(12);
    }

    data->average = static_cast<double>(sum) / data->arr.size();
    std::cout << "[average] avg = " << data->average << std::endl;
    return 0;
}

int main() {
    SharedData data;

    std::cout << "Введите размер массива: ";
    int n;
    std::cin >> n;

    data.arr.resize(n);
    for (int i = 0; i < n; ++i) {
        std::cout << "arr[" << i << "] = ";
        std::cin >> data.arr[i];
    }

    DWORD id1, id2;
    HANDLE hMinMax = CreateThread(nullptr, 0, MinMaxThread, &data, 0, &id1);
    HANDLE hAverage = CreateThread(nullptr, 0, AverageThread, &data, 0, &id2);

    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    CloseHandle(hMinMax);
    CloseHandle(hAverage);

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

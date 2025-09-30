#include <windows.h>
#include <iostream>
#include <vector>

struct SharedData {
    std::vector<int> arr;
};

DWORD WINAPI MinMaxThread(LPVOID lpParam) {
    SharedData* data = static_cast<SharedData*>(lpParam);

    std::cout << "[min_max thread] Запущен. Размер массива = "
              << data->arr.size() << std::endl;

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

    DWORD threadId;
    HANDLE hMinMax = CreateThread(
        nullptr,
        0,
        MinMaxThread,
        &data,
        0,
        &threadId
    );

    if (!hMinMax) {
        std::cerr << "Не удалось создать поток min_max" << std::endl;
        return 1;
    }

    WaitForSingleObject(hMinMax, INFINITE);

    CloseHandle(hMinMax);

    std::cout << "Главный поток завершает работу." << std::endl;
    return 0;
}

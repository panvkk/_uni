#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <numeric>
#include <atomic>
#include <algorithm>

std::mutex mtx;
std::condition_variable start_cv, main_cv, marker_cv;
bool start_gate = false;
std::atomic<int> stuck_markers_count(0);
std::vector<bool> terminate_flags;

std::vector<int> data_array;
int active_markers_count = 0;

void print_array() {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Состояние массива: [ ";
    for (int val : data_array) {
        std::cout << val << " ";
    }
    std::cout << "]" << std::endl;
}

void marker_thread_proc(int id) {
    {
        std::unique_lock<std::mutex> lock(mtx);
        start_cv.wait(lock, [] { return start_gate; });
    }

    srand(id);

    while (true) {
        int index = rand() % data_array.size();

        std::unique_lock<std::mutex> lock(mtx);

        if (data_array[index] == 0) {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(5));

            lock.lock();
            if (data_array[index] == 0) {
                data_array[index] = id;
            } else {
                continue;
            }
            lock.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(5));

        } else {
            int marked_count = 0;
            for (int val : data_array) {
                if (val == id) {
                    marked_count++;
                }
            }
            std::cout << "Поток " << id << ": не могу пометить элемент " << index
                      << ". Помечено " << marked_count << " элементов." << std::endl;

            stuck_markers_count++;
            main_cv.notify_one();

            marker_cv.wait(lock, [&]{ return terminate_flags[id-1]; });

            if (terminate_flags[id - 1]) {
                 std::cout << "Поток " << id << " получил сигнал на завершение и очищает свои метки." << std::endl;
                for (size_t i = 0; i < data_array.size(); ++i) {
                    if (data_array[i] == id) {
                        data_array[i] = 0;
                    }
                }
                return;
            }
        }
    }
}


int main() {
    setlocale(LC_ALL, "Russian");

    int array_size;
    std::cout << "Введите размер массива: ";
    std::cin >> array_size;
    data_array.resize(array_size);

    std::fill(data_array.begin(), data_array.end(), 0);

    std::cout << "Введите количество потоков marker: ";
    std::cin >> active_markers_count;

    std::vector<std::thread> threads;
    std::vector<int> thread_ids(active_markers_count);
    std::iota(thread_ids.begin(), thread_ids.end(), 1);

    terminate_flags.resize(active_markers_count, false);


    for (int i = 0; i < active_markers_count; ++i) {
        threads.emplace_back(marker_thread_proc, thread_ids[i]);
    }

    std::cout << "Потоки marker созданы и ожидают сигнала..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    {
        std::lock_guard<std::mutex> lock(mtx);
        start_gate = true;
    }
    start_cv.notify_all();

    while (!threads.empty()) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            main_cv.wait(lock, [&] { return stuck_markers_count == active_markers_count; });
        }

        std::cout << "\n--- Все активные потоки сообщили о невозможности работы. ---" << std::endl;

        print_array();

        int thread_to_terminate_id;
        while (true) {
            std::cout << "Введите номер потока для завершения (активные ID: ";
            for(int id : thread_ids) std::cout << id << " ";
            std::cout << "): ";
            std::cin >> thread_to_terminate_id;

            auto it = std::find(thread_ids.begin(), thread_ids.end(), thread_to_terminate_id);
            if (it != thread_ids.end()) {
                break;
            }
            std::cout << "Ошибка: неверный ID потока. Попробуйте снова." << std::endl;
        }


        {
            std::lock_guard<std::mutex> lock(mtx);
            std::fill(terminate_flags.begin(), terminate_flags.end(), true);
        }
        marker_cv.notify_all();

        auto id_it = std::find(thread_ids.begin(), thread_ids.end(), thread_to_terminate_id);
        size_t index_to_remove = std::distance(thread_ids.begin(), id_it);

        if (threads[index_to_remove].joinable()) {
            threads[index_to_remove].join();
        }

        std::cout << "Поток " << thread_to_terminate_id << " завершил свою работу." << std::endl;

        threads.erase(threads.begin() + index_to_remove);
        thread_ids.erase(id_it);
        active_markers_count--;

        print_array();

        if (threads.empty()) break;

        {
            std::lock_guard<std::mutex> lock(mtx);
            stuck_markers_count = 0;
            std::fill(terminate_flags.begin(), terminate_flags.end(), false);
        }
    }

    std::cout << "\nВсе потоки marker завершили свою работу. Программа завершена." << std::endl;
    return 0;
}
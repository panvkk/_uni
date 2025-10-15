#include "marker_logic.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include <vector>

std::vector<int> data_array;

std::mutex mtx;
std::condition_variable start_cv, main_cv, marker_cv;
bool start_gate = false;
std::atomic<int> stuck_markers_count(0);
std::vector<bool> terminate_flags;
std::vector<bool> continue_flags;
int active_markers_count = 0;

void print_array() {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Array state: [ ";
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

        if (terminate_flags[id]) {
            for (size_t i = 0; i < data_array.size(); ++i) {
                if (data_array[i] == id) {
                    data_array[i] = 0;
                }
            }
            return;
        }

        if (data_array[index] == 0) {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            lock.lock();

            if (terminate_flags[id]) {
                for (size_t i = 0; i < data_array.size(); ++i) {
                    if (data_array[i] == id) {
                        data_array[i] = 0;
                    }
                }
                return;
            }

            if (data_array[index] == 0) {
                data_array[index] = id;
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
            std::cout << "Thread " << id << ": cannot mark element " << index
                      << ". Marked elements: " << marked_count << "." << std::endl;

            stuck_markers_count++;
            main_cv.notify_one();

            marker_cv.wait(lock, [&] { return terminate_flags[id] || continue_flags[id]; });

            if (terminate_flags[id]) {
                for (size_t i = 0; i < data_array.size(); ++i) {
                    if (data_array[i] == id) {
                        data_array[i] = 0;
                    }
                }
                return;
            }
             if (continue_flags[id]) {
                continue_flags[id] = false;
            }
        }
    }
}
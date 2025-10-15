#include "marker_logic.h"
#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <atomic>

extern std::mutex mtx;
extern std::condition_variable start_cv, main_cv, marker_cv;
extern bool start_gate;
extern std::atomic<int> stuck_markers_count;
extern std::vector<bool> terminate_flags;
extern std::vector<bool> continue_flags;
extern int active_markers_count;

int main() {
    int array_size;
    std::cout << "Enter array size: ";
    std::cin >> array_size;
    data_array.assign(array_size, 0);

    std::cout << "Enter number of marker threads: ";
    std::cin >> active_markers_count;

    std::vector<std::thread> threads;
    std::vector<int> thread_ids(active_markers_count);
    std::iota(thread_ids.begin(), thread_ids.end(), 1);

    terminate_flags.assign(active_markers_count + 1, false);
    continue_flags.assign(active_markers_count + 1, false);

    for (int id : thread_ids) {
        threads.emplace_back(marker_thread_proc, id);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    {
        std::lock_guard<std::mutex> lock(mtx);
        start_gate = true;
    }
    start_cv.notify_all();

    while (active_markers_count > 0) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            main_cv.wait(lock, [&] { return stuck_markers_count == active_markers_count; });
        }

        std::cout << "\n--- All threads are waiting ---" << std::endl;
        print_array();

        int thread_to_terminate_id;
        std::cout << "Enter thread ID to terminate: ";
        std::cin >> thread_to_terminate_id;

        auto id_it = std::find(thread_ids.begin(), thread_ids.end(), thread_to_terminate_id);
        if (id_it == thread_ids.end()) {
            std::cout << "Invalid or already terminated thread ID." << std::endl;
            continue;
        }
        size_t index_to_remove = std::distance(thread_ids.begin(), id_it);

        {
            std::lock_guard<std::mutex> lock(mtx);
            terminate_flags[thread_to_terminate_id] = true;
        }
        marker_cv.notify_all();

        if (threads[index_to_remove].joinable()) {
            threads[index_to_remove].join();
        }

        std::cout << "Thread " << thread_to_terminate_id << " has terminated." << std::endl;

        threads.erase(threads.begin() + index_to_remove);
        thread_ids.erase(id_it);
        active_markers_count--;

        print_array();

        if (active_markers_count == 0) break;

        {
            std::lock_guard<std::mutex> lock(mtx);
            stuck_markers_count = 0;
            for(int id : thread_ids){
                continue_flags[id] = true;
            }
        }
        marker_cv.notify_all();
    }

    std::cout << "\nAll marker threads have finished. Exiting." << std::endl;
    return 0;
}
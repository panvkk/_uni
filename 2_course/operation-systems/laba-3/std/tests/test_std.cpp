#include "../src/marker_logic.h"
#include <cassert>
#include <thread>
#include <numeric>
#include <iostream>
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

void test_cleanup() {
    std::cout << "Running std::thread cleanup test..." << std::endl;

    data_array = {0, 1, 0, 1, 0};
    active_markers_count = 1;
    stuck_markers_count = 0;
    start_gate = false;
    terminate_flags.assign(2, false);
    continue_flags.assign(2, false);

    std::thread t(marker_thread_proc, 1);

    {
        std::lock_guard<std::mutex> lock(mtx);
        start_gate = true;
    }
    start_cv.notify_all();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    data_array[0] = 2;

    {
        std::unique_lock<std::mutex> lock(mtx);
        main_cv.wait(lock, []{ return stuck_markers_count == 1; });
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        terminate_flags[1] = true;
    }
    marker_cv.notify_one();

    t.join();

    bool cleanup_ok = true;
    for(int val : data_array) {
        if (val == 1) {
            cleanup_ok = false;
            break;
        }
    }
    assert(cleanup_ok && "Test failed: Thread 1 did not clean up its markers.");
    std::cout << "Test passed." << std::endl;
}


int main() {
    test_cleanup();
    return 0;
}
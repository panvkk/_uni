#include "marker_logic.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <atomic>

std::vector<int> data_array;

CRITICAL_SECTION g_cs;
CONDITION_VARIABLE g_start_cv, g_main_cv, g_marker_cv;
bool g_start_gate = false;
std::atomic<int> g_stuck_markers_count(0);
std::vector<bool> g_terminate_flags;
std::vector<bool> g_continue_flags;
int g_active_markers_count = 0;

void print_array() {
    EnterCriticalSection(&g_cs);
    std::cout << "Array state: [ ";
    for (int val : data_array) {
        std::cout << val << " ";
    }
    std::cout << "]" << std::endl;
    LeaveCriticalSection(&g_cs);
}

DWORD WINAPI marker_thread_proc_winapi(LPVOID lpParam) {
    int id = reinterpret_cast<intptr_t>(lpParam);

    EnterCriticalSection(&g_cs);
    while (!g_start_gate) {
        SleepConditionVariableCS(&g_start_cv, &g_cs, INFINITE);
    }
    LeaveCriticalSection(&g_cs);

    srand(id);

    while (true) {
        int index = rand() % data_array.size();
        EnterCriticalSection(&g_cs);

        if (data_array[index] == 0) {
            LeaveCriticalSection(&g_cs);
            Sleep(5);
            EnterCriticalSection(&g_cs);
            if (data_array[index] == 0) {
                data_array[index] = id;
            }
            LeaveCriticalSection(&g_cs);
            Sleep(5);
        } else {
            int marked_count = 0;
            for (int val : data_array) {
                if (val == id) {
                    marked_count++;
                }
            }
            std::cout << "Thread " << id << ": cannot mark element " << index
                      << ". Marked elements: " << marked_count << "." << std::endl;

            g_stuck_markers_count++;
            WakeConditionVariable(&g_main_cv);

            while (!g_terminate_flags[id - 1] && !g_continue_flags[id-1]) {
                SleepConditionVariableCS(&g_marker_cv, &g_cs, INFINITE);
            }

            if (g_terminate_flags[id - 1]) {
                for (size_t i = 0; i < data_array.size(); ++i) {
                    if (data_array[i] == id) {
                        data_array[i] = 0;
                    }
                }
                LeaveCriticalSection(&g_cs);
                return 0;
            }
            if (g_continue_flags[id-1]) {
                 g_continue_flags[id-1] = false;
            }
            LeaveCriticalSection(&g_cs);
        }
    }
    return 0;
}
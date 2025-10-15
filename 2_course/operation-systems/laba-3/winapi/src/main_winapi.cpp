#include "marker_logic.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <atomic>

extern CRITICAL_SECTION g_cs;
extern CONDITION_VARIABLE g_start_cv, g_main_cv, g_marker_cv;
extern bool g_start_gate;
extern std::atomic<int> g_stuck_markers_count;
extern std::vector<bool> g_terminate_flags;
extern std::vector<bool> g_continue_flags;
extern int g_active_markers_count;

DWORD WINAPI marker_thread_proc_winapi(LPVOID lpParam);

int main() {
    InitializeCriticalSection(&g_cs);
    InitializeConditionVariable(&g_start_cv);
    InitializeConditionVariable(&g_main_cv);
    InitializeConditionVariable(&g_marker_cv);

    int array_size;
    std::cout << "Enter array size: ";
    std::cin >> array_size;
    data_array.assign(array_size, 0);

    std::cout << "Enter number of marker threads: ";
    std::cin >> g_active_markers_count;

    std::vector<HANDLE> threads;
    std::vector<int> thread_ids(g_active_markers_count);
    std::iota(thread_ids.begin(), thread_ids.end(), 1);

    g_terminate_flags.assign(g_active_markers_count, false);
    g_continue_flags.assign(g_active_markers_count, false);

    for (int i = 0; i < g_active_markers_count; ++i) {
        threads.push_back(CreateThread(NULL, 0, marker_thread_proc_winapi, (LPVOID)(intptr_t)thread_ids[i], 0, NULL));
    }

    Sleep(1000);
    
    EnterCriticalSection(&g_cs);
    g_start_gate = true;
    LeaveCriticalSection(&g_cs);
    WakeAllConditionVariable(&g_start_cv);

    while (g_active_markers_count > 0) {
        EnterCriticalSection(&g_cs);
        while (g_stuck_markers_count < g_active_markers_count) {
            SleepConditionVariableCS(&g_main_cv, &g_cs, INFINITE);
        }
        LeaveCriticalSection(&g_cs);

        std::cout << "\n--- All threads are waiting ---" << std::endl;
        print_array();

        int thread_to_terminate_id;
        std::cout << "Enter thread ID to terminate: ";
        std::cin >> thread_to_terminate_id;

        auto id_it = std::find(thread_ids.begin(), thread_ids.end(), thread_to_terminate_id);
        if (id_it == thread_ids.end()) {
             std::cout << "Invalid thread ID." << std::endl;
             g_stuck_markers_count = 0;
             EnterCriticalSection(&g_cs);
             for(int id : thread_ids) g_continue_flags[id-1] = true;
             LeaveCriticalSection(&g_cs);
             WakeAllConditionVariable(&g_marker_cv);
             continue;
        }
        size_t index_to_remove = std::distance(thread_ids.begin(), id_it);
        
        EnterCriticalSection(&g_cs);
        g_terminate_flags[thread_to_terminate_id - 1] = true;
        LeaveCriticalSection(&g_cs);
        WakeAllConditionVariable(&g_marker_cv);

        WaitForSingleObject(threads[index_to_remove], INFINITE);
        CloseHandle(threads[index_to_remove]);

        std::cout << "Thread " << thread_to_terminate_id << " has terminated." << std::endl;

        threads.erase(threads.begin() + index_to_remove);
        thread_ids.erase(id_it);
        g_active_markers_count--;
        
        print_array();
        
        if (g_active_markers_count == 0) break;
        
        EnterCriticalSection(&g_cs);
        g_stuck_markers_count = 0;
        for(int id : thread_ids){
            g_continue_flags[id-1] = true;
        }
        LeaveCriticalSection(&g_cs);
        WakeAllConditionVariable(&g_marker_cv);
    }
    
    std::cout << "\nAll marker threads have finished. Exiting." << std::endl;
    DeleteCriticalSection(&g_cs);
    return 0;
}
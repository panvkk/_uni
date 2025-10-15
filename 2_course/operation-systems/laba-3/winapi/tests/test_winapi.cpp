#include "../src/marker_logic.h"
#include <windows.h>
#include <cassert>
#include <iostream>
#include <atomic>

extern CRITICAL_SECTION g_cs;
extern CONDITION_VARIABLE g_start_cv, g_main_cv, g_marker_cv;
extern bool g_start_gate;
extern std::atomic<int> g_stuck_markers_count;
extern std::vector<bool> g_terminate_flags;
extern std::vector<bool> g_continue_flags;
extern int g_active_markers_count;

DWORD WINAPI marker_thread_proc_winapi(LPVOID lpParam);

void test_cleanup_winapi() {
    std::cout << "Running WinAPI cleanup test..." << std::endl;

    InitializeCriticalSection(&g_cs);
    InitializeConditionVariable(&g_start_cv);
    InitializeConditionVariable(&g_main_cv);
    InitializeConditionVariable(&g_marker_cv);

    data_array = {0, 1, 0, 1, 0};
    g_active_markers_count = 1;
    g_stuck_markers_count = 0;
    g_start_gate = false;
    g_terminate_flags = {false};
    g_continue_flags = {false};

    HANDLE hThread = CreateThread(NULL, 0, marker_thread_proc_winapi, (LPVOID)1, 0, NULL);

    EnterCriticalSection(&g_cs);
    g_start_gate = true;
    LeaveCriticalSection(&g_cs);
    WakeAllConditionVariable(&g_start_cv);

    Sleep(100);
    data_array[0] = 2; 

    EnterCriticalSection(&g_cs);
    while (g_stuck_markers_count < 1) {
        SleepConditionVariableCS(&g_main_cv, &g_cs, INFINITE);
    }
    LeaveCriticalSection(&g_cs);

    EnterCriticalSection(&g_cs);
    g_terminate_flags[0] = true;
    LeaveCriticalSection(&g_cs);
    WakeConditionVariable(&g_marker_cv);

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    bool cleanup_ok = true;
    for(int val : data_array) {
        if (val == 1) {
            cleanup_ok = false;
            break;
        }
    }
    assert(cleanup_ok && "Test failed: Thread 1 did not clean up its markers.");
    std::cout << "Test passed." << std::endl;

    DeleteCriticalSection(&g_cs);
}

int main() {
    test_cleanup_winapi();
    return 0;
}
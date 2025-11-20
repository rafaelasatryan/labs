#include "pch.h"
#include "ThreadFuncs.h"
#include <iostream>
#include <windows.h>

CRITICAL_SECTION g_cs;
HANDLE g_mutex = nullptr;
volatile int g_turn = 0;

THREADFUNCS_API void InitializeSyncObjects() {
    InitializeCriticalSection(&g_cs);
    g_mutex = CreateMutex(nullptr, FALSE, nullptr);
    g_turn = 0;
}

THREADFUNCS_API void CleanupSyncObjects() {
    DeleteCriticalSection(&g_cs);
    if (g_mutex) CloseHandle(g_mutex);
    g_mutex = nullptr;
}

extern "C" THREADFUNCS_API DWORD WINAPI ThreadWholePhrase(LPVOID param) {
    auto* arg = static_cast<ThreadArg*>(param);
    for (int i = 0; i < REPEAT_COUNT; ++i) {
        std::cout << arg->phrase << "\n";
        Sleep(10);
    }
    return 0;
}

extern "C" THREADFUNCS_API DWORD WINAPI ThreadLettersUnsync(LPVOID param) {
    auto* arg = static_cast<ThreadArg*>(param);
    for (int r = 0; r < REPEAT_COUNT; ++r) {
        for (char c : arg->phrase) {
            std::cout << c << std::flush;
            Sleep(100);
        }
        std::cout << "\n";
    }
    return 0;
}

extern "C" THREADFUNCS_API DWORD WINAPI ThreadCriticalSection(LPVOID param) {
    auto* arg = static_cast<ThreadArg*>(param);
    for (int r = 0; r < REPEAT_COUNT; r++) {
        while (true) {
            EnterCriticalSection(&g_cs);
            if (g_turn == arg->id) {
                std::cout << arg->phrase << "\n";
                g_turn = 1 - arg->id;
                LeaveCriticalSection(&g_cs);
                break;
            }
            LeaveCriticalSection(&g_cs);
            Sleep(1);
        }
        Sleep(1);
    }
    return 0;
}

extern "C" THREADFUNCS_API DWORD WINAPI ThreadMutex(LPVOID param) {
    auto* arg = static_cast<ThreadArg*>(param);
    for (int r = 0; r < REPEAT_COUNT; r++) {
        while (true) {
            DWORD w = WaitForSingleObject(g_mutex, INFINITE);
            if (w != WAIT_OBJECT_0) return 1;
            if (g_turn == arg->id) {
                std::cout << arg->phrase << "\n";
                g_turn = 1 - arg->id;
                ReleaseMutex(g_mutex);
                break;
            }
            ReleaseMutex(g_mutex);
            Sleep(1);
        }
        Sleep(1);
    }
    return 0;
}

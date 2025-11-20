#pragma once
#include <windows.h>
#include <string>

#ifdef THREADFUNCS_EXPORTS
#define THREADFUNCS_API __declspec(dllexport)
#else
#define THREADFUNCS_API __declspec(dllimport)
#endif

constexpr int REPEAT_COUNT = 10;

struct ThreadArg {
    std::string phrase;
    int id = 0;
};

extern "C" {
    THREADFUNCS_API DWORD WINAPI ThreadWholePhrase(LPVOID param);
    THREADFUNCS_API DWORD WINAPI ThreadLettersUnsync(LPVOID param);
    THREADFUNCS_API DWORD WINAPI ThreadCriticalSection(LPVOID param);
    THREADFUNCS_API DWORD WINAPI ThreadMutex(LPVOID param);

    THREADFUNCS_API void InitializeSyncObjects();
    THREADFUNCS_API void CleanupSyncObjects();
}

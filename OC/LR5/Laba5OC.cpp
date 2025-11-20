#include <windows.h>
#include <iostream>
#include <string>
#include <cstdlib>

constexpr int REPEAT_COUNT = 10;


CRITICAL_SECTION g_cs;
HANDLE g_mutex = nullptr;
volatile int g_turn = 0;


[[noreturn]] void FatalWinError(const char* msg) {
    DWORD e = GetLastError();
    LPVOID buf = nullptr;

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, e, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&buf, 0, NULL
    );
    std::cerr << msg << " (Error " << e << "): " << (char*)buf << "\n";
    LocalFree(buf);
    ExitProcess(e);
}


struct ThreadArg {
    std::string phrase;
    int id = 0;
};


DWORD WINAPI ThreadWholePhrase(LPVOID param) {
    auto* arg = static_cast<ThreadArg*>(param);

    for (int i = 0; i < REPEAT_COUNT; ++i) {
        std::cout << arg->phrase << "\n";
        Sleep(10);
    }
    return 0;
}


DWORD WINAPI ThreadLettersUnsync(LPVOID param) {
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


DWORD WINAPI ThreadCriticalSection(LPVOID param) {
    auto* arg = static_cast<ThreadArg*>(param);

    for (int r = 0; r < REPEAT_COUNT; r++) {

        while (true) {
            EnterCriticalSection(&g_cs);

            if (g_turn == arg->id) {
                // Наш ход
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


DWORD WINAPI ThreadMutex(LPVOID param) {
    auto* arg = static_cast<ThreadArg*>(param);

    for (int r = 0; r < REPEAT_COUNT; r++) {

        while (true) {
            DWORD w = WaitForSingleObject(g_mutex, INFINITE);
            if (w != WAIT_OBJECT_0) FatalWinError("WaitForSingleObject(mutex)");

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


bool RunTwoThreads(LPTHREAD_START_ROUTINE routine, ThreadArg args[2]) {
    HANDLE th[2];

    for (int i = 0; i < 2; i++) {
        th[i] = CreateThread(nullptr, 0, routine, &args[i], 0, nullptr);
        if (!th[i]) {
            std::cerr << "CreateThread failed\n";
            return false;
        }
    }

    DWORD w = WaitForMultipleObjects(2, th, TRUE, INFINITE);
    if (w == WAIT_FAILED) FatalWinError("WaitForMultipleObjects");

    CloseHandle(th[0]);
    CloseHandle(th[1]);
    return true;
}


int main() {
    setlocale(LC_ALL, "RU");

    std::string A = "Поток А: Здравствуй мир!";
    std::string B = "Поток Б: Привет из Волгограда!";
    ThreadArg args[2] = { {A, 0}, {B, 1} };

    while (true) {
        std::cout << "\nВыберите режим работы программы:\n";
        std::cout << " 1 — два потока печатают полные фразы\n";
        std::cout << " 2 — печать по буквам с перемешиванием\n";
        std::cout << " 3 — критическая секция, фразы чередуются\n";
        std::cout << " 4 — мьютекс, фразы чередуются\n";
        std::cout << " 0 — выход\n";
        std::cout << "Введите номер режима (0-4): ";

        int mode = -1;
        while (true) {
            if (!(std::cin >> mode)) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Неверный ввод. Введите число от 0 до 4: ";
                continue;
            }

            if (mode >= 0 && mode <= 4) break;

            std::cout << "Неверный ввод. Введите число от 0 до 4: ";
        }

        if (mode == 0) {
            std::cout << "Выход из программы.\n";
            break;
        }

        std::cout << "\nВыбор: " << mode << "\n\n";

        switch (mode) {
        case 1:
            RunTwoThreads(ThreadWholePhrase, args);
            break;

        case 2:
            RunTwoThreads(ThreadLettersUnsync, args);
            break;

        case 3:
            InitializeCriticalSection(&g_cs);
            g_turn = 0;
            RunTwoThreads(ThreadCriticalSection, args);
            DeleteCriticalSection(&g_cs);
            break;

        case 4:
            g_mutex = CreateMutex(nullptr, FALSE, nullptr);
            if (!g_mutex) FatalWinError("CreateMutex");
            g_turn = 0;
            RunTwoThreads(ThreadMutex, args);
            CloseHandle(g_mutex);
            break;
        }
    }

    return 0;
}
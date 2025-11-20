#include <windows.h>
#include <iostream>
#include <string>

struct ThreadArg {
    std::string phrase;
    int id = 0;
};

using ThreadFunc = DWORD(WINAPI*)(LPVOID);

int main() {
    setlocale(LC_ALL, "RU");

    HMODULE hDll = LoadLibrary(L"Laba6.1OC_DLL.dll");
    if (!hDll) { std::cerr << "Не удалось загрузить DLL\n"; return 1; }

    auto InitializeSyncObjects = (void(*)())GetProcAddress(hDll, "InitializeSyncObjects");
    auto CleanupSyncObjects = (void(*)())GetProcAddress(hDll, "CleanupSyncObjects");
    auto ThreadWholePhrase = (ThreadFunc)GetProcAddress(hDll, "ThreadWholePhrase");
    auto ThreadLettersUnsync = (ThreadFunc)GetProcAddress(hDll, "ThreadLettersUnsync");
    auto ThreadCriticalSection = (ThreadFunc)GetProcAddress(hDll, "ThreadCriticalSection");
    auto ThreadMutex = (ThreadFunc)GetProcAddress(hDll, "ThreadMutex");

    if (!InitializeSyncObjects || !CleanupSyncObjects || !ThreadWholePhrase ||
        !ThreadLettersUnsync || !ThreadCriticalSection || !ThreadMutex) {
        std::cerr << "Не удалось получить адрес функции из DLL\n";
        FreeLibrary(hDll);
        return 1;
    }

    InitializeSyncObjects();

    std::string A = "Поток А: Здравствуй мир!";
    std::string B = "Поток Б: Привет из Волгограда!";
    ThreadArg args[2] = { {A, 0}, {B, 1} };

    auto RunTwoThreads = [](ThreadFunc routine, ThreadArg args[2]) {
        HANDLE th[2];
        for (int i = 0; i < 2; i++) {
            th[i] = CreateThread(nullptr, 0, routine, &args[i], 0, nullptr);
            if (!th[i]) { std::cerr << "Ошибка создания потока\n"; return false; }
        }
        WaitForMultipleObjects(2, th, TRUE, INFINITE);
        CloseHandle(th[0]);
        CloseHandle(th[1]);
        return true;
        };

    while (true) {
        std::cout << "\nВыберите режим работы программы:\n";
        std::cout << " 1 — два потока печатают полные фразы\n";
        std::cout << " 2 — печать по буквам с перемешиванием\n";
        std::cout << " 3 — критическая секция, фразы чередуются\n";
        std::cout << " 4 — мьютекс, фразы чередуются\n";
        std::cout << " 0 — выход\n";
        std::cout << "Введите номер режима (0-4): ";

        int mode;
        std::cin >> mode;
        if (mode == 0) break;

        switch (mode) {
        case 1: RunTwoThreads(ThreadWholePhrase, args); break;
        case 2: RunTwoThreads(ThreadLettersUnsync, args); break;
        case 3: RunTwoThreads(ThreadCriticalSection, args); break;
        case 4: RunTwoThreads(ThreadMutex, args); break;
        default: std::cout << "Неверный ввод\n"; break;
        }
    }

    CleanupSyncObjects();
    FreeLibrary(hDll);
    return 0;
}

#include <iostream>
#include <string>
#include <windows.h>
#include "ThreadFuncs.h"

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
    if (w == WAIT_FAILED) {
        std::cerr << GetLastError();
        return false;
    }
    CloseHandle(th[0]);
    CloseHandle(th[1]);
    return true;
}

int main() {
    setlocale(LC_ALL, "RU");

    InitializeSyncObjects(); // Инициализация DLL-синхронизации

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


        std::cout << "\nВыбор: " << mode << "\n\n";

        switch (mode) {
        case 0:
            std::cout << "Выход из программы.\n";
            return 0;

        case 1:
            RunTwoThreads(ThreadWholePhrase, args);
            break;

        case 2:
            RunTwoThreads(ThreadLettersUnsync, args);
            break;

        case 3:
            RunTwoThreads(ThreadCriticalSection, args);
            break;

        case 4:
            RunTwoThreads(ThreadMutex, args);
            break;
        }
    }

    CleanupSyncObjects(); // Очистка DLL-ресурсов
    return 0;
}

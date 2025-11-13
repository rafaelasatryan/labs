#include<Windows.h>
#include<iostream>
#include<conio.h>
#include<locale>

using namespace std;

int main() {
	setlocale(LC_ALL, "RU");

	cout << "Мониторинг памяти. Нажмите 'q' для выхода.\n\n";

	while (true) {
		if (_kbhit()) {
			char ch = _getch();
			if (ch == 'q' || ch == 'Q') break;
		}

		MEMORYSTATUSEX memStatus;
		memStatus.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memStatus);

		cout << "--------------------------\n";
		cout << "Загрузка памяти: " << memStatus.dwMemoryLoad << " %\n";
		cout << "Всего физической памяти: " << memStatus.ullTotalPhys / (1024 * 1024) << " MB\n";
		cout << "Доступно физической памяти: " << memStatus.ullAvailPhys / (1024 * 1024) << "MB\n";

		Sleep(1000);
	}
	cout << "\nПрограмма завершена\n";
}
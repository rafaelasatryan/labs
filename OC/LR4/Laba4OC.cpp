#include<Windows.h>
#include<iostream>
#include<conio.h>
#include<thread>
#include<chrono>
#include<locale>

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");

	//Калькулятор
	STARTUPINFO siCalc = { sizeof(siCalc) };
	PROCESS_INFORMATION piCalc;

	if (!CreateProcess(L"C:\\Windows\\System32\\calc.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &siCalc, &piCalc))
	{
		cerr << "Ошибка создания процесса калькулятора: " << GetLastError() << endl;
		return 1;
	}

	cout << "Процесс калькулятора создан. PID: " << piCalc.dwProcessId << endl;

	while (true) {
		cout << "Введите команду (s - приостановить, r - возобновить, t - завершить): ";
		char cmd = _getch();
		cout << cmd << endl;

		if (cmd == 's') {
			if (SuspendThread(piCalc.hThread) == (DWORD)-1) {
				cerr << "Не удалось приостановить поток. Ошибка: " << GetLastError() << endl;
			}
			else {
				cout << "Поток приостановлен\n";
			}
		}
		else if (cmd == 'r') {
			if (ResumeThread(piCalc.hThread) == (DWORD)-1) {
				cerr << "Не удалось возобновить поток. Ошибка: " << GetLastError() << endl;
			}
			else {
				cout << "Поток возобновлен\n";
			}
		}
		else if (cmd == 't') {
			if (!TerminateProcess(piCalc.hProcess, 0)) {
				cerr << "Не удалось завершить процесс. Ошибка: " << GetLastError() << endl;
			}
			else {
				cout << "Процесс завершен\n";
			}
			break;
		}
	}

	CloseHandle(piCalc.hThread);
	CloseHandle(piCalc.hProcess);


	//Блокнот
	STARTUPINFO siNotepad = { sizeof(siNotepad) };
	PROCESS_INFORMATION piNotepad;

	if (!CreateProcess(L"C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &siNotepad, &piNotepad))
	{
		cerr << "Ошибка создания процесса блокнота: " << GetLastError() << endl;
		return 1;
	}

	cout << "Процесс блокнота создан. PID: " << piNotepad.dwProcessId << endl;

	cout << "Ожидание 10 секунд перед запуском потока...\n";
	this_thread::sleep_for(chrono::seconds(10));

	if (ResumeThread(piNotepad.hThread) == (DWORD)-1) {
		cerr << "Не удалось возобновить поток блокнота. Ошибка: " << GetLastError() << endl;
	}
	else {
		cout << "Поток блокнота возобновлен\n";
	}

	int priority = GetThreadPriority(piNotepad.hThread);
	if (priority == THREAD_PRIORITY_ERROR_RETURN) {
		cerr << "Не удалось получить приоритет потока. Ошибка: " << GetLastError() << endl;
	}
	else {
		cout << "Приоритет первичного потока блокнота: " << priority << endl;
		cout << "Значение приоритета: ";
		switch (priority) {
		case THREAD_PRIORITY_LOWEST: std::cout << "THREAD_PRIORITY_LOWEST\n"; break;
		case THREAD_PRIORITY_BELOW_NORMAL: std::cout << "THREAD_PRIORITY_BELOW_NORMAL\n"; break;
		case THREAD_PRIORITY_NORMAL: std::cout << "THREAD_PRIORITY_NORMAL\n"; break;
		case THREAD_PRIORITY_ABOVE_NORMAL: std::cout << "THREAD_PRIORITY_ABOVE_NORMAL\n"; break;
		case THREAD_PRIORITY_HIGHEST: std::cout << "THREAD_PRIORITY_HIGHEST\n"; break;
		case THREAD_PRIORITY_TIME_CRITICAL: std::cout << "THREAD_PRIORITY_TIME_CRITICAL\n"; break;
		case THREAD_PRIORITY_IDLE: std::cout << "THREAD_PRIORITY_IDLE\n"; break;
		default: std::cout << "Неизвестный приоритет\n"; break;
		}
	}

	WaitForSingleObject(piNotepad.hProcess, INFINITE);
	CloseHandle(piNotepad.hThread);
	CloseHandle(piNotepad.hProcess);


}
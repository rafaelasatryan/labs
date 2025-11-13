#include<Windows.h>
#include<iostream>
#include<locale>

using namespace std;

int main() {
	setlocale(LC_ALL, "RU");

	char cwd[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, cwd);
	cout << cwd << endl;

	char sourceFile[MAX_PATH];
	char destFile[MAX_PATH];
	DWORD pos;
	cout << "Введите имя исходного файла: ";
	cin >> sourceFile;
	cout << "Введите имя нового файла: ";
	cin >> destFile;
	cout << "Введите номер символа с которого начать копирование: ";
	cin >> pos;

	HANDLE hSourse = CreateFileA(sourceFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSourse == INVALID_HANDLE_VALUE)
	{
		cout << "Ошибка: не удалось открыть исходный файл. Код ошибки: " << GetLastError() << endl;
		return 1;
	}

	HANDLE hDest = CreateFileA(destFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDest == INVALID_HANDLE_VALUE)
	{
		cout << "Ошибка: не удалось создать новый файл. Код ошибки: " << GetLastError() << endl;
		return 1;
	}

	DWORD newpos = SetFilePointer(hSourse, pos, NULL, FILE_BEGIN);

	if (newpos == INVALID_SET_FILE_POINTER)
	{
		cout << "Ошибка: неверная позиция в файле." << endl;
		CloseHandle(hSourse);
		CloseHandle(hDest);
		return 1;
	}

	char buffer[512];
	DWORD read = 0;
	DWORD written = 0;

	if (!ReadFile(hSourse, buffer, sizeof(buffer), &read, NULL))
	{
		cout << "Ошибка чтения файла. Код ошибки: " << GetLastError() << endl;
		CloseHandle(hSourse);
		CloseHandle(hDest);
		return 1;
	}

	if (!WriteFile(hDest, buffer, read, &written, NULL))
	{
		cout << "Ошибка записи файла. Код ошибки: " << GetLastError() << endl;
		CloseHandle(hSourse);
		CloseHandle(hDest);
		return 1;
	}

	CloseHandle(hSourse);
	CloseHandle(hDest);

	if (!DeleteFileA(sourceFile))
	{
		cout << "Ошибка: не удалось удалить исходный файл. Код ошибки: " << GetLastError() << endl;
		return 1;
	}

	cout << "Файл скопирован и исходный удалён." << endl;
}
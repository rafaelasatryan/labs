#include<Windows.h>
#include<iostream>
#include<string>
#include<locale>

using namespace std;

int main() {
	setlocale(LC_ALL, "RU");

	char drive;
	cout << "Введите букву диска: ";
	cin >> drive;

	drive = toupper(drive);
	string path = string(1, drive) + ":\\";

	DWORD dmask = GetLogicalDrives();
	if (dmask == 0) {
		cout << "Ошибка при вызове GetLogicalDrives()" << endl;
		return 1;
	}

	int bit = drive - 'A';
	if (!(dmask & (1 << bit))) {
		cout << "Диск " << drive << " не существует." << endl;
	}

	UINT type = GetDriveTypeA(path.c_str());
	cout << "Тип диска: ";
	switch (type) {
	case DRIVE_UNKNOWN: cout << "Неизвестный тип.\n"; break;
	case DRIVE_NO_ROOT_DIR: cout << "Неправильный путь.\n"; break;
	case DRIVE_REMOVABLE: cout << "Съёмный диск.\n"; break;
	case DRIVE_FIXED: cout << "Фиксированный диск.\n"; break;
	case DRIVE_REMOTE: cout << "Удалённый или network диск.\n"; break;
	case DRIVE_CDROM: cout << "CD-ROM диск.\n"; break;
	case DRIVE_RAMDISK: cout << "RAM диск.\n"; break;
	}

	

	char VolumeNameBuffer[MAX_PATH + 1];
	DWORD VolumeSerialNumber = 0;
	DWORD MaximumComponentLength = 0;
	DWORD FileSystemFlags = 0;
	char FileSystemNameBuffer[MAX_PATH + 1];
	
	if (GetVolumeInformationA(path.c_str(), VolumeNameBuffer, sizeof(VolumeNameBuffer), &VolumeSerialNumber, &MaximumComponentLength, &FileSystemFlags, FileSystemNameBuffer, sizeof(FileSystemNameBuffer)))
	{
		cout << "Название диска: " << VolumeNameBuffer << endl;
		cout << "Серийный номер: " << VolumeSerialNumber << endl;
		cout << "Файловая система: " << FileSystemNameBuffer << endl;
		cout << "Максимальная длина имени файла: " << MaximumComponentLength << endl;
	}
	else { cout << "Не удалось получить информацию о диске." << endl; }


	ULARGE_INTEGER FreebytesAvailable, TotalNumberOfBytes, TotalNumberOfFreeBytes;

	if (GetDiskFreeSpaceExA(path.c_str(), &FreebytesAvailable, &TotalNumberOfBytes, &TotalNumberOfFreeBytes))
	{
		cout << "Всего: " << (TotalNumberOfBytes.QuadPart >> 30) << "ГБ" << endl;
		cout << "Свободно: " << (TotalNumberOfFreeBytes.QuadPart >> 30) << "ГБ" << endl;
		cout << "Доступно пользователю: " << (FreebytesAvailable.QuadPart >> 30) << "ГБ" << endl;
	}
	else { cout << "Не удалось получить данные о свободном месте." << endl; }
}
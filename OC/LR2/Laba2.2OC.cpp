#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <locale>

using namespace std;

void PrintMemoryInfo(void* addr) {
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(addr, &mbi, sizeof(mbi))) {
        cout << "--------------------------------------\n";
        cout << "Адрес     : " << addr << endl;
        cout << "State     : 0x" << hex << mbi.State << dec << endl;
        cout << "Protect   : 0x" << hex << mbi.Protect << dec << endl;
        cout << "Type      : 0x" << hex << mbi.Type << dec << endl;
        cout << "--------------------------------------\n";
    }
    else {
        cerr << "Ошибка VirtualQuery: " << GetLastError() << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    SYSTEM_INFO si;
    GetSystemInfo(&si);
    SIZE_T pageSize = si.dwPageSize;
    cout << "Размер страницы: " << pageSize << " байт\n";

    SIZE_T totalPages;
    cout << "Введите количество страниц для резервирования: ";
    if (!(cin >> totalPages) || totalPages == 0) {
        cerr << "Некорректный ввод.\n";
        return 1;
    }

    SIZE_T reserveSize = totalPages * pageSize;
    void* baseAddress = VirtualAlloc(NULL, reserveSize, MEM_RESERVE, PAGE_READWRITE);
    if (!baseAddress) {
        cerr << "Ошибка резервирования памяти. Код: " << GetLastError() << endl;
        return 1;
    }

    cout << "Зарезервирована область по адресу: " << baseAddress << endl;

    SIZE_T commitPages, startPage;
    cout << "Введите количество страниц для commit: ";
    cin >> commitPages;
    cout << "Введите номер начальной страницы (1.." << totalPages << "): ";
    cin >> startPage;

    if (startPage < 1 || startPage + commitPages - 1 > totalPages) {
        cerr << "Некорректный диапазон commit страниц.\n";
        VirtualFree(baseAddress, 0, MEM_RELEASE);
        return 1;
    }

    void* commitAddress = (BYTE*)baseAddress + (startPage - 1) * pageSize;
    if (!VirtualAlloc(commitAddress, commitPages * pageSize, MEM_COMMIT, PAGE_READWRITE)) {
        cerr << "Ошибка commit памяти. Код: " << GetLastError() << endl;
        VirtualFree(baseAddress, 0, MEM_RELEASE);
        return 1;
    }

    cout << "Commit адрес: " << commitAddress << endl;

    int arr[5] = { 10, 20, 30, 40, 50 };
    if (commitPages * pageSize >= sizeof(arr))
        memcpy_s(commitAddress, commitPages * pageSize, arr, sizeof(arr));

    cout << "Содержимое записанной области: ";
    int* p = (int*)commitAddress;
    for (int i = 0; i < 5; i++) cout << p[i] << " ";
    cout << "\n";

    cout << "\nИнформация о выделенной области:\n";
    PrintMemoryInfo(commitAddress);

    void* nextPage = (BYTE*)commitAddress + commitPages * pageSize;
    cout << "Информация об области после выделенной:\n";
    PrintMemoryInfo(nextPage);

    if (!VirtualAlloc(nextPage, pageSize, MEM_COMMIT, PAGE_READONLY)) {
        cerr << "Ошибка commit PAGE_READONLY. Код: " << GetLastError() << endl;
    }
    else {
        cout << "Выделена еще 1 страница по адресу: " << nextPage << endl;
        PrintMemoryInfo(nextPage);
    }

    SIZE_T decommitPages, decommitStart;
    cout << "\nВведите количество страниц для decommit: ";
    cin >> decommitPages;
    cout << "Введите номер начальной страницы: ";
    cin >> decommitStart;

    if (decommitStart < 1 || decommitStart + decommitPages - 1 > totalPages) {
        cerr << "Некорректный диапазон decommit.\n";
    }
    else {
        void* decommitAddr = (BYTE*)baseAddress + (decommitStart - 1) * pageSize;
        if (VirtualFree(decommitAddr, decommitPages * pageSize, MEM_DECOMMIT)) {
            cout << "Decommit успешно. Адрес: " << decommitAddr << endl;
            PrintMemoryInfo(decommitAddr);
        }
        else {
            cerr << "Ошибка VirtualFree(DECOMMIT): " << GetLastError() << endl;
        }
    }

    if (!VirtualFree(baseAddress, 0, MEM_RELEASE)) {
        cerr << "Ошибка полного освобождения памяти. Код: " << GetLastError() << endl;
    }
    else {
        cout << "Вся зарезервированная память успешно освобождена.\n";
    }

    return 0;
}

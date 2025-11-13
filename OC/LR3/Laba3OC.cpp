#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <ctime>
#include <limits>  // для проверки и очистки потока ввода

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    srand((unsigned)time(NULL));


    HANDLE hHeap = HeapCreate(0, 0, 0);
    if (!hHeap) {
        cerr << "Ошибка создания кучи. Код ошибки: " << GetLastError() << endl;
        return 1;
    }
    cout << "Куча успешно создана.\n";


    int rows;
    cout << "Введите количество строк массива (не менее 5): ";

    while (true) {
        cin >> rows;
        if (cin.fail() || rows < 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Некорректный ввод. Введите целое число >= 5: ";
        }
        else {
            break;
        }
    }


    int** arr = new int* [rows];
    SIZE_T* lengths = new SIZE_T[rows];


    for (int i = 0; i < rows; i++) {
        lengths[i] = rand() % 10 + 1;

        arr[i] = (int*)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, lengths[i] * sizeof(int));
        if (!arr[i]) {
            cerr << "Ошибка HeapAlloc для строки " << i << ". Код: " << GetLastError() << endl;

            for (int j = 0; j < i; j++) HeapFree(hHeap, 0, arr[j]);
            delete[] arr;
            delete[] lengths;
            HeapDestroy(hHeap);
            return 1;
        }

        for (size_t j = 0; j < lengths[i]; j++) {
            arr[i][j] = rand() % 10 + 1;
        }
    }


    cout << "\nИсходный массив:\n";
    for (int i = 0; i < rows; i++) {
        for (size_t j = 0; j < lengths[i]; j++) cout << arr[i][j] << " ";
        cout << endl;
    }


    int idx = rand() % rows;
    SIZE_T newLen = rand() % 10 + 1;
    cout << "\nИзменяем длину строки " << idx << " на " << newLen << " элементов.\n";
    int* newPtr = (int*)HeapReAlloc(hHeap, HEAP_ZERO_MEMORY, arr[idx], newLen * sizeof(int));

    if (newPtr) {
        arr[idx] = newPtr;
        lengths[idx] = newLen;
    }
    else {
        cerr << "Ошибка HeapReAlloc для строки " << idx << ". Код: " << GetLastError() << endl;
    }


    for (int i = 0; i < rows - 1; i++) {
        for (int j = i + 1; j < rows; j++) {
            if (lengths[i] > lengths[j]) {
                swap(lengths[i], lengths[j]);
                swap(arr[i], arr[j]);
            }
        }
    }


    cout << "\nПосле сортировки по длине строк:\n";
    for (int i = 0; i < rows; i++) {
        for (size_t j = 0; j < lengths[i]; j++) cout << arr[i][j] << " ";
        cout << endl;
    }


    cout << "\nПроверка HeapSize:\n";
    for (int i = 0; i < rows; i++) {
        SIZE_T size = HeapSize(hHeap, 0, arr[i]);
        if (size == (SIZE_T)-1) {
            cerr << "Ошибка HeapSize для строки " << i << ". Код: " << GetLastError() << endl;
        }
        else {
            cout << "Строка " << i << ": реально " << size / sizeof(int) << " элементов\n";
        }
    }


    for (int i = 0; i < rows; i++) {
        if (!HeapFree(hHeap, 0, arr[i])) {
            cerr << "Ошибка освобождения строки " << i << ". Код: " << GetLastError() << endl;
        }
    }

    delete[] arr;
    delete[] lengths;


    if (!HeapDestroy(hHeap)) {
        cerr << "Ошибка удаления кучи. Код: " << GetLastError() << endl;
        return 1;
    }

    cout << "\nКуча успешно удалена. Работа завершена.\n";
    return 0;
}

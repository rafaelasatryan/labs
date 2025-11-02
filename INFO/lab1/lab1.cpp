#include "DynamicVector.h"
#include "Point2D.h"
#include<iostream>

/*
int main() {
	DynamicVector<Point2D> v1;
	
	v1.push_back(Point2D(10, 20));
	v1.push_back(Point2D(5, 8));

	cout << "v1: " << v1 << "\n";
	cout << "Size: " << v1.size() << "\n";
	cout << "Erase index 0\n";

	v1.erase(0);
	cout << "v1: " << v1 << "\n";

	cout << "Push list...\n";
	v1.push_list(Point2D(200, 0), Point2D(40, 30), Point2D(3, 4));
	cout << "After push_list: " << v1 << "\n";

	int idx = v1.find(Point2D(4, 3));
	cout << "Find element abs=5 => index = " << idx << "\n";

	DynamicVector<Point2D> v2;
	v2.push_back(Point2D(1, 1));
	v2.push_back(Point2D(2, 2));
	v2.push_back(Point2D(3, 3));

	cout << "v2: " << v2 << "\n";
	cout << "v1 + v2 = " << (v1 + v2) << "\n";
	cout << "v1 * v2 = " << (v1 * v2) << "\n";
}

*/



using namespace std;

Point2D input_point() {
 double x, y;
 cout << "Введите X: ";
 cin >> x;
 cout << "Введите Y: ";
 cin >> y;
 return Point2D(x, y);
}

void fill_vector(DynamicVector<Point2D>& V) {
 int n;
 cout << "Введите количество элементов: ";
 cin >> n;

 for (int i = 0; i < n; i++) {
 cout << "Элемент #" << i << "\n";
 V.push_back(input_point());
 }
}

int main() {
 setlocale(LC_ALL, "Russian");

 DynamicVector<Point2D> V1;
 cout << "Создаём вектор V1:\n";
 fill_vector(V1);

 int choice;

 do {
 cout << "\n----- МЕНЮ -----\n";
 cout << "1. Показать вектор\n";
 cout << "2. Добавить элемент\n";
 cout << "3. Удалить по индексу\n";
 cout << "4. Найти элемент (по модулю)\n";
 cout << "5. Изменить размер\n";
 cout << "6. Сложить с другим вектором\n";
 cout << "7. Умножить с другим вектором\n";
 cout << "0. Выход\n";
 cout << "Выбор: ";
 cin >> choice;

 switch (choice) {
 case 1:
 cout << "V1 = " << V1 << "\n";
 break;

 case 2:
 cout << "Введите точку:\n";
 V1.push_back(input_point());
 break;

 case 3: {
 int idx;
 cout << "Введите индекс: ";
 cin >> idx;
 V1.erase(idx);
 break;
 }

 case 4: {
 cout << "Введите точку для поиска:\n";
 int index = V1.find(input_point());
 cout << "Индекс: " << index << "\n";
 break;
 }

 case 5: {
 int new_size;
 cout << "Новый размер: ";
 cin >> new_size;
 V1.resize(new_size);
 break;
 }

 case 6: {
 DynamicVector<Point2D> V2;
 cout << "Создаём второй вектор:\n";
 fill_vector(V2);
 cout << "Результат: " << (V1 + V2) << "\n";
 break;
 }

 case 7: {
 DynamicVector<Point2D> V2;
 cout << "Создаём второй вектор:\n";
 fill_vector(V2);
 cout << "Результат: " << (V1 * V2) << "\n";
 break;
 }

 case 0:
 cout << "Работа завершена.\n";
 break;

 default:
 cout << "Неверный выбор!\n";
 break;
 }

 } while (choice != 0);

 return 0;
}



#include "DynamicVector.h"
#include "Point2D.h"

int main() {
    setlocale(LC_ALL, "RU");

    DynamicVector<Point2D> v1;

    v1.push_back(Point2D(10, 20));
    v1.push_back(Point2D(5, 8));

    cout << "v1: " << v1 << "\n";
    cout << "Размер: " << v1.size() << "\n";
    cout << "Удаляем элемент с индексом 0\n";

    v1.erase(0);
    cout << "v1 после удаления: " << v1 << "\n";

    cout << "Добавляем элементы (push_list)...\n";
    v1.push_list(Point2D(200, 0), Point2D(40, 30), Point2D(3, 4));
    cout << "После push_list: " << v1 << "\n";

    int idx = v1.find(Point2D(4, 3));
    cout << "Поиск элемента с модулем 5 → индекс = " << idx << "\n";

    DynamicVector<Point2D> v2;
    v2.push_back(Point2D(1, 1));
    v2.push_back(Point2D(2, 2));
    v2.push_back(Point2D(3, 3));

    cout << "v2: " << v2 << "\n";
    cout << "v1 + v2 = " << (v1 + v2) << "\n";
    cout << "v1 * v2 = " << (v1 * v2) << "\n";
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>

int main() {
    setlocale(LC_ALL, "RU");
    srand(static_cast<unsigned int>(time(nullptr)));

    size_t N;
    double a, b;
    std::cout << "Введите размер вектора N: ";
    std::cin >> N;
    std::cout << "Введите интервал [a, b] для удаления: ";
    std::cin >> a >> b;

    std::vector<double> vec(N);

    // Заполнение случайными числами
    for (size_t i = 0; i < N; ++i) {
        vec[i] = static_cast<double>(rand() % 1000) / 10.0; // 0.0 - 99.9
    }

    std::cout << "Исходный вектор:\n";
    for (double v : vec) std::cout << v << " ";
    std::cout << "\n";

    // Попарная перестановка
    for (size_t i = 0; i + 1 < N; i += 2) {
        std::swap(vec[i], vec[i + 1]);
    }

    // Поиск первого минимального и последнего максимального
    auto min_it = std::min_element(vec.begin(), vec.end());
    auto max_it = std::max_element(vec.begin(), vec.end());
    auto first_min_index = std::distance(vec.begin(), min_it);
    auto last_max_index = std::distance(vec.begin(), max_it);

    if (first_min_index > last_max_index)
        std::swap(first_min_index, last_max_index);

    double sum_between = 0;
    for (size_t i = first_min_index + 1; i < last_max_index; ++i) {
        sum_between += vec[i];
    }
    std::cout << "Сумма элементов между первым минимумом и последним максимумом: " << sum_between << "\n";

    // Удаление элементов с модулем в [a, b]
    for (double& v : vec) {
        if (std::fabs(v) >= a && std::fabs(v) <= b) v = 0.0;
    }

    // Сортировка части без конечных нулей
    auto non_zero_end = std::find(vec.begin(), vec.end(), 0.0);
    std::sort(vec.begin(), non_zero_end);

    std::cout << "Результирующий вектор:\n";
    for (double v : vec) std::cout << v << " ";
    std::cout << "\n";

    // Демонстрация size() и capacity(), resize() и reserve()
    std::cout << "Размер вектора (size): " << vec.size() << "\n";
    std::cout << "Ёмкость вектора (capacity): " << vec.capacity() << "\n";

    vec.resize(vec.size() + 5, 1.0); // добавим 5 элементов со значением 1
    std::cout << "После resize(+5): size = " << vec.size() << ", capacity = " << vec.capacity() << "\n";

    vec.reserve(50); // резервируем память для 50 элементов
    std::cout << "После reserve(50): size = " << vec.size() << ", capacity = " << vec.capacity() << "\n";

    return 0;
}

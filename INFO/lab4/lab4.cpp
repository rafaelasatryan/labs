#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <random>

// Структура Date с перегрузкой операторов
struct Date {
    int day;
    int month;
    int year;

    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }

    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }

    bool operator>(const Date& other) const { return other < *this; }
    bool operator<=(const Date& other) const { return !(*this > other); }
    bool operator>=(const Date& other) const { return !(*this < other); }
};

// Перегрузка вывода в консоль
std::ostream& operator<<(std::ostream& os, const Date& d) {
    os << d.day << "." << d.month << "." << d.year;
    return os;
}

// Функция для печати диапазона элементов (поддерживает ленивые views)
template <typename Range>
void print(const Range& elements) {
    int count = 0;
    for (auto it = std::begin(elements); it != std::end(elements); ++it)
        ++count;

    std::cout << "Число элементов: " << count << " \n";
    for (const auto& el : elements)
        std::cout << el << "  ";
    std::cout << "\n";
}

// Генерация случайной даты
Date randomDate(int day_min, int day_max, int month_min, int month_max, int year_min, int year_max) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> day_dist(day_min, day_max);
    std::uniform_int_distribution<int> month_dist(month_min, month_max);
    std::uniform_int_distribution<int> year_dist(year_min, year_max);
    return { day_dist(gen), month_dist(gen), year_dist(gen) };
}

// Генерация вектора дат
std::vector<Date> generateVector(int n) {
    int day_min, day_max, month_min, month_max, year_min, year_max;
    std::cout << "Введите диапазон дней: ";
    std::cin >> day_min >> day_max;
    std::cout << "Введите диапазон месяцев: ";
    std::cin >> month_min >> month_max;
    std::cout << "Введите диапазон годов: ";
    std::cin >> year_min >> year_max;

    std::vector<Date> v(n);
    for (int i = 0; i < n; ++i)
        v[i] = randomDate(day_min, day_max, month_min, month_max, year_min, year_max);
    return v;
}

int main() {
    setlocale(LC_ALL, "RU");

    int n1, n2;
    std::cout << "Введите размер первого вектора: ";
    std::cin >> n1;
    std::cout << "Введите размер второго вектора: ";
    std::cin >> n2;

    auto vec1 = generateVector(n1);
    auto vec2 = generateVector(n2);

    std::cout << "Первый вектор:\n"; print(vec1);
    std::cout << "Второй вектор:\n"; print(vec2);

    // 2. Найти во втором векторе максимальный элемент
    auto max2 = *std::ranges::max_element(vec2, std::ranges::less{});
    std::cout << "Максимальная дата второго вектора: " << max2 << "\n";

    // Удалить из первого все элементы, которые больше max2
    vec1.erase(std::remove_if(vec1.begin(), vec1.end(), [&](const Date& d) { return d > max2; }), vec1.end());
    std::cout << "После удаления из первого вектора элементов > max второго:\n";
    print(vec1);

    // 7(1) - количество элементов первого вектора > пользовательской даты
    Date userDate;
    std::cout << "Введите дату для сравнения (день месяц год): ";
    std::cin >> userDate.day >> userDate.month >> userDate.year;

    int count_gt = std::count_if(vec1.begin(), vec1.end(), [&](const Date& d) { return d > userDate; });
    std::cout << "Количество дат > введенной: " << count_gt << "\n";

    // 9(2) - сдвинуть второй вектор так, чтобы выбранный элемент стал первым
    int pos;
    std::cout << "Введите номер элемента второго вектора, который должен стать первым: ";
    std::cin >> pos;
    std::rotate(vec2.begin(), vec2.begin() + pos, vec2.end());
    std::cout << "Второй вектор после сдвига:\n"; print(vec2);

    // 12(1) - частичная сортировка первой трети первого вектора
    auto one_third = vec1.begin() + vec1.size() / 3;
    std::partial_sort(vec1.begin(), one_third, vec1.end());
    std::cout << "Первый вектор после partial_sort первой трети:\n"; print(vec1);

    // 10(1) и 10(2) - полная сортировка обоих векторов
    std::sort(vec1.begin(), vec1.end());
    std::sort(vec2.begin(), vec2.end());
    std::cout << "Первый вектор после полной сортировки:\n"; print(vec1);
    std::cout << "Второй вектор после полной сортировки:\n"; print(vec2);

    // 14 - объединение двух векторов
    std::vector<Date> vec3;
    vec3.reserve(vec1.size() + vec2.size());
    std::ranges::copy(vec1, std::back_inserter(vec3));
    std::ranges::copy(vec2, std::back_inserter(vec3));
    std::cout << "Третий вектор (объединение):\n"; print(vec3);

    // 2-е задание через views
    auto result_view = vec3
        | std::views::filter([](const Date& d) { return d.month >= 9 && d.month <= 11; }) // осенние
        | std::views::transform([](Date d) { d.day += 1; return d; }) // прибавить 1 день
        | std::views::drop(1) // выбросить первую
        | std::views::reverse
        | std::views::take(5);

    // Преобразуем view в вектор через back_inserter
    std::vector<Date> result_vec;
    std::ranges::copy(result_view, std::back_inserter(result_vec));
    std::cout << "Результат после представлений:\n";
    print(result_vec);


    return 0;
}

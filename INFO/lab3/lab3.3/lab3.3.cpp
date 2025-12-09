#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

struct Weather {
    std::string date;
    double temperature;
    double humidity;
};

int main() {
    std::ifstream file("weather_classification_data.csv");
    std::vector<Weather> data(100); // первые 100 записей

    std::string line;
    size_t index = 0;

    // Пропускаем заголовок
    std::getline(file, line);

    // Читаем первые 100 записей
    while (index < 100 && std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;
        std::getline(ss, data[index].date, ',');
        std::getline(ss, field, ','); data[index].temperature = std::stod(field);
        std::getline(ss, field, ','); data[index].humidity = std::stod(field);
        index++;
    }

    // Обработка следующих блоков по 100
    while (std::getline(file, line)) {
        std::vector<Weather> block(100);
        block[0] = Weather{};
        size_t block_index = 0;
        do {
            std::stringstream ss(line);
            std::string field;
            std::getline(ss, block[block_index].date, ',');
            std::getline(ss, field, ','); block[block_index].temperature = std::stod(field);
            std::getline(ss, field, ','); block[block_index].humidity = std::stod(field);
            block_index++;
        } while (block_index < 100 && std::getline(file, line));

        // Сравнение с первым блоком по минимальной температуре
        for (size_t i = 0; i < block_index; ++i) {
            if (block[i].temperature < data[i].temperature) {
                data[i].temperature = block[i].temperature;
                data[i].date = block[i].date;
                data[i].humidity = block[i].humidity;
            }
        }
    }

    // Вывод 100 записей с минимальной температурой
    for (const auto& w : data) {
        std::cout << w.date << " " << w.temperature << " " << w.humidity << "\n";
    }

    return 0;
}

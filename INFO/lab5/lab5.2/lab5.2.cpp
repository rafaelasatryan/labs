#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <limits>
#include <cctype>
#include <locale>
#include <iomanip>


struct Key {
    std::string country;
    int year;

    bool operator==(const Key& o) const {
        return year == o.year && country == o.country;
    }
};


struct Value {
    std::string continent = "Неизвестно";
    double coal = 0, gas = 0, oil = 0;
    double gdp = 0;
    long long population = 0;

    friend std::ostream& operator<<(std::ostream& os, const Value& v) {
        os << "Часть света=" << v.continent
            << ", Уголь=" << v.coal
            << ", Газ=" << v.gas
            << ", Нефть=" << v.oil
            << ", ВВП=" << v.gdp
            << ", Население=" << v.population;
        return os;
    }
};


static inline void hash_combine(std::size_t& seed, std::size_t v) {
    seed ^= v + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
}

struct KeyHash {
    std::size_t operator()(const Key& k) const noexcept {
        std::size_t seed = 0;
        hash_combine(seed, std::hash<std::string>{}(k.country));
        hash_combine(seed, std::hash<int>{}(k.year));
        return seed;
    }
};


std::unordered_map<Key, Value, KeyHash> data;


static inline std::string trim(std::string s) {
    auto issp = [](unsigned char c) { return std::isspace(c); };
    while (!s.empty() && issp((unsigned char)s.front())) s.erase(s.begin());
    while (!s.empty() && issp((unsigned char)s.back())) s.pop_back();
    return s;
}

double safe_stod(const std::string& s) {
    std::string t = trim(s);
    if (t.empty()) return 0.0;
    try { return std::stod(t); }
    catch (...) { return 0.0; }
}


std::vector<std::string> splitCSV(const std::string& line) {
    std::vector<std::string> f;
    std::string cur;
    bool quotes = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            if (quotes && i + 1 < line.size() && line[i + 1] == '"') {
                cur.push_back('"'); ++i;
            }
            else quotes = !quotes;
        }
        else if (c == ',' && !quotes) {
            f.push_back(trim(cur));
            cur.clear();
        }
        else cur.push_back(c);
    }
    f.push_back(trim(cur));
    return f;
}


void readGDP(const std::string& fileName) {
    std::ifstream in(fileName);
    if (!in) {
        std::cerr << "Ошибка открытия файла " << fileName << "\n";
        return;
    }

    std::string line;
    std::getline(in, line);

    while (std::getline(in, line)) {
        if (trim(line).empty()) continue;
        auto f = splitCSV(line);
        if (f.size() < 5) continue;

        int year = (int)safe_stod(f[0]);
        std::string country = f[2];
        std::string continent = f[3];
        double gdp = safe_stod(f[4]);

        Value& v = data[Key{ country, year }];
        if (!continent.empty()) v.continent = continent;
        v.gdp = gdp;
    }
}


void readEmissions(const std::string& fileName) {
    std::ifstream in(fileName);
    if (!in) {
        std::cerr << "Ошибка открытия файла " << fileName << "\n";
        return;
    }

    std::string line;
    std::getline(in, line);

    while (std::getline(in, line)) {
        if (trim(line).empty()) continue;
        auto f = splitCSV(line);
        if (f.size() < 7) continue;

        std::string country = f[0];
        int year = (int)safe_stod(f[2]);

        Value& v = data[Key{ country, year }];
        v.coal = safe_stod(f[4]);
        v.oil = safe_stod(f[5]);
        v.gas = safe_stod(f[6]);
    }
}


void readPopulationWide(const std::string& fileName) {
    std::ifstream in(fileName);
    if (!in) {
        std::cerr << "Ошибка открытия файла " << fileName << "\n";
        return;
    }

    std::string header;
    std::getline(in, header);
    auto h = splitCSV(header);

    std::vector<int> years(h.size(), -1);
    for (size_t i = 1; i < h.size(); ++i)
        years[i] = (int)safe_stod(h[i]);

    std::string line;
    while (std::getline(in, line)) {
        if (trim(line).empty()) continue;
        auto f = splitCSV(line);
        if (f.size() < 2) continue;

        std::string country = f[0];
        for (size_t i = 1; i < f.size() && i < years.size(); ++i) {
            int year = years[i];
            if (year <= 0) continue;

            long long pop = (long long)safe_stod(f[i]);
            if (pop <= 0) continue;

            data[Key{ country, year }].population = pop;
        }
    }
}


void printLast50() {
    std::vector<std::pair<Key, Value>> first;
    first.reserve(200);

    int cnt = 0;
    for (const auto& kv : data) {
        if (cnt++ >= 200) break;
        first.push_back(kv);
    }

    int start = std::max(0, (int)first.size() - 50);

    std::cout << "\n----------------------------------------------------------------------------------------------------------------\n";
    std::cout << std::left
        << std::setw(28) << "Страна"
        << std::setw(14) << "Часть света"
        << std::setw(6) << "Год"
        << std::setw(12) << "Уголь"
        << std::setw(12) << "Газ"
        << std::setw(12) << "Нефть"
        << std::setw(18) << "ВВП"
        << std::setw(18) << "Население"
        << "\n";
    std::cout << "----------------------------------------------------------------------------------------------------------------\n";

    for (size_t i = start; i < first.size(); ++i) {
        const Key& k = first[i].first;
        const Value& v = first[i].second;

        std::cout << std::left
            << std::setw(28) << k.country.substr(0, 27)
            << std::setw(14) << v.continent.substr(0, 13)
            << std::setw(6) << k.year
            << std::setw(12) << std::fixed << std::setprecision(3) << v.coal
            << std::setw(12) << v.gas
            << std::setw(12) << v.oil
            << std::setw(18) << v.gdp
            << std::setw(18) << v.population
            << "\n";
    }
}


void query(int year, const std::string& continent) {
    std::string bestGdpPc, bestEm;
    double maxGdpPc = -1, maxEm = -1;
    std::vector<std::string> gasDominant;

    for (const auto& kv : data) {
        const Key& k = kv.first;
        const Value& v = kv.second;

        if (k.year != year || v.continent != continent) continue;

        if (v.population > 0) {
            double gdpPc = v.gdp / v.population;
            if (gdpPc > maxGdpPc) {
                maxGdpPc = gdpPc;
                bestGdpPc = k.country;
            }
        }

        double sumEm = v.coal + v.gas + v.oil;
        if (sumEm > maxEm) {
            maxEm = sumEm;
            bestEm = k.country;
        }

        if (v.gas > v.coal && v.gas > v.oil)
            gasDominant.push_back(k.country);
    }

    std::sort(gasDominant.begin(), gasDominant.end());
    gasDominant.erase(std::unique(gasDominant.begin(), gasDominant.end()), gasDominant.end());

    std::cout << "\n================ РЕЗУЛЬТАТЫ =================\n";
    std::cout << "Год: " << year << ", Часть света: " << continent << "\n";
    std::cout << "Страна с максимальным ВВП на душу населения: "
        << (bestGdpPc.empty() ? "не найдена" : bestGdpPc) << "\n";
    std::cout << "Страна с максимальными выбросами: "
        << (bestEm.empty() ? "не найдена" : bestEm) << "\n";

    std::cout << "Страны, где выбросы газа больше, чем угля и нефти:\n";
    if (gasDominant.empty()) std::cout << "  нет\n";
    else for (const auto& c : gasDominant) std::cout << "  - " << c << "\n";
}


int main() {
    setlocale(LC_ALL, "RU");

    readGDP("GDP.csv");
    readEmissions("Emissions.csv");
    readPopulationWide("Population.csv");

    std::cout << "Размер контейнера: " << data.size() << "\n";
    printLast50();

    int year;
    std::string continent;
    std::cout << "\nВведите год: ";
    std::cin >> year;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Введите часть света (например Europe или America): ";
    std::getline(std::cin, continent);
    continent = trim(continent);

    query(year, continent);
    return 0;
}
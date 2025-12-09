#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>

std::string remove_duplicates(const std::string& word) {
    std::string result;
    std::unordered_set<char> seen;
    for (char c : word) {
        if (!seen.count(c)) {
            result += c;
            seen.insert(c);
        }
    }
    return result;
}

int main() {
    setlocale(LC_ALL, "RU");

    std::string line;
    int n, k;
    std::cout << "Введите строку: ";
    std::getline(std::cin, line);
    std::cout << "Введите n и k для подстроки: ";
    std::cin >> n >> k;

    std::istringstream iss(line);
    std::vector<std::string> words;
    std::string word;

    while (iss >> word) {
        words.push_back(remove_duplicates(word));
    }

    std::vector<std::string> result_words;
    for (const auto& w : words) {
        if (k - 1 + n <= w.size()) {
            std::string sub = w.substr(k - 1, n);
            if (w.find(sub) == std::string::npos) {
                result_words.push_back(w);
            }
        }
        else {
            result_words.push_back(w);
        }
    }

    std::cout << "Результирующее предложение:\n";
    for (const auto& w : result_words) std::cout << w << " ";
    std::cout << "\n";

    return 0;
}

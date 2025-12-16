#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>
#include <locale>
#include <windows.h>

using namespace std;


bool isChar(unsigned char c) {
    return (c >= 0xC0 && c <= 0xFF) || c == 0xA8 || c == 0xB8;
}


char toLower(char c) {
    return (char)tolower((unsigned char)c);
}


string toLower(const string& s) {
    string res;
    for (unsigned char c : s) res += toLower(c);
    return res;
}


vector<string> getWords(const string& text) {
    string cleaned;
    for (unsigned char c : text)
        cleaned += isChar(c) ? toLower(c) : ' ';

    vector<string> words;
    istringstream iss(cleaned);
    string word;
    while (iss >> word) words.push_back(word);
    return words;
}


map<string, int> countFrequency(const vector<string>& words) {
    map<string, int> freq;
    for (auto& w : words) freq[w]++;
    return freq;
}


void removeStopWords(map<string, int>& freq, const set<string>& stopwords) {
    for (auto it = freq.begin(); it != freq.end(); ) {
        if (stopwords.count(it->first))
            it = freq.erase(it);
        else
            ++it;
    }
}


void printTopN(const map<string, int>& freq, int N) {
    multimap<int, string, greater<>> sorted;
    for (auto& [w, c] : freq)
        sorted.insert({ c, w });

    int k = 0;
    for (auto& [c, w] : sorted) {
        if (k++ == N) break;
        cout << c << " - " << w << endl;
    }
}


set<string> readStopWords(const string& filename) {
    set<string> stopwords;
    ifstream fin(filename);
    string word;
    while (fin >> word)
        stopwords.insert(toLower(word));
    return stopwords;
}


set<string> intersection(const map<string, int>& a, const map<string, int>& b) {
    set<string> res;
    for (auto& [w, _] : a)
        if (b.count(w)) res.insert(w);
    return res;
}


set<string> difference(const map<string, int>& a, const map<string, int>& b) {
    set<string> res;
    for (auto& [w, _] : a)
        if (!b.count(w)) res.insert(w);
    return res;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, ".1251");

    ifstream fin1("text1.txt");
    ifstream fin2("text2.txt");

    string text1((istreambuf_iterator<char>(fin1)), {});
    string text2((istreambuf_iterator<char>(fin2)), {});

    auto words1 = getWords(text1);
    auto words2 = getWords(text2);

    auto freq1 = countFrequency(words1);
    auto freq2 = countFrequency(words2);

    auto stopwords = readStopWords("stopwords-ru.txt");
    removeStopWords(freq1, stopwords);
    removeStopWords(freq2, stopwords);

    int N;
    cout << "Введите N для вывода самых частых слов: ";
    cin >> N;

    cout << "\nТоп " << N << " слов для text1:\n";
    printTopN(freq1, N);

    cout << "\nТоп " << N << " слов для text2:\n";
    printTopN(freq2, N);

    auto inter = intersection(freq1, freq2);
    auto diff = difference(freq1, freq2);

    cout << "\nПересечение слов:\n";
    for (auto& w : inter) cout << w << endl;

    cout << "\nРазность слов (text1 - text2):\n";
    for (auto& w : diff) cout << w << endl;

    set<unsigned char> vowels = { (unsigned char)'а', (unsigned char)'е', (unsigned char)'ё', (unsigned char)'и', (unsigned char)'о', (unsigned char)'у', (unsigned char)'ы', (unsigned char)'э', (unsigned char)'ю', (unsigned char)'я' };

    string substr;
    cout << "\nВведите подстроку для фильтра: ";
    cin >> substr;
    substr = toLower(substr);

    cout << "\nПервые " << N << " слов после фильтра:\n";
    int k = 0;
    for (auto& [word, _] : freq1) {
        bool start_vowel = vowels.count(word.front());
        bool end_vowel = vowels.count(word.back());
        bool contains = word.find(substr) != string::npos;


        if (!((start_vowel && end_vowel) || contains)) {
            cout << word << endl;
            if (++k == N) break;
        }
    }

    return 0;
}
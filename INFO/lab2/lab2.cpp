#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <concepts>
#include <chrono>
#include <ctime>

using namespace std;


class MyException {
    string message;

public:
    MyException(const string& msg) : message(msg) {}

    void printError() const {
        using namespace chrono;

        auto now = system_clock::now();
        time_t t = system_clock::to_time_t(now);
        char buffer[26];
        ctime_s(buffer, sizeof(buffer), &t);

        string timeStr(buffer);
        timeStr.pop_back();


        cerr << "[ERROR] " << message << " | Time: " << timeStr << "\n";

        ofstream log("error_log.txt", ios::app);
        log << "[ERROR] " << message << " | Time: " << timeStr << "\n";
    }
};


class Point2D {
private:
    double x, y;

public:
    Point2D(double xx = 0, double yy = 0) : x(xx), y(yy) {}

    double abs() const {
        return sqrt(x * x + y * y);
    }

    Point2D operator+(const Point2D& p) const {
        return Point2D(x + p.x, y + p.y);
    }

    Point2D operator-(const Point2D& p) const {
        return Point2D(x - p.x, y - p.y);
    }

    Point2D operator*(double k) const {
        return Point2D(x * k, y * k);
    }

    Point2D operator/(double k) const {
        return Point2D(x / k, y / k);
    }

    friend ostream& operator<<(ostream& os, const Point2D& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};



template <typename T>
concept Math = requires(T a, T b, double x) {
    { a + b } -> same_as<T>;
    { a - b } -> same_as<T>;
    { a* x } -> same_as<T>;
    { a / x } -> same_as<T>;
    { a.abs() } -> same_as<double>;
};

template<Math T>
class DynamicVector {
private:
    T* data;
    size_t length;
    size_t capacity;

    void reallocate(size_t newCap) {
        T* newData = new T[newCap];
        for (size_t i = 0; i < length; i++)
            newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = newCap;
    }

public:
    DynamicVector() : data(nullptr), length(0), capacity(0) {}

    ~DynamicVector() {
        delete[] data;
    }

    size_t size() const { return length; }

  
    T& operator[](int index) {
        if (index < 0 || index >= (int)length)
            throw MyException("Ошибка: оператор [] — индекс вне диапазона.");
        return data[index];
    }

    void push_back(const T& value) {
        if (length >= capacity)
            reallocate(capacity == 0 ? 2 : capacity * 2);
        data[length++] = value;
    }

   
    void erase(int index) {
        if (index < 0 || index >= (int)length)
            throw MyException("Ошибка: метод erase — индекс вне диапазона.");

        for (size_t i = index; i < length - 1; i++)
            data[i] = data[i + 1];
        length--;
    }

   
    void resize(int newSize) {
        if (newSize < 0)
            throw MyException("Ошибка: resize — отрицательный размер.");

        if (newSize > (int)capacity)
            reallocate(newSize);

        length = newSize;
    }

    int find(const T& value) const {
        for (size_t i = 0; i < length; i++)
            if (data[i].abs() == value.abs()) return (int)i;
        return -1;
    }

    DynamicVector operator+(const DynamicVector& other) const {
        DynamicVector result;
        size_t n = min(length, other.length);
        for (size_t i = 0; i < n; i++)
            result.push_back(data[i] + other.data[i]);
        return result;
    }

    DynamicVector operator*(const DynamicVector& other) const {
        DynamicVector result;
        size_t n = min(length, other.length);
        for (size_t i = 0; i < n; i++)
            result.push_back(data[i] * other.data[i].abs());
        return result;
    }

    template<typename... Args>
    void push_list(Args... args) {
        (push_back(check_limit(args)), ...);
    }

private:
    static T check_limit(T value) {
        if (value.abs() > 100)
            return value / 10.0;
        return value;
    }

    friend ostream& operator<<(ostream& os, const DynamicVector& v) {
        for (size_t i = 0; i < v.length; i++)
            os << v.data[i] << " ";
        return os;
    }
};



void processFile() {
    string filename;
    cout << "Введите имя файла: ";
    cin >> filename;

    ifstream in(filename);

    if (!in.is_open())
        throw MyException("Ошибка: файл не существует или недоступен.");

    string line;
    double a, b;

    while (getline(in, line)) {
        stringstream ss(line);

        if (!(ss >> a >> b)) {
            throw MyException("Ошибка: некорректная строка в файле.");
        }

        try {
            if (b == 0)
                throw MyException("Ошибка: деление на ноль.");

            cout << a << " / " << b << " = " << (a / b) << "\n";
        }
        catch (MyException& ex) {
            ex.printError();
        }
    }
}




int main() {
    setlocale(LC_ALL, "RU");

    try {
        DynamicVector<Point2D> v;
        v.resize(-5);
    }
    catch (MyException& e) {
        e.printError();
    }


    try {
        DynamicVector<Point2D> v;
        v.push_back(Point2D(1, 2));
        v.erase(10);
    }
    catch (MyException& e) {
        e.printError();
    }


    try {
        DynamicVector<Point2D> v;
        v.push_back(Point2D(3, 4));
        auto t = v[5];
    }
    catch (MyException& e) {
        e.printError();
    }


    try {
        processFile();
    }
    catch (MyException& e) {
        e.printError();
    }

    return 0;
}

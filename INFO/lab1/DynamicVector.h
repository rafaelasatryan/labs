#pragma once
#include <concepts>
#include <cmath>
#include <iostream>

using namespace std;

template <typename T>
concept  Math = requires(T a, T b, double x) {
	{ a + b } -> same_as<T>;
	{ a - b } -> same_as<T>;
	{ a * x } -> same_as<T>;
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
		for (size_t i = 0; i < length;i++)
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

	size_t size() const {
		return length;
	}

	void push_back(const T& value) {
		if (length >= capacity)
			reallocate(capacity == 0 ? 2 : capacity * 2);
		data[length++] = value;
	}

	void erase(int index) {
		if (index < 0 || index >= (int)length) return;
		for (size_t i = index; i < length - 1;i++)
			data[i] = data[i + 1];
		length--;
	}

	void resize(int newSize) {
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

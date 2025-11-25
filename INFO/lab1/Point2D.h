#pragma once
#include <cmath>

using namespace std;

class Point2D {
private:
	double x, y;

public:
	Point2D(double xx = 0, double yy = 0): x(xx), y(yy) {}

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

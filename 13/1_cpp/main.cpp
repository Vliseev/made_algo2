#include <bits/stdc++.h>
using namespace std;

template<class T>
struct Circle {
  T x{};
  T y{};
  T r{};
  Circle(T x, T y, T r) : x(x), y(y), r(r) {}
};

template<class T>
struct Point {
  T x{};
  T y{};
  Point(T x, T y) : x(x), y(y) {}
};

template<class T>
T square(T a) {
	return a * a;
}

void intersection(const Circle<int64_t> &c1, const Circle<int64_t> &c2) {
	int64_t d_square = square(c1.x - c2.x) + square(c1.y - c2.y);

	int64_t dx = c2.x - c1.x;
	int64_t dy = c2.y - c1.y;

	double d = sqrt(d_square);

	if (d_square > square(c1.r + c2.r)) {
		cout << 0 << "\n";
		return;
	} else if (d_square < square(c1.r - c2.r)) {
		cout << 0 << "\n";
		return;
	} else {
		double a = static_cast<double >((square(c1.r) - square(c2.r) + d_square)) / (2 * d);
		double h = sqrt(static_cast<double >(square(c1.r)) - square(a));
		double cx = static_cast<double>(c1.x) + static_cast<double>(a * dx) / static_cast<double>(d);
		double cy = static_cast<double>(c1.y) + static_cast<double>(a * dy) / static_cast<double>(d);

		if (d_square == square(c1.r + c2.r) || d_square == square(c1.r - c2.r)) {
			cout << 1 << "\n";
			cout << cx << " " << cy << "\n";
			return;
		}

		cout << 2 << "\n";
		cout << cx << " " << cy << "\n";
		cout << abs(a) << " " << abs(h) << "\n";
		double x = cx + h * static_cast<double >(dy) / d;
		double y = cy - h * static_cast<double >(dx) / d;
		cout << x << " " << y << "\n";
		x = cx - h * static_cast<double >(dy) / d;
		y = cy + h * static_cast<double >(dx) / d;
		cout << x << " " << y << "\n";
		return;
	}
}

void solution(istream &iss) {
	int n;
	iss >> n;
	for (int i = 0; i < n; ++i) {
		int64_t x, y, r;
		iss >> x >> y >> r;
		Circle<int64_t> c1(x, y, r);
		iss >> x >> y >> r;
		Circle<int64_t> c2(x, y, r);
		if (c1.x == c2.x && c1.y == c2.y) {
			if (c1.r == c2.r) {
				cout << 3 << "\n";
			} else {
				cout << 0 << "\n";
			}
		} else {
			intersection(c1, c2);
		}
	}

}

int main() {
	std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);
	std::cout.precision(16);
	solution(cin);
	return 0;
}

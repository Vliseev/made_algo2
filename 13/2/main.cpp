#include <bits/stdc++.h>
#include <ostream>
using namespace std;

static constexpr double EPS = 1e-16;
static constexpr double INF = 1e7;

template<class T>
int sgn(T x) { return (x > 0) - (x < 0); }

template<class T>
struct Point {
  T x, y;
  explicit Point(T x = 0, T y = 0) : x(x), y(y) {}
  bool operator<(Point p) const { return tie(x, y) < tie(p.x, p.y); }
  bool operator==(Point p) const { return tie(x, y) == tie(p.x, p.y); }
  Point operator+(Point p) const { return Point(x + p.x, y + p.y); }
  Point operator-(Point p) const { return Point(x - p.x, y - p.y); }
  Point operator*(T d) const { return Point(x * d, y * d); }
  Point operator/(T d) const { return Point(x / d, y / d); }
  T dot(Point p) const { return x * p.x + y * p.y; }
  T cross(Point p) const { return x * p.y - y * p.x; }
  T cross(Point a, Point b) const { return (a - *this).cross(b - *this); }
  T dist2() const { return x * x + y * y; }
  double dist() const { return sqrt((double)dist2()); }
  double angle() const { return atan2(y, x); }
  Point unit() const { return *this / dist(); }
  Point perp() const { return Point(-y, x); }
  Point normal() const { return perp().unit(); }
  Point rotate(double a) const {
	  return Point(x * cos(a) - y * sin(a), x * sin(a) + y * cos(a));
  }
  friend ostream &operator<<(ostream &os, Point p) {
	  return os << "(" << p.x << "," << p.y << ")";
  }
};

template<typename T>
static T round(T val) {
	if (val > INF) {
		return INF;
	} else if (val < -INF) {
		return -INF;
	}
	return val;
}

template<class T>
struct Line {
  T a{}, b{}, c{};
  Line() = default;
  Line(T a, T b, T c) : a(a), b(b), c(c) {}
  Line(Point<T> p1, Point<T> p2) {
	  a = p1.y - p2.y;
	  b = p2.x - p1.x;
	  c = p1.x * (p2.y - p1.y)
		  + p1.y * (p1.x - p2.x);
	  double norm = sqrt(a * a + b * b + c * c);
	  a = a / norm;
	  b = b / norm;
	  c = c / norm;
  };

  T det(T a1, T b1, T a2, T b2) {
	  return a1 * b2 - b1 * a2;
  }

  bool isIntersect(const Line &other) {
	  return det(a, b, other.a, other.c) > EPS;
  }

  Point<T> getIntersection(const Line &other) {
	  T zn = det(a, b, other.a, other.b);
	  T x = -det(c, b, other.c, other.b) / zn;
	  T y = -det(a, c, other.a, other.c) / zn;
	  return Point(x, y);
  }

  friend istream &operator>>(istream &os, Line &plate) {
	  os >> plate.a >> plate.b >> plate.c;
	  return os;
  }
  friend ostream &operator<<(ostream &os, const Line &plate) {
	  os << "a: " << plate.a << " b: " << plate.b << " c: " << plate.c;
	  return os;
  }
};

template<class T>
struct HPlate : public Line<T> {
  HPlate() = default;
  HPlate(T a, T b, T c) : Line<T>(a, b, c) {};

  bool isContainPoint(const Point<T> &p) {
	  return this->a * p.x + this->b * p.y + this->c >= -EPS;
  }

};

using DPoint = Point<double>;
using DHPlate = HPlate<double>;
using DLine = Line<double>;

double L(const DPoint &point, const DHPlate &plate) {
	return point.x * plate.a + point.y * plate.b + plate.c;
}

struct Poligon {
  vector<DPoint> points{};
  vector<DHPlate> plates{};
  Poligon(const vector<DPoint> &points, const vector<DHPlate> &plates)
	  : points(points), plates(plates) {};
  Poligon() = default;
};

void cut(vector<DPoint> &points, DHPlate &plate) {
	vector<DPoint> new_points;
	int n = points.size();
	for (int i = 0; i < n; i++) {
		int j = (i + 1) % n;
		if (plate.isContainPoint(points[i])) {
			new_points.push_back(points[i]);
		}
		if (L(points[i], plate) * L(points[j], plate) < -EPS) {
			auto ppi_line = DLine(points[i], points[j]);
			auto new_point = plate.getIntersection(ppi_line);
			new_points.push_back(new_point);
		}
	}
	swap(new_points, points);
}

double calcArea(const vector<DPoint> &points) {
	const auto p = points[0];
	double area = 0;
	for (int i = 1; i < points.size() - 1; ++i) {
		auto vec1 = points[i]-p;
		auto vec2 = points[i + 1]-p;
		area += vec1.cross(vec2);
	}
	return abs(area * 0.5);
}

void solution(istream &iss) {
	vector<DPoint> points = {DPoint(-INF, -INF),
							 DPoint(-INF, INF),
							 DPoint(INF, INF),
							 DPoint(INF, -INF)};

	int n;
	iss >> n;
	for (int i = 0; i < n; i++) {
		DHPlate plate;
		iss >> plate;
		cut(points, plate);
	}
	cout << calcArea(points);

}

int main() {
	istringstream iss("3\n"
					  "-3 -3 4\n"
					  "0 3 1\n"
					  "5 -3 -2");
	std::cout.precision(9);
	solution(cin);
	return 0;
}

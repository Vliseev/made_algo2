#include <bits/stdc++.h>
#include <ostream>
using namespace std;

static constexpr double EPS = 1e-8;
static constexpr double INF = 1e6;
static constexpr double MIN_AREA = 1e-8;

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
  friend istream &operator>>(istream &os, Point &point) {
      os >> point.x >> point.y;
      return os;
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
  };

  Line(const Line &other) = default;
  Line &operator=(const Line &other) = default;
  Line(Line &&other) = default;
  Line &operator=(Line &&other) = default;

  const Line<double> operator-() const {
      return Line(-a, -b, -c);
  }

  T det(T a1, T b1, T a2, T b2) const {
      return a1 * b2 - b1 * a2;
  }

  bool isIntersect(const Line &other) const {
      return det(a, b, other.a, other.c) > EPS;
  }

  Point<T> getIntersection(const Line &other) const {
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
  HPlate(const Line<T> l) : Line<T>(l) {};
  HPlate(const Point<T> &p1, const Point<T> &p2) : Line<T>(p1, p2) {};

  HPlate(const HPlate &other) = default;
  HPlate &operator=(const HPlate &other) = default;
  HPlate(HPlate &&other) = default;
  HPlate &operator=(HPlate &&other) = default;

  bool isContainPoint(const Point<T> &p) const {
      return this->a * p.x + this->b * p.y + this->c >= -EPS;
  }

};

using DPoint = Point<double>;
using DHPlate = HPlate<double>;
using DLine = Line<double>;

struct Part {
  Part() : points(
      {DPoint(-INF, -INF),
       DPoint(-INF, INF),
       DPoint(INF, INF),
       DPoint(INF, -INF)
      }) {}
  Part(const vector<DPoint> &p) : points(p) {};
  Part(vector<DPoint> &&p) : points(move(p)) {};

  double calcArea() const;
  vector<DPoint> points{};
};

double Part::calcArea() const {
    if (points.empty()) return 0;
    const auto p = points[0];
    double area = 0;
    for (int i = 1; i < points.size() - 1; ++i) {
        auto vec1 = points[i] - p;
        auto vec2 = points[i + 1] - p;
        area += vec1.cross(vec2);
    }
    return abs(area * 0.5);
}

double L(const DPoint &point, const DHPlate &plate) {
    return point.x * plate.a + point.y * plate.b + plate.c;
}

double cutVal(double d) {
    if (abs(abs(d) - INF) < EPS)
        d = d > 0 ? INF : -INF;
    return d;
}

DPoint cutPoint(DPoint p) {
    Point tp = p;
    tp.x = cutVal(tp.x);
    tp.y = cutVal(tp.y);
    return tp;
}

bool isInf(const DPoint &p) {
    return (abs(p.x) < INF) && (abs(p.y) < INF);
}

bool isInf(const Part &p) {
    bool is_inf = true;

    for (const auto &point: p.points) {
        if (!isInf(point))
            return false;
    }
    return true;
}

vector<DPoint> cut(const vector<DPoint> &points, const DHPlate &plate) {
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
            new_point = cutPoint(new_point);
            new_points.push_back(new_point);
        }
    }

    return new_points;
}

void solution(istream &iss) {
    vector<Part> parts(1);

    int n;
    iss >> n;
    DPoint p1, p2;
    for (int i = 0; i < n; i++) {
        vector<Part> tmp_parts;
        iss >> p1 >> p2;
        DHPlate plate(p1, p2);
        for (const auto &part: parts) {
            auto part1 = Part(cut(part.points, plate));
            if (part1.calcArea() > MIN_AREA)
                tmp_parts.push_back(part1);
            auto part2 = Part(cut(part.points, -plate));
            if (part2.calcArea() > MIN_AREA)
                tmp_parts.push_back(part2);
        }
        parts = move(tmp_parts);
    }

    vector<double> areas{};
    for (const auto &p : parts) {
        if (isInf(p)) {
            auto area = p.calcArea();

            if (area > MIN_AREA)
                areas.push_back(area);
        }
    }
    cout << areas.size() << "\n";
    sort(areas.begin(), areas.end());
    for (auto area: areas) {
        cout << area << "\n";
    }

}

int main() {
    std::cout.precision(16);
    solution(cin);
    return 0;
}

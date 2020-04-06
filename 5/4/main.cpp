#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

#define DEBUG 0
#define RELEASE 1
#define MODE RELEASE

#if MODE == DEBUG
template <typename T>
struct Matrix {
    Matrix() = default;

    Matrix(size_t n, size_t m, T val)
        : n(n), m(m), m_data(n, std::vector<T>(m, val)) {}
    T& operator()(size_t i, size_t j) {
        return m_data[i][j];
    }

    const T& operator()(size_t i, size_t j) const {
        return m_data[i][j];
    }

    size_t n{}, m{};
    std::vector<std::vector<T>> m_data{};
};

template <>
struct Matrix<bool> {
    using Ref = std::vector<bool>::reference;
    using ConstRef = std::vector<bool>::const_reference;

    Matrix() = default;
    Matrix(size_t n, size_t m, bool val)
        : n(n), m(m), m_data(n, std::vector<bool>(m, val)){};

    Ref operator()(size_t i, size_t j) {
        return m_data[i][j];
    }

    ConstRef operator()(size_t i, size_t j) const {
        return m_data[i][j];
    }

    size_t n{}, m{};
    std::vector<std::vector<bool>> m_data{};
};
#else
template <typename T>
struct Matrix {
    Matrix() = default;
    Matrix(size_t n, size_t m, T val) : n(n), m(m), m_data(n * m, val){};

    T& operator()(size_t i, size_t j) {
        return m_data[i * m + j];
    }

    const T& operator()(size_t i, size_t j) const {
        return m_data[i * m + j];
    }

    size_t n, m;
    std::vector<T> m_data;
};

template <>
struct Matrix<bool> {
    using ref = std::vector<bool>::reference;
    using const_ref = std::vector<bool>::const_reference;

    Matrix() = default;
    Matrix(size_t n, size_t m) : n(n), m(m), m_data(n * m){};

    ref operator()(size_t i, size_t j) {
        return m_data.at(i * m + j);
    }

    const_ref operator()(size_t i, size_t j) const {
        return m_data.at(i * m + j);
    }

    size_t n, m;
    std::vector<bool> m_data;
};
#endif

class RmqEng {
    size_t N, K;
    Matrix<int64_t> table_{};
    std::vector<int64_t> log_table_{};

 public:
    RmqEng(const std::vector<int64_t>& v);
    int64_t rmq(int64_t l, int64_t r);
};
RmqEng::RmqEng(const std::vector<int64_t>& v) {
    N = v.size();
    K = static_cast<size_t>(std::ceil(std::log2(N)));
    table_ = Matrix<int64_t>(N + 1, K + 1, std::numeric_limits<int64_t>::max());

    log_table_ = std::vector<int64_t>(N + 1, 0);
    log_table_[1] = 0;

    for (int i = 2; i <= N; i++)
        log_table_[i] = log_table_[i / 2] + 1;

    for (size_t i = 0; i < N; i++) {
        table_(i, 0) = v[i];
    }

    for (size_t j = 1; (1 << j) <= N; j++) {
        size_t power = 1;
        for (size_t i = 0; i + (1UL << (j - 1)) < N; i++) {
            auto id1 = i + (1 << (j - 1));
            auto val1 = table_(i, j - 1);
            auto val2 = table_(id1, j - 1);

            table_(i, j) = std::min(val1, val2);
        }
    }
}
int64_t RmqEng::rmq(int64_t l, int64_t r) {
    int64_t idx = log_table_[r - l + 1];
    return std::min(table_(l, idx), table_(r - (1 << idx) + 1, idx));
}

void solution(std::istream& iss) {
    int64_t n, m, a1;
    int64_t u1, v1;

    iss >> n >> m >> a1 >> u1 >> v1;
    std::vector<int64_t> a(n, 0);
    a[0] = a1;

    for (int i = 1; i < n; ++i) {
        a[i] = (23 * a[i - 1] + 21563) % 16714589;
    }

    RmqEng rmq(a);
    int64_t r, u = u1, v = v1;
    for (int j = 1; j < m; ++j) {
        r = rmq.rmq(std::min(u - 1, v - 1), std::max(u - 1, v - 1));
        u = (17 * u) % n;
        u = (u + 751 + r) % n;
        u = (u + 2 * j) % n;
        u += 1;
        v = (13 * v) % n;
        v = (v + 593 + r) % n;
        v = (v + 5 * j) % n;
        v += 1;
    }
    r = rmq.rmq(std::min(u - 1, v - 1), std::max(u - 1, v - 1));
    std::cout << u << " " << v << " " << r;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    solution(std::cin);
    return 0;
}
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

template <typename T>
struct Matrix {
    Matrix(int n, int m) : n(n), m(m), m_data(n * m){};

    T& operator()(int i, int j) {
        return m_data[i * m + j];
    }

    const T& operator()(int i, int j) const {
        return m_data[i * m + j];
    }

    int n, m;
    std::vector<T> m_data;
};

template <>
struct Matrix<bool> {
    using ref = std::vector<bool>::reference;
    using const_ref = std::vector<bool>::const_reference;

    Matrix(int n, int m) : n(n), m(m), m_data(n * m){};

    ref operator()(int i, int j) {
        return m_data.at(i * m + j);
    }

    const_ref operator()(int i, int j) const {
        return m_data.at(i * m + j);
    }

    int n, m;
    std::vector<bool> m_data;
};

bool getBit(uint32_t a, int i) {
    return (a >> i) & 1;
}

void solution(std::istream& iss) {
    int n;
    iss >> n;
    Matrix<bool> m(n, n);
    uint32_t order = 1u << n;

    std::vector<int> dp(order, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            char c;
            iss >> c;
            m(i, j) = c != 'N';
        }
    }
    dp[0] = 0;
    for (uint32_t mask = 1; mask < order; ++mask) {
        for (int i = 0; i < n; ++i) {
            if (getBit(mask, i)) {
                dp[mask] = dp[mask - (1 << i)];
                for (int j = 0; j < n; ++j) {
                    if (j != i && getBit(mask, j) && m(i, j)) {
                        dp[mask] =
                            std::max(dp[mask], dp[mask - (1 << i) - (1 << j)]+1);
                    }
                }
            }
        }
    }

    std::cout << dp[order - 1]*2;
}

int main() {
    std::istringstream iss(
        "5\n"
        "NYYYY\n"
        "YNNNN\n"
        "YNNNN\n"
        "YNNNN\n"
        "YNNNN");
    solution(std::cin);
    return 0;
}

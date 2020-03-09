#include <algorithm>
#include <iostream>
#include <vector>

template <typename T>
struct Matrix {
    Matrix(int n, int m, T val = 0) : n(n), m(m), m_data(n * m, val){};

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
    Matrix(int n, int m, bool val = false) : n(n), m(m), m_data(n * m, val){};

    std::vector<bool>::reference operator()(int i, int j) {
        return m_data[i * m + j];
    }

    std::vector<bool>::const_reference operator()(int i, int j) const {
        return m_data[i * m + j];
    }

    int n, m;
    std::vector<bool> m_data;
};

bool getBit(uint32_t a, int i) {
    return (a >> i) & 1;
}

void solution(std::istream& iss) {
    int n, m;
    iss >> n >> m;
    Matrix<bool> desk(n, m + 1, false);
    std::vector<int> masks(m);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            char c;
            iss >> c;
            desk(i, j) = c == 'X';
        }
    }
    Matrix<uint64_t> dp(n * m + 1, 1 << n);
    dp(0, 0) = 1;
    for (int i = 1; i < (1 << n); ++i) {
        dp(0, i) = 0;
    }
    for (int x = 0; x < n * m; x++) {
        for (int mask = 0; mask < (1 << n); mask++) {
            int i_id = x % n;
            int j_id = x / n;

            if ((mask & 1) || desk(i_id, j_id)) {    //или дырка
                dp(x + 1, mask >> 1) += dp(x, mask);
            } else {
                //проверка на дырку справа в профиле
                auto mask_1 = (mask >> 1) + (1 << (n - 1));
                if (desk(i_id, j_id + 1) == false) {
                    dp(x + 1, mask_1) += dp(x, mask);
                }

                auto mask_2 = (mask >> 1) + 1;

                if (((x % n) != n - 1) && ((mask & 2) == 0)) {
                    //проверка на нижнюю дырку
                    int i_id = (x + 1) % n;
                    int j_id = (x + 1) / n;
                    if (desk(i_id, j_id) == false) {
                        dp(x + 1, mask_2) += dp(x, mask);
                    }
                }
            }
        }
    }

    std::cout << dp(n * m, 0);
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr),
        std::cout.tie(nullptr);

    solution(std::cin);

    return 0;
}
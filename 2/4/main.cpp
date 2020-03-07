#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

template <typename T>
struct Matrix {
    Matrix(int n, int m, int val = 0) : n(n), m(m), m_data(n * m, val){};

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

static const int INF = 1000000000;

void solution(std::istream& iss) {
    int n;
    iss >> n;
    if (n == 1) {
        std::cout << 0 << "\n" << 1;
        return;
    }

    int max_mask = (1 << n);
    Matrix<int> dp(max_mask, n, INF), path(max_mask, n);
    Matrix<int> dist(n, n);
    int min_dist = INF;
    int min_id{};

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            iss >> dist(i, j);
        }
    }

    for (int i = 0; i < n; ++i) {
        dp(1 << i, i) = 0;
    }

    for (int mask = 1; mask < (1 << n); mask++) {
        for (int i = 0; i < n; i++) {
            if (getBit(mask, i)) {
                int mask_without_i = mask ^ (1 << i);

                for (int j = 0; j < n; j++) {
                    if (j != i && (getBit(mask, j))) {
                        if (dp(mask_without_i, j) + dist(j, i) < dp(mask, i)) {
                            dp(mask, i) = dp(mask_without_i, j) + dist(j, i);
                            path(mask, i) = j;
                        }
                    }
                }
            }
            if ((mask == max_mask - 1) && dp(mask, i) < min_dist) {
                min_dist = dp(mask, i);
                min_id = i;
            }
        }
    }

    std::cout << min_dist << "\n";

    int cur_mask = max_mask - 1;

    while (cur_mask > 0) {
        std::cout << min_id + 1 << " ";
        int temp = cur_mask;
        cur_mask -= (1 << min_id);
        min_id = path(temp, min_id);
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);
    solution(std::cin);

    return 0;
}
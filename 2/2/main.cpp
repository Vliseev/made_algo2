#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

template <typename T>
struct Matrix {
    Matrix(int n, int m) : n(n), m(m), m_data(n * m, 0){};

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

void printAns(const std::string& s, int l, int r, const Matrix<int>& dp,
              const Matrix<int>& sp) {
    if (dp(l, r) == r - l + 1) {
        return;
    }
    if (sp(l, r) == std::numeric_limits<int>::max()) {
        std::cout << s[l];
        printAns(s, l + 1, r - 1, dp, sp);
        std::cout << s[r];
    } else {
        if (dp(l, r) == 0) {
            std::cout << s.substr(l, r - l + 1);
        } else {
            int k = sp(l, r);
            printAns(s, l, k, dp, sp);
            printAns(s, k + 1, r, dp, sp);
        }
    }
}

void solution(std::istream& iss) {
    std::string s;
    iss >> s;
    int n = s.size();
    Matrix<int> dp(n + 1, n + 1), sp(n + 1, n + 1);
    for (int r = 0; r < n; r++) {
        dp(r, r) = 1;
        for (int l = r - 1; l >= 0; l--) {
            int min_del = std::numeric_limits<int>::max();
            int split_pos = std::numeric_limits<int>::max();
            if ((s[l] == '(' && s[r] == ')') || (s[l] == '[' && s[r] == ']') ||
                (s[l] == '{' && s[r] == '}')) {
                min_del = dp(l + 1, r - 1);
            }

            for (int k = l; k < r; k++) {
                auto cur_min = dp(l, k) + dp(k + 1, r);
                if (cur_min < min_del) {
                    min_del = cur_min;
                    split_pos = k;
                }
            }
            dp(l, r) = min_del;
            sp(l, r) = split_pos;
        }
    }

    printAns(s, 0, n - 1, dp, sp);
}

int main() {
    std::istringstream iss(")[()(]");
    solution(std::cin);
    return 0;
}

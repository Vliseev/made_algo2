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

void GetPath(int i, int j, const Matrix<int>& p) {
    if (i == 0 && j == 0)
        return;
    if (p(i, j) == 0) {
        GetPath(i - 1, j, p);
        std::cout << "D";
    }
    if (p(i, j) == 1) {
        GetPath(i, j - 1, p);
        std::cout << "R";
    }
}

void Solution(std::istream& iss) {
    int n, m;
    iss >> n >> m;

    Matrix<int> board(n, m), dp(n, m);
    Matrix<std::pair<int, int>> p(n, m);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int val;
            iss >> val;
            board(i, j) = val;
        }
    }

    dp(0, 0) = board(0, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (i == 0 && j > 0) {
                dp(i, j) = dp(i, j - 1) + board(i, j);
                p(i, j) = {i, j - 1};
            }
            if (i > 0 && j == 0) {
                dp(i, j) = dp(i - 1, j) + board(i, j);
                p(i, j) = {i - 1, j};
            }
            if (i > 0 && j > 0) {
                if (dp(i - 1, j) > dp(i, j - 1)) {
                    dp(i, j) = dp(i - 1, j) + board(i, j);
                    p(i, j) = {i - 1, j};
                } else {
                    dp(i, j) = dp(i, j - 1) + board(i, j);
                    p(i, j) = {i, j - 1};
                }
            }
        }
    }

    std::vector<std::pair<int, int>> ans;
    {
        int i = n - 1, j = m - 1;
        while (i > 0 || j > 0) {
            ans.emplace_back(i, j);
            auto ni = p(i, j).first;
            auto nj = p(i, j).second;
            i = ni;
            j = nj;
        }
    }
    std::cout << dp(n - 1, m - 1) << "\n";
    std::reverse(ans.begin(), ans.end());
    if (ans[0].first > 0) {
        std::cout << "D";
    } else {
        std::cout << "R";
    }
    for (int i = 1; i < ans.size(); ++i) {
        if (ans[i - 1].first < ans[i].first) {
            std::cout << "D";
        } else {
            std::cout << "R";
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr),
        std::cout.tie(nullptr);

    Solution(std::cin);

    return 0;
}

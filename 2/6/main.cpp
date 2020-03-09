#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

template <typename T>
struct Matrix {
    Matrix(int n, int m, T val)
        : n(n), m(m), m_data(n, std::vector<T>(m, val)) {
    }
    T& operator()(int i, int j) {
        return m_data[i][j];
    }

    const T& operator()(int i, int j) const {
        return m_data[i][j];
    }

    int n, m;
    std::vector<std::vector<int>> m_data;
};

template <>
struct Matrix<bool> {
    using Ref = std::vector<bool>::reference;
    using ConstRef = std::vector<bool>::const_reference;

    Matrix(int n, int m, bool val)
        : n(n), m(m), m_data(n, std::vector<bool>(m, val)){};

    Ref operator()(int i, int j) {
        return m_data[i][j];
    }

    ConstRef operator()(int i, int j) const {
        return m_data[i][j];
    }

    int n, m;
    std::vector<std::vector<bool>> m_data;
};

static const int INF = 1e9;

bool canDel(int t1, int t2, int t3) {
    return (t1 < t2 && t3 < t2) || (t1 > t2 && t3 > t2);
}

void recDel(int l, int r, std::vector<int>& del_pos,
            const Matrix<int>& del_dp) {
    if (del_dp(l, r) != l) {
        recDel(l, del_dp(l, r), del_pos, del_dp);
        recDel(del_dp(l, r), r, del_pos, del_dp);
        del_pos.push_back(del_dp(l, r));
    }
}

void printAns(int l, int r, std::vector<int>& del_pos,
              const Matrix<int>& del_dp, const Matrix<int>& sp) {
    if (sp(l, r) == l) {
        recDel(l, r, del_pos, del_dp);
    } else {
        printAns(l, sp(l, r), del_pos, del_dp, sp);
        printAns(sp(l, r), r, del_pos, del_dp, sp);
    }
}

void solution(std::istream& iss) {
    int n;
    iss >> n;
    std::vector<int> v(n, 0);
    for (int i = 0; i < n; ++i) {
        iss >> v[i];
    }

    Matrix<int> del_dp(n + 1, n + 1, -1), dp(n + 1, n + 1, INF),
        pos(n + 1, n + 1, -1);

    for (int i = 0; i + 1 < n; i++) {
        del_dp(i, i + 1) = i;
    }

    for (int len = 2; len < n; len++) {
        for (int beg = 0; beg + len < n; beg++) {
            int end = beg + len;
            for (int i = beg + 1; i < end; i++) {
                if (del_dp(beg, i) >= 0 && del_dp(i, end) >= 0 &&
                    canDel(v[beg], v[i], v[end])) {
                    del_dp(beg, end) = i;
//                    break;
                }
            }
        }
    }

    for (int len = 1; len < n; len++) {
        for (int beg = 0; beg + len < n; ++beg) {
            int end = beg + len;
            if (v[beg] > v[end])
                continue;
            if (del_dp(beg, end) >= 0) {
                dp(beg, end) = end - beg - 1;
                pos(beg, end) = beg;
            }
            for (int i = beg + 1; i < end; i++) {
                if (v[beg] > v[i] || v[i] > v[end])
                    continue;
                auto min_v = dp(beg, i) + dp(i, end);

                if (dp(beg, end) > min_v) {
                    dp(beg, end) = min_v;
                    pos(beg, end) = i;
                }
            }
        }
    }
    if (pos(0, n - 1) < 0) {
        std::cout << "-1";
        return;
    }

    std::vector<int> del_pos;
    printAns(0, n - 1, del_pos, del_dp, pos);
    std::cout << del_pos.size() << "\n";
    for (auto p : del_pos) {
        std::cout << p + 1 << "\n";
    }
}

int main() {
    std::istringstream iss(
        "6\n"
        "400 300 310 300 310 500");
    solution(std::cin);
    return 0;
}

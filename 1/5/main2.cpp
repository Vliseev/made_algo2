#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <valarray>
#include <vector>

static const int kMaxPrice = 100;
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

void Solution(std::istream& iss) {
    int n;
    iss >> n;
    Matrix<int> dp(n + 1, n + 1);
    std::vector<int> price(n + 1, 0);

    for (int i = 1; i <= n; ++i) {
        iss >> price[i];
    }

    dp(0, 0) = 0;
    for (int j = 1; j <= n; ++j) {
        dp(0, j) = std::numeric_limits<int>::max();
        dp(1, j) = std::numeric_limits<int>::max();
    }
    dp(1, 0) = price[1];
    if (price[1] > kMaxPrice)
        dp(1, 1) = price[1];

    for (int i = 2; i <= n; ++i) {
        for (int j = 0; j <= n; ++j) {
            if (0 < j && j < n) {
                if (price[i] <= kMaxPrice) {
                    auto min =
                        std::min(dp(i - 1, j) + price[i], dp(i - 1, j + 1));
                    dp(i, j) = min >= 0 ? min : std::numeric_limits<int>::max();

                } else {
                    auto min =
                        std::min(dp(i - 1, j - 1) + price[i], dp(i - 1, j + 1));
                    dp(i, j) = min >= 0 ? min : std::numeric_limits<int>::max();
                }
            } else if (j == 0) {
                auto min = std::min(dp(i - 1, 0) + price[i], dp(i - 1, 1));
                dp(i, j) = min >= 0 ? min : std::numeric_limits<int>::max();
            } else if (j == n) {
                if (price[i] <= kMaxPrice) {
                    dp(i, j) = dp(i - 1, j) + price[i];
                    dp(i, j) = dp(i, j) >= 0 ? dp(i, j)
                                             : std::numeric_limits<int>::max();
                } else {
                    dp(i, j) = dp(i - 1, j - 1) + price[i];
                    dp(i, j) = dp(i, j) >= 0 ? dp(i, j)
                                             : std::numeric_limits<int>::max();
                }
            }
        }
    }

    int min_el = dp(n, 0);
    int min_id = 0;
    for (int i = 1; i <= n; ++i) {
        if (dp(n, i) <= min_el) {
            min_el = dp(n, i);
            min_id = i;
        }
    }
    std::cout << min_el << "\n";
    std::cout << min_id << " ";
    if (n > 1) {
        std::vector<int> pos;

        for (int i = n; i >= 1; i--) {
            if (dp(i, min_id) == dp(i - 1, min_id + 1)) {
                pos.push_back(i);
                min_id++;
            } else {
                if (price[i] > kMaxPrice)
                    min_id--;
            }
        }
        std::cout << pos.size() << "\n";

        std::copy(pos.rbegin(), pos.rend(),
                  std::ostream_iterator<int>(std::cout, "\n"));
    } else {
        std::cout << 0;
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr),
        std::cout.tie(nullptr);
    Solution(std::cin);

    return 0;
}

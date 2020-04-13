#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>

int64_t F(int64_t t) {
    return t & (t + 1);
}

class FanwikTree {
    std::vector<std::vector<std::vector<int64_t>>> tree_{};
    int64_t n_;

 public:
    FanwikTree(int64_t n)
        : n_(n),
          tree_(n, std::vector<std::vector<int64_t>>(
                       n, std::vector<int64_t>(n, 0))){};

    int64_t Sum(int64_t x, int64_t y, int64_t z) {
        int64_t ans = 0;
        for (int64_t i = x; i >= 0; i = F(i) - 1) {
            for (int64_t j = y; j >= 0; j = F(j) - 1) {
                for (int64_t k = z; k >= 0; k = F(k) - 1) {
                    ans += tree_[i][j][k];
                }
            }
        }
        return ans;
    }

    int64_t Sum(int64_t x1, int64_t y1, int64_t z1, int64_t x2, int64_t y2,
                int64_t z2) {
        return (Sum(x2, y2, z2) - Sum(x1 - 1, y2, z2) - Sum(x2, y1 - 1, z2) +
                Sum(x1 - 1, y1 - 1, z2)) -
               (Sum(x2, y2, z1 - 1) - Sum(x1 - 1, y2, z1 - 1) -
                Sum(x2, y1 - 1, z1 - 1) + Sum(x1 - 1, y1 - 1, z1 - 1));
    }

    void Add(int64_t x, int64_t y, int64_t z, int64_t val) {
        for (int64_t i = x; i < n_; i = i | (i + 1)) {
            for (int64_t j = y; j < n_; j = j | (j + 1)) {
                for (int64_t k = z; k < n_; k = k | (k + 1)) {
                    tree_[i][j][k] += val;
                }
            }
        }
    }
};

void solution(std::istream& iss) {
    int64_t n;
    iss >> n;
    FanwikTree tree(n);
    int64_t query_type;
    int64_t x1, y1, z1, x2, y2, z2;
    int64_t k;

    while (true) {
        iss >> query_type;
        if (query_type == 1) {
            iss >> x1 >> y1 >> z1 >> k;
            tree.Add(x1, y1, z1, k);
        } else if (query_type == 2) {
            iss >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
            std::cout << tree.Sum(x1, y1, z1, x2, y2, z2) << "\n";
        } else {
            return;
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr),
        std::cout.tie(nullptr);
    std::istringstream iss(
        "3\n"
        "2 1 1 1 1 1 1\n"
        "1 0 0 0 1\n"
        "1 0 1 0 3\n"
        "2 0 0 0 0 0 0\n"
        "2 0 0 0 0 1 0\n"
        "1 0 1 0 -2\n"
        "2 0 0 0 1 1 1\n"
        "3");

    solution(iss);

    return 0;
}

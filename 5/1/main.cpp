#include <iostream>
#include <sstream>
#include <vector>
class RsqEng {
    std::vector<int64_t> sum_{};

 public:
    RsqEng(int n, int64_t x, int y, int a_0);
    int64_t Rsq(int l, int r);
};
RsqEng::RsqEng(int n, int64_t x, int y, int a_0) : sum_(n, 0) {
    int64_t cur_a = a_0;
    sum_[0] = cur_a;

    for (int i = 1; i < n; ++i) {
        cur_a = (cur_a * x) & ((1 << 16) - 1);
        cur_a = (cur_a + y) & ((1 << 16) - 1);
        sum_[i] = sum_[i - 1] + cur_a;
    }
}
int64_t RsqEng::Rsq(int l, int r) {
    if (l == 0) {
        return sum_[r];
    } else {
        return sum_[r] - sum_[l - 1];
    }
}

void solution(std::istream &iss) {
    int n, y, a_0;
    int64_t x;

    int m;
    int64_t z, t, b_0;

    iss >> n >> x >> y >> a_0;
    iss >> m >> z >> t >> b_0;

    if (m == 0) {
        std::cout << 0;
        return;
    }

    RsqEng rsq_eng(n, x, y, a_0);
    std::vector<int> c(2 * m, 0);

    int64_t cur_b = b_0;
    c[0] = b_0 % n;

    for (int i = 1; i < 2 * m; ++i) {
        cur_b = (cur_b * z) & ((1 << 30) - 1);
        cur_b += t;
        cur_b = cur_b > 0 ? cur_b : cur_b + (1 << 30);
        cur_b = cur_b & ((1 << 30) - 1);
        c[i] = cur_b % n;
    }

    int64_t sum = 0;
    for (int i = 0; i < 2 * m; i += 2) {
        auto l = c[i];
        auto r = c[i + 1];

        if (r < l)
            std::swap(r, l);

        sum += rsq_eng.Rsq(l, r);
    }
    std::cout << sum;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    solution(std::cin);
    return 0;
}

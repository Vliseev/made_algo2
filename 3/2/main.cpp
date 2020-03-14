#include <iostream>
#include <sstream>
#include <valarray>
#include <vector>

using p_ofi = std::pair<uint64_t, int>;
using vi = std::vector<uint64_t>;
using fi = std::vector<p_ofi>;

int findFactors(int p, const vi& lp, fi& result) {
    int idx = 0;
    result[idx++] = {lp[p], 1};
    while (p != lp[p]) {
        p /= lp[p];
        if (result[idx - 1].first == lp[p]) {
            result[idx - 1].second++;
        } else {
            result[idx++] = {lp[p], 1};
        }
    }
    return idx;
}

void sieve(int n, vi& lp, vi& primes) {
    lp.resize(n + 1);
    for (int i = 2; i <= n; i++) {
        if (lp[i] == 0) {
            lp[i] = i;
            primes.push_back(i);
        }
        for (int p : primes) {
            if (p > lp[i] || i * p >= n + 1) {
                break;
            }
            lp[i * p] = p;
        }
    }
}

void solution(std::istream& iss) {
    int n;
    uint64_t d = 0, s_0 = 1, s_1 = 1, phi = 1;

    iss >> n;
    vi lp, primes;
    fi result;
    result.reserve(1000);

    sieve(n + 1, lp, primes);

    for (int i = 2; i <= n; ++i) {
        if (lp[i] != i) {
            int f_size = findFactors(i, lp, result);
            uint64_t cur_s_0 = 1;
            uint64_t cur_s_1 = 1;
            uint64_t cur_phi = 1;
            for (int i = 0; i < f_size; i++) {
                cur_s_0 *= (result[i].second + 1);
                auto p_k = static_cast<uint64_t>(
                    std::pow(result[i].first, result[i].second));
                cur_s_1 *= (p_k * result[i].first - 1) / (result[i].first - 1);
                cur_phi *= (p_k - p_k / result[i].first);
            }
            d += result.front().first;
            s_0 += cur_s_0;
            s_1 += cur_s_1;
            phi += cur_phi;
        } else {
            d += i;
            s_0 += 2;
            s_1 += (i + 1);
            phi += (i - 1);
        }
    }
    std::cout << d << " " << s_0 << " " << s_1 << " " << phi << "\n";
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    solution(std::cin);

    return 0;
}

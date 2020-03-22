#include <stdint.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

using vi = std::vector<int64_t>;

int64_t binPowMod(int64_t a, int64_t b, int64_t n) {

    int64_t b_a = a, b_n = n;
    int64_t res = 1;
    while (b > 0) {
        if (b & 1) {
            res = (res * b_a) % b_n;
            b--;
        } else {
            b_a = (b_a * b_a) % b_n;
            b >>= 1;
        }
    }
    return res % n;
}

vi findFacors(int64_t n) {
    vi result;
    uint64_t tmp_n = n;
    for (int i = 2; i * i < n; ++i) {
        if ((tmp_n % i) == 0) {
            result.push_back(i);
            tmp_n /= i;
        }
        while ((tmp_n % i) == 0) {
            result.push_back(i);
            tmp_n /= i;
        }
    }
    if (tmp_n != 1)
        result.push_back(tmp_n);

    return result;
}

void solution(std::istream& iss) {
    int64_t a, b, n, q;
    __int128 a1;

    iss >> a >> b >> n;
    q = n - 1;
    std::unordered_map<int64_t, int> table;

    int64_t k = std::sqrt(n) + 10;
    int64_t r = q - k;
    r = r % q;
    a1 = binPowMod(a, r, n);

    __int128 key = b;
    key %= n;
    table[key] = 0;

    for (int64_t i = 1; i < k; ++i) {
        key = (key * a1) % n;
        table[key] = i;
    }

    for (int64_t i = 0; i < k + 2; ++i) {
        auto key = binPowMod(a, i, n);
        auto it = table.find(key);
        if (it != table.end()) {
            std::cout << (k * it->second + i) % q << "\n";
            break;
        }
    }
}

int main() {
    std::istringstream iss("23 36704214080 114436818211\n");
    solution(iss);

    return 0;
}

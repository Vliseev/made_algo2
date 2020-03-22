#include <iostream>
#include <sstream>
#include <vector>

using vi = std::vector<uint64_t>;

uint64_t binPowMod(uint64_t a, uint64_t b, uint64_t n) {
    uint64_t res = 1;
    while (b) {
        if (b & 1) {
            res = (res * a) % n;
            b--;
        } else {
            a = (a * a) % n;
            b >>= 1;
        }
    }
    return res;
}

vi findFacors(uint64_t n) {
    vi result;
    uint64_t tmp_n = n;
    for (int i = 2; i * i < n; ++i) {
        if ((tmp_n % i) == 0) {
            result.push_back(i);
            tmp_n /= i;
        }
        while ((tmp_n % i) == 0) {
            // result.push_back(i);
            tmp_n /= i;
        }
    }
    if (tmp_n != 1)
        result.push_back(tmp_n);

    return result;
}

void solution(std::istream& iss) {
    uint64_t p, q;
    iss >> p;
    q = p - 1;
    auto factors = findFacors(q);

    for (int i = 1; i < p; i++) {
        bool is_root = true;
        for (auto delim : factors) {
            if (binPowMod(i, q / delim, p) == 1) {
                is_root = false;
                break;
            }
        }
        if (is_root) {
            std::cout << i;
            return;
        }
    }
}

int main() {
    // std::istringstream iss("999999929");
    solution(std::cin);

    return 0;
}

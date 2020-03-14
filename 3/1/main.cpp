#include <iostream>
#include <sstream>
#include <vector>
using vi = std::vector<int>;
static const int B = 1e6;
static const int FbSize = 1e6 + 1;
 
void sieve(int n, vi& lp, vi& primes) {
    lp.resize(n+1);
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
    vi lp, primes;
    sieve(B + 1, lp, primes);
 
    iss >> n;
    for (int i = 0; i < n; ++i) {
        int v;
        iss >> v;
        std::cout << lp[v] << " ";
        while (v != lp[v]) {
            v /= lp[v];
            std::cout << lp[v] << " ";
        }
        std::cout << "\n";
    }
}
 
int main() {
 std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    solution(std::cin);
 
    return 0;
}

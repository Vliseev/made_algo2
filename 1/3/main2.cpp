#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

void Solution(std::istream& iss) {
    int n;
    iss >> n;
    std::vector<int> v(n), d(n, 1), prev(n, -1);

    std::copy(std::istream_iterator<int>(iss), std::istream_iterator<int>(),
              v.begin());

    auto max_idx = 0;
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (v[j] < v[i] && d[i] < d[j] + 1) {
                d[i] = d[j] + 1;
                prev[i] = j;
            }
        }
        if (d[i] > d[max_idx])
            max_idx = i;
    }

    std::cout << d[max_idx] << "\n";
    auto cud_idx = max_idx;
    std::vector<int> ans;

    while (cud_idx != -1) {
        ans.push_back(v[cud_idx]);
        cud_idx = prev[cud_idx];
    }
    std::copy(ans.rbegin(), ans.rend(),
              std::ostream_iterator<int>(std::cout, " "));
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr),
        std::cout.tie(nullptr);
    Solution(std::cin);

    return 0;
}

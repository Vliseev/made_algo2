#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

void Solution(std::istream &iss, std::ofstream &oss) {
    int n, k;
    std::vector<int> coins;
    std::vector<int> d;
    std::vector<int> prev;
    std::vector<int> path;

    iss >> n >> k;

    coins.assign(n + 1,0);
    d.assign(n + 1,0);
    prev.assign(n + 1,0);
    for (int i = 2; i < n; ++i) {
        int val;
        iss >> val;
        coins[i] = val;
    }

    d[1] = 0;
    coins[n] = 0;

    for (int i = 2; i <= n; ++i) {
        auto beg_it = i - k > 0 ? d.begin() + i - k : d.begin()+1;
        auto it = std::max_element(beg_it, d.begin() + i);

        d[i] = *it + coins[i];
        prev[i] = std::distance(d.begin(), it);
    }
    oss << d[n] << "\n";
    auto id = prev[n];

    path.push_back(n);
    while (id != 0) {
        path.push_back(id );
        id = prev[id];
    }
    std::reverse(path.begin(), path.end());

    oss << path.size() - 1 << "\n";
    for (auto el : path) {
        oss << el << " ";
    }
}

int main() {
    std::ifstream ifs("input.txt");
    std::ofstream ofs("output.txt");

    Solution(ifs, ofs);

    return 0;
}

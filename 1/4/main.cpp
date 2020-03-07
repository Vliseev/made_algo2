//
// Created by user on 02.03.2020.
//
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

int LevenstainDistance(const std::string& s1, const std::string& s2) {
    auto m = s1.size();
    auto n = s2.size();

    if (m > n)
        return LevenstainDistance(s2, s1);

    std::vector<int> dist(m + 1), prev_dist(m + 1);

    for (int i = 0; i <= m; ++i) {
        dist[i] = i;
    }

    for (int i = 1; i <= n; ++i) {
        prev_dist = dist;
        dist[0] += 1;
        for (int j = 1; j <= m; ++j) {
            if (s1[j - 1] != s2[i - 1]) {
                dist[j] = std::min(dist[j-1],
                                   std::min(dist[j], prev_dist[j - 1])) +
                          1;
            } else {
                dist[j] = prev_dist[j - 1];
            }
        }
    }
    return dist[m];
}

void Solution(std::istream& iss) {
    int n;
    std::string s, v;
    iss >> s >> v;

    std::cout << LevenstainDistance(s, v);
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr),
        std::cout.tie(nullptr);


    Solution(std::cin);

    return 0;
}

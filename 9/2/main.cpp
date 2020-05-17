#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>
using namespace std;
static const int64_t INF = std::numeric_limits<int64_t>::max();

struct Edge {
    int64_t cap{}, flow{}, cost{};
};

class MinCostMaxFlow {
    int N;
    vector<vector<Edge>> edges;
    vector<int> found;
    vector<int64_t> dist, pi, width;
    vector<pair<int, int>> dad;

   public:
    MinCostMaxFlow(int N)
        : N(N),
          edges(N, vector<Edge>(N)),
          found(N),
          dist(N),
          pi(N),
          width(N),
          dad(N) {}

    void addEdge(int from, int to, int64_t cap, int64_t cost) {
        edges[from][to].cap = cap;
        edges[from][to].cost = cost;
    }

    void relax(int s, int k, int64_t cap, int64_t cost, int dir) {
        int64_t val = dist[s] + pi[s] - pi[k] + cost;
        if (cap && val < dist[k]) {
            dist[k] = val;
            dad[k] = make_pair(s, dir);
            width[k] = min(cap, width[s]);
        }
    }

    int64_t dijkstra(int s, int t) {
        fill(found.begin(), found.end(), false);
        fill(dist.begin(), dist.end(), INF);
        fill(width.begin(), width.end(), 0);
        dist[s] = 0;
        width[s] = INF;

        while (s != -1) {
            int best = -1;
            found[s] = true;
            for (int k = 0; k < N; k++) {
                if (found[k]) {
                    continue;
                }
                relax(s, k, edges[s][k].cap - edges[s][k].flow,
                      edges[s][k].cost, 1);
                relax(s, k, edges[k][s].flow, -edges[k][s].cost, -1);
                if (best == -1 || dist[k] < dist[best]) {
                    best = k;
                }
            }
            s = best;
        }

        for (int k = 0; k < N; k++) {
            pi[k] = min(pi[k] + dist[k], INF);
        }
        return width[t];
    }

    pair<int64_t, int64_t> GetMaxFlow(int s, int t) {
        int64_t totflow = 0, totcost = 0;
        while (int64_t amt = dijkstra(s, t)) {
            totflow += amt;
            for (int x = t; x != s; x = dad[x].first) {
                if (dad[x].second == 1) {
                    edges[dad[x].first][x].flow += amt;
                    totcost += amt * edges[dad[x].first][x].cost;
                } else {
                    edges[x][dad[x].first].flow -= amt;
                    totcost -= amt * edges[x][dad[x].first].cost;
                }
            }
        }
        return make_pair(totflow, totcost);
    }

    const vector<vector<Edge>>& getEdges() const {
        return edges;
    }
};

void solution(istream& iss) {
    int n;
    iss >> n;
    int m = 2 * n + 2;

    MinCostMaxFlow mcmf(m);

    for (int i = 0; i < n; i++) {
        mcmf.addEdge(0, i + 1, 1, 0);
        mcmf.addEdge(n + i + 1, 2 * n + 1, 1, 0);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int64_t w;
            iss >> w;
            mcmf.addEdge(i + 1, n + j + 1, INF, w);
        }
    }
    pair<int64_t, int64_t> res = mcmf.GetMaxFlow(0, m - 1);
    cout << res.second << "\n";
    const auto& edges = mcmf.getEdges();

    for (int i = 1; i < n + 1; i++) {
        for (int j = n; j < 2 * n + 1; ++j) {
            if (edges[i][j].flow > 0) std::cout << i << " " << j - n << "\n";
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr),
        std::cout.tie(nullptr);

    istringstream iss(
        "3\n"
        "3 2 1\n"
        "1 3 2\n"
        "2 1 3");

    solution(iss);

    return 0;
}

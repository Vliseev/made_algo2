//
// Created by user on 26.04.2020.
//
#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>
using namespace std;

struct FlowEdge {
    int v, u;
    long long cap, flow = 0;
    FlowEdge(int v, int u, long long cap) : v(v), u(u), cap(cap) {}
};

struct Dinic {
    const long long flow_inf = 1e18;
    vector<FlowEdge> edges;
    vector<vector<int>> adj;
    int n, m = 0;
    int s, t;
    vector<int> level, ptr;
    queue<int> q;

    Dinic(int n, int s, int t) : n(n), s(s), t(t) {
        adj.resize(n);
        level.resize(n);
        ptr.resize(n);
    }

    void add_edge(int v, int u, long long cap) {
        edges.emplace_back(v, u, cap);
        edges.emplace_back(u, v, 0);
        adj[v].push_back(m);
        adj[u].push_back(m + 1);
        m += 2;
    }

    bool bfs() {
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int id : adj[v]) {
                if (edges[id].cap - edges[id].flow < 1)
                    continue;
                if (level[edges[id].u] != -1)
                    continue;
                level[edges[id].u] = level[v] + 1;
                q.push(edges[id].u);
            }
        }
        return level[t] != -1;
    }

    long long dfs(int v, long long pushed, vector<int>& path) {
        if (pushed == 0)
            return 0;
        if (v == t)
            return pushed;
        for (int& cid = ptr[v]; cid < (int)adj[v].size(); cid++) {
            int id = adj[v][cid];
            int u = edges[id].u;
            if (level[v] + 1 != level[u] || edges[id].cap - edges[id].flow < 1)
                continue;
            long long tr =
                dfs(u, min(pushed, edges[id].cap - edges[id].flow), path);
            if (tr == 0)
                continue;
            edges[id].flow += tr;
            path.push_back(u + 1);
            edges[id ^ 1].flow -= tr;
            return tr;
        }
        return 0;
    }

    long long flow() {
        long long f = 0;

        vector<int> path[2];
        int id_path = 0;

        while (true) {
            fill(level.begin(), level.end(), -1);
            level[s] = 0;
            q.push(s);
            if (!bfs())
                break;
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, flow_inf, path[id_path])) {
                id_path += 1;
                if (id_path > 1)
                    break;
                f += pushed;
            }
        }

        if (path[0].size() && path[1].size()) {
            cout << "YES\n";

            if (path[0].size() > path[1].size())
                std::swap(path[1], path[0]);

            reverse(path[0].begin(), path[0].end());
            std::cout << 1 << " ";
            for (auto el : path[0]) {
                std::cout << el << " ";
            }
            cout << "\n";
            reverse(path[1].begin(), path[1].end());
            std::cout << 1 << " ";
            for (auto el : path[1]) {
                std::cout << el << " ";
            }

        } else {
            std::cout << "NO\n";
        }

        return f;
    }
};

void solution(istream& iss) {
    int n, m, s, t;
    iss >> n >> m >> s >> t;
    Dinic d(n, s - 1, t - 1);

    for (int i = 0; i < m; ++i) {
        int u, v;
        iss >> u >> v;
        if (u == v)
            continue;
        d.add_edge(u - 1, v - 1, 1);
    }

    d.flow();
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr),
        std::cout.tie(nullptr);

    istringstream iss(
        "3 3 1 3\n"
        "1 2\n"
        "1 3\n"
        "2 3");

    solution(iss);

    return 0;
}

#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

using namespace std;

static const int INF = std::numeric_limits<int>::max();
static const int64_t LONG_INF = std::numeric_limits<int64_t>::max();

struct Edge {
    int64_t cap{};
    int64_t flow{};
    Edge(int64_t cap)
        : cap(cap), flow(0) {}
};

class Dinic {
    vector<vector<Edge>> edges;
    std::vector<int64_t> vert_2_id{};
    int n, m = 0;
    int s, t;
    vector<int> parent;
    queue<int> q;
    vector<bool> visited{};

    bool bfs();
    int64_t dfs(int u, int64_t pushed);
    void dfsCut(int v);

 public:
    Dinic(int n, int s, int t);
    void addEdge(int u, int v, int64_t cap);
    int64_t GetMinCut();
    const vector<vector<Edge>> &getEdges() const {
        return edges;
    }
};
bool Dinic::bfs() {

    q.push(s);
    fill_n(parent.begin(), n, -1);
    std::fill(visited.begin(), visited.end(), false);
    visited[s] = true;

    while (!q.empty()) {
        auto u = q.front();
        q.pop();
        for (int v = 0; v < n; v++) {
            if (!visited[v] && (edges[u][v].cap > 0) && (edges[u][v].flow >= 0) && (edges[u][v].flow < edges[u][v].cap)) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return visited[t];
}
Dinic::Dinic(int n, int s, int t) : n(n), s(s), t(t), edges(n, vector<Edge>(n, Edge(-1))) {
    parent.resize(n);
    visited.resize(n);
    vert_2_id.resize(n * n);
}
int64_t Dinic::GetMinCut() {
    int64_t max_flow = 0;

    while (bfs()) {
        int64_t path_flow = std::numeric_limits<int64_t>::max();
        for (auto v = t; v != s; v = parent[v]) {
            auto u = parent[v];
            path_flow = min(path_flow, edges[u][v].cap - edges[u][v].flow);
        }
        for (auto v = t; v != s; v = parent[v]) {
            auto u = parent[v];
            edges[u][v].flow += path_flow;
            edges[v][u].flow -= path_flow;
        }
    }
    fill(visited.begin(), visited.end(), false);
    dfsCut(0);

    std::vector<int64_t> cut_edge;

    for (int64_t i = 0; i < n; i++) {
        for (int64_t j = 0; j < n; j++) {
            if (visited[i] && !visited[j] && (edges[i][j].cap > 0) && (edges[i][j].flow > 0)) {
                max_flow += edges[i][j].cap;
                cut_edge.emplace_back(vert_2_id[i * n + j] + 1);
            }
        }
    }
    std::cout << cut_edge.size() << " " << max_flow << "\n";
    std::sort(cut_edge.begin(), cut_edge.end());
    for (auto el : cut_edge) {
        std::cout << el << " ";
    }

    return max_flow;
}
void Dinic::addEdge(int u, int v, int64_t cap) {
    edges[u][v] = cap;
    edges[v][u] = cap;
    vert_2_id[u * n + v] = m;
    vert_2_id[v * n + u] = m;
    m++;
}
void Dinic::dfsCut(int v) {
    visited[v] = true;
    for (int64_t i = 0; i < n; i++) {
        if ((edges[v][i].cap > 0) && (edges[v][i].cap > edges[v][i].flow) && !visited[i])
            dfsCut(i);
    }
}

void solution(istream &iss) {
    int64_t n, m;
    iss >> n >> m;

    Dinic dinic(n, 0, n - 1);

    for (int64_t j = 0; j < m; j++) {
        int64_t a, b, c;
        iss >> a >> b >> c;
        dinic.addEdge(a - 1, b - 1, c);
    }

    dinic.GetMinCut();
}

int main() {

    std::ios::sync_with_stdio(false), std::cin.tie(nullptr),
        std::cout.tie(nullptr);
    istringstream iss(
        "3 3\n"
        "1 2 3\n"
        "1 3 5\n"
        "3 2 7");

    solution(std::cin);

    return 0;
}
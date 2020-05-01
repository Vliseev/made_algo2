#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

using namespace std;

static const int INF = std::numeric_limits<int>::max();
static const int64_t LONG_INF = std::numeric_limits<int64_t>::max();

struct Edge {
  int from{};
  int to{};
  int64_t cap{};
  int64_t flow{};
  Edge(int from, int to, int64_t cap)
      : from(from), to(to), cap(cap), flow(0) {}
};

class Dinic {
  vector<Edge> edges;
  vector<vector<int>> adj;
  int n, m = 0;
  int s, t;
  vector<int> d, ptr;
  queue<int> q;

  bool bfs();
  int64_t dfs(int u, int64_t pushed);

 public:
  Dinic(int n, int s, int t);
  void addEdge(int u, int v, int64_t cap);
  int64_t maxFlow();
  const vector<Edge> &getEdges() const {
    return edges;
  }
};
bool Dinic::bfs() {
  fill(d.begin(), d.end(), INF);
  d[s] = 0;
  q.push(s);

  while (!q.empty()) {
    auto u = q.front();
    q.pop();
    for (auto id : adj[u]) {
      auto v = edges[id].to;
      if ((edges[id].flow < edges[id].cap) && d[v] == INF) {
        d[v] = d[u] + 1;
        q.push(v);
      }
    }
  }
  return d[t] != INF;
}
Dinic::Dinic(int n, int s, int t) : n(n), s(s), t(t) {
  adj.resize(n);
  d.resize(n);
  ptr.resize(n);
}
int64_t Dinic::dfs(int u, int64_t pushed) {
  if ((u == t) || (pushed == 0)) {
    return pushed;
  }

  uint64_t flow = 0;
  for (auto i = ptr[u]; i < adj[u].size(); i++) {
    auto edge_id = adj[u][i];
    auto v = edges[edge_id].to;

    if (d[v] == d[u] + 1) {
      auto delta =
          dfs(v, min(pushed, edges[edge_id].cap - edges[edge_id].flow));
      if (delta) {
        edges[edge_id].flow += delta;
        edges[edge_id ^ 1].flow -= delta;
        flow += delta;
        pushed -= delta;
        if (pushed == 0) break;
      }
    }
    ptr[u]++;
  }
  return flow;
}
int64_t Dinic::maxFlow() {
  int64_t max_flow = 0;
  while (bfs()) {
    fill(ptr.begin(), ptr.end(), 0);
    auto flow = dfs(s, LONG_INF);
    while (flow) {
      max_flow += flow;
      flow = dfs(s, LONG_INF);
    }
  }

  return max_flow;
}
void Dinic::addEdge(int u, int v, int64_t cap) {
  edges.emplace_back(u, v, cap);
  edges.emplace_back(v, u, cap);
  adj[u].push_back(m++);
  adj[v].push_back(m++);
}

void solution(istream &iss) {
  int n, m;
  iss >> n;
  Dinic d(n, 0, n - 1);

  iss >> m;
  for (int i = 0; i < m; ++i) {
    int u, v;
    int64_t cap;
    iss >> u >> v >> cap;
    d.addEdge(u - 1, v - 1, cap);
  }

  std::cout << d.maxFlow();
  std::cout << "\n";
  for (int i = 0; i < d.getEdges().size(); i += 2) {
    std::cout << d.getEdges()[i].flow << "\n";
  }
}

int main() {
  std::ios::sync_with_stdio(false), std::cin.tie(nullptr),
      std::cout.tie(nullptr);

  istringstream iss(
      "2\n"
      "2\n"
      "1 2 1\n"
      "2 1 3");

  solution(std::cin);

  return 0;
}


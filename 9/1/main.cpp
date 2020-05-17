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
  int64_t cost{};
  Edge(int from, int to, int64_t cap, int64_t cost)
      : from(from), to(to), cap(cap), flow(0), cost(cost) {}
};

class BellmanFord {
  const vector<Edge> &edges;
  const vector<vector<int>> &adj;
  vector<int> p{};
  vector<int> p_idx{};
  vector<int64_t> d{};
  int n = 0;

 public:
  BellmanFord(const vector<Edge> &edges, const vector<vector<int>> &adj, int n) : edges(edges), adj(adj), n(n), p(n), d(n), p_idx(n) {}
  vector<int> negCycle(int s);
};

vector<int> BellmanFord::negCycle(int s) {
  vector<int> ans;
  ans.reserve(n);

  fill_n(p.begin(), n, -1);
  fill_n(p_idx.begin(), n, -1);
  fill_n(d.begin(), n, 0);

  d[s] = 0;
  for (int i = 0; i < n; ++i) {
    int e_id = 0;
    for (const auto &edge : edges) {
      if (edge.cap - edge.flow != 0) {
        auto w = edge.cost;
        if (d[edge.to] > d[edge.from] + w) {
          d[edge.to] = d[edge.from] + w;
          p[edge.to] = edge.from;
          p_idx[edge.to] = e_id;
        }
      }
      e_id++;
    }
  }
  for (const auto &edge : edges) {
    if (edge.cap - edge.flow != 0) {
      auto w = edge.cost;
      if (d[edge.to] > d[edge.from] + w) {
        int v = edge.to, u;
        for (int i = 0; i < n - 1; ++i) {
          v = p[v];
          if (v == -1)
            break;
        }
        if (v == -1)
          continue;
        u = v;
        v = p[v];
        ans.push_back(p_idx[u]);
        while (u != v) {
          ans.push_back(p_idx[v]);
          v = p[v];
        }
        reverse(ans.begin(), ans.end());
        break;
      }
    }
  }

  return ans;
}

class Dinic {
  vector<Edge> edges;
  vector<vector<int>> adj;
  int n, m = 0;
  int s, t;
  vector<int> d, ptr;
  queue<int> q;
  BellmanFord bf;

  bool bfs();
  int64_t dfs(int u, int64_t pushed);

 public:
  Dinic(int n, int s, int t);
  void addEdge(int u, int v, int64_t cap, int64_t cost);
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
Dinic::Dinic(int n, int s, int t) : n(n), s(s), t(t), bf(edges, adj, n) {
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
        return delta;
      }
    }
    ptr[u]++;
  }
  return 0;
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

  for (auto cyrcle = bf.negCycle(s); !cyrcle.empty(); cyrcle = bf.negCycle(s)) {
    auto min_cost = LONG_INF;
    for (int i = 0; i < cyrcle.size(); i++) {
      const auto &edge = cyrcle[i];
      auto cost = edges[edge].cap - edges[edge].flow;
      if (cost && cost < min_cost) {
        min_cost = cost;
      }
    }

    for (int i = 0; i < cyrcle.size(); i++) {
      const auto &edge = cyrcle[i];
      if (edges[edge].flow + min_cost <= edges[edge].cap) {
        edges[edge].flow += min_cost;
        edges[edge ^ 1].flow -= min_cost;
      }
    }
  }

  int64_t min_cost_flow = 0;
  for (const auto &e : edges) {
    min_cost_flow += e.flow > 0 ? e.flow * e.cost : 0;
  }

  return min_cost_flow;
}
void Dinic::addEdge(int u, int v, int64_t cap, int64_t cost) {
  edges.emplace_back(u, v, cap, cost);
  edges.emplace_back(v, u, 0, -cost);
  adj[u].push_back(m++);
  adj[v].push_back(m++);
}

void solution(istream &iss) {
  int n, m;
  iss >> n >> m;
  Dinic d(n, 0, n - 1);

  for (int i = 0; i < m; ++i) {
    int u, v;
    int64_t cap, w;
    iss >> u >> v >> cap >> w;
    d.addEdge(u - 1, v - 1, cap, w);
  }

  std::cout << d.maxFlow();
}

int main() {
  std::ios::sync_with_stdio(false), std::cin.tie(nullptr),
      std::cout.tie(nullptr);

  istringstream iss(
      "4 5\n"
      "1 2 1 2\n"
      "1 3 2 2\n"
      "3 2 1 1\n"
      "2 4 2 1\n"
      "3 4 2 3");

  solution(cin);

  return 0;
}

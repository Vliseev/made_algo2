//TODO: not work

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
  vector<bool> visited{};
  std::vector<int64_t> vert_2_id{};

  bool bfs();
  int64_t dfs(int u, int64_t pushed);
  void dfsCut(int v);

 public:
  Dinic(int n, int s, int t);
  void addEdge(int u, int v, int64_t cap);
  int64_t GetMinCut();
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
  visited.resize(n);
  vert_2_id.resize(n * n);
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
int64_t Dinic::GetMinCut() {
  int64_t max_flow = 0;
  while (bfs()) {
    fill(ptr.begin(), ptr.end(), 0);
    auto flow = dfs(s, LONG_INF);
    while (flow) {
      max_flow += flow;
      flow = dfs(s, LONG_INF);
    }
  }
  fill(visited.begin(), visited.end(), false);
  dfsCut(0);

  std::vector<int64_t> cut_edge;

  for (int64_t i = 0; i < n; i++) {
    for (int64_t j = 0; j < adj[i].size(); j++) {
      auto edge_idx = adj[i][j];
      const auto &edge = edges[edge_idx];
      if (visited[edge.from] && !visited[edge.to] && (edge.flow > 0))
        cut_edge.push_back(vert_2_id[edge.from * n + edge.to] + 1);
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
  vert_2_id[u * n + v] = m / 2;
  vert_2_id[v * n + u] = m / 2;
  edges.emplace_back(u, v, cap);
  edges.emplace_back(v, u, cap);
  adj[u].push_back(m++);
  adj[v].push_back(m++);
}
void Dinic::dfsCut(int v) {
  visited[v] = true;
  for (int i = 0; i < adj[v].size(); ++i) {
    auto adj_idx = adj[v][i];
    const auto &edge = edges[adj_idx];
    if (edge.cap > edge.flow && edge.flow > 0) {
      dfsCut(edges[adj_idx].to);
    }
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

  solution(iss);

  return 0;
}
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
  vector<int> d, ptr;
  queue<int> q;
  vector<bool> visited{};

  bool bfs();
  int64_t dfs(int u, int64_t pushed);

 public:
  Dinic(int n, int s, int t);
  void addEdge(int u, int v, int64_t cap);
  int64_t GetMinCut();
  const vector<vector<Edge>> &getEdges() const {
    return edges;
  }
};

int64_t Dinic::dfs(int u, int64_t pushed) {
  if ((u == t) || (pushed == 0)) {
    return pushed;
  }

  uint64_t flow = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; ++j) {
      if (d[j] == d[i] + 1) {
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
}

bool Dinic::bfs() {

  q.push(s);
  fill(d.begin(), d.end(), INF);
  d[s] = 0;
  q.push(s);

  while (!q.empty()) {
    auto u = q.front();
    q.pop();
    for (int v = 0; v < n; v++) {
      if (!visited[v] && (edges[u][v].flow < edges[u][v].cap)) {
        q.push(v);
        parent[v] = u;
        visited[v] = true;
      }
    }
  }
  return visited[t];
}

Dinic::Dinic(int n, int s, int t) : n(n), s(s), t(t), edges(n, vector<Edge>(n, Edge(-1))) {
  d.resize(n);
  ptr.resize(n);
  vert_2_id.resize(n * n);
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

  return max_flow;
}
void Dinic::addEdge(int u, int v, int64_t cap) {
  edges[u][v] = cap;
  edges[v][u] = cap;
  vert_2_id[u * n + v] = m;
  vert_2_id[v * n + u] = m;
  m++;
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
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using WeightedEdge = std::pair<int, int>;

template <typename T>
using Vofv = std::vector<std::vector<T>>;

template <class Iterator, class T, class Compare>
Iterator myBinSearch(Iterator first, Iterator last, const T& value,
                     Compare comp) {
    const auto kPotentialMatch(std::lower_bound(first, last, value, comp));

    if (kPotentialMatch != last && kPotentialMatch->first == value) {
        return kPotentialMatch;
    }
    return last;
}

class Tree {
    Vofv<WeightedEdge> edges_{};
    std::vector<int> parent_{};
    int n_{};

 public:
    Tree() = default;
    Tree(int n) : edges_(n), parent_(n, 0), n_(n){};
    void addEdge(int i, int j, int w);
    const std::vector<WeightedEdge>& getChildren(int i) const;
    std::vector<WeightedEdge>& getChildren(int i);
    WeightedEdge getEdge(int i, int j) const;

    int getPar(int i) const;
    int getN() const;
};

int getWeight(const Tree& tree, int vert);
void Tree::addEdge(int i, int j, int w) {
    edges_[i].emplace_back(j, w);
    parent_[j] = i;
}

const std::vector<WeightedEdge>& Tree::getChildren(int i) const {
    return edges_[i];
}

std::vector<WeightedEdge>& Tree::getChildren(int i) {
    return edges_[i];
}
int Tree::getPar(int i) const {
    return parent_[i];
}
int Tree::getN() const {
    return n_;
}
WeightedEdge Tree::getEdge(int i, int j) const {
    return edges_[i][j];
}

void dfs(const Tree& tree, std::vector<int>& d, int vertex, int depth) {
    if (vertex == 0 && depth)
        return;
    d[vertex] = depth;
    for (auto x : tree.getChildren(vertex)) {
        dfs(tree, d, x.first, depth + 1);
    }
}

void precalc(Vofv<int>& jmp, Vofv<int>& min_jmp, const Tree& tree, int log_n) {
    int n = tree.getN();

    for (int i = 0; i < n; ++i) {
        jmp[i][0] = tree.getPar(i);
        if (i != 0) {
            auto w = getWeight(tree, i);
            min_jmp[i][0] = w;
        }
    }
    for (int k = 1; k <= log_n; ++k) {
        for (int v = 0; v < n; ++v) {
            auto u = jmp[v][k - 1];
            jmp[v][k] = jmp[u][k - 1];
            min_jmp[v][k] = std::min(min_jmp[v][k - 1], min_jmp[u][k - 1]);
        }
    }
}
int getWeight(const Tree& tree, int vert) {
    auto par = tree.getPar(vert);

    const auto& par_child = tree.getChildren(par);
    auto it = myBinSearch(par_child.begin(), par_child.end(), vert,
                          [](auto item, auto val) { return item.first < val; });

    return tree.getEdge(par, it - par_child.begin()).second;
}

int lca(int u, int v, Vofv<int>& jmp, Vofv<int>& min_jmp,
        const std::vector<int>& d, int log_n) {
    if (d[u] < d[v]) {
        std::swap(u, v);
    }
    int delta = d[u] - d[v];

    auto min = min_jmp[u][0];

    for (int k = log_n; k >= 0; k--) {
        if (delta >= (1 << k)) {
            min = std::min(min, min_jmp[u][k]);
            u = jmp[u][k];
            delta -= (1 << k);
        }
    }
    if (u == v)
        return min;
    int u_hat, v_hat;
    min = std::min(min_jmp[v][0], min);
    for (int k = log_n; k >= 0; k--) {
        u_hat = jmp[u][k];
        v_hat = jmp[v][k];
        if (u_hat != v_hat) {
            min = std::min(min, min_jmp[u][k]);
            u = u_hat;
            min = std::min(min, min_jmp[v][k]);
            v = v_hat;
        }
    }
    min = std::min(min, min_jmp[v][0]);
    return std::min(min, min_jmp[u][0]);
}

int findMin(int u, int v, int lca_v, const Tree& tree) {
    int min = std::numeric_limits<int>::max();

    int cur_v = u;
    while (cur_v != lca_v) {
        auto par = tree.getPar(cur_v);
        min = std::min(getWeight(tree, cur_v), min);
        cur_v = par;
    }

    cur_v = v;
    while (cur_v != lca_v) {
        auto par = tree.getPar(cur_v);
        min = std::min(getWeight(tree, cur_v), min);
        cur_v = par;
    }

    return min;
}

void solution(std::istream& iss, std::ostream& out) {
    int n;
    iss >> n;

    Tree tree(n);
    for (int i = 1; i < n; ++i) {
        int v, w;
        iss >> v >> w;
        tree.addEdge(v - 1, i, w);
    }

    std::vector<int> d(n);
    dfs(tree, d, 0, 0);

    int log_n = 1;
    while ((1 << log_n) < n) {
        log_n++;
    }

    Vofv<int> jmp(n, std::vector<int>(log_n + 1, 0));
    Vofv<int> min_jmp(
        n, std::vector<int>(log_n + 1, std::numeric_limits<int>::max()));

    precalc(jmp, min_jmp, tree, log_n);

    int m;
    iss >> m;
    for (int j = 0; j < m; ++j) {
        int u, v;
        iss >> u >> v;
        auto min = lca(u - 1, v - 1, jmp, min_jmp, d, log_n);
        out << min << "\n";
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr),
        std::cout.tie(nullptr);
    /*std::istringstream s(
        "5\n"
        "1 1\n"
        "1 2\n"
        "2 3\n"
        "3 4\n"
        "2\n"
        "1 4\n"
        "3 2");

    solution(s);*/
    std::ifstream iss("minonpath.in");
    std::ofstream oss("minonpath.out");

    solution(iss, oss);

    return 0;
}
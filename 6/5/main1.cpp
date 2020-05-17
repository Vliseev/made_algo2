#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using WeightedEdge = std::pair<int, int>;
class Tree {
    std::vector<std::vector<WeightedEdge>> edges_{};
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

void precalc(std::vector<std::vector<int>>& jmp, const Tree& tree, int log_n) {
    int n = tree.getN();

    for (int i = 0; i < n; ++i) {
        jmp[i][0] = tree.getPar(i);
    }
    for (int k = 1; k < log_n; ++k) {
        for (int v = 0; v < n; ++v) {
            jmp[v][k] = jmp[jmp[v][k - 1]][k - 1];
        }
    }
}

int lca(int u, int v, std::vector<std::vector<int>>& jmp,
        const std::vector<int>& d, int log_n) {
    if (d[u] < d[v]) {
        std::swap(u, v);
    }
    int delta = d[u] - d[v];

    for (int k = log_n; k >= 0; k--) {
        if (delta >= (1 << k)) {
            u = jmp[u][k];
            delta -= (1 << k);
        }
    }
    if (u == v)
        return u;
    int u_hat, v_hat;
    for (int k = log_n; k >= 0; k--) {
        u_hat = jmp[u][k];
        v_hat = jmp[v][k];
        if (u_hat != v_hat) {
            u = u_hat;
            v = v_hat;
        }
    }
    return jmp[u][0];
}

template <class Iterator, class T, class Compare>
Iterator myBinSearch(Iterator first, Iterator last, const T& value,
                     Compare comp) {
    const auto kPotentialMatch(std::lower_bound(first, last, value, comp));

    if (kPotentialMatch != last) {
        return kPotentialMatch;
    }
    return last;
}

int findMin(int u, int v, int lca_v, const Tree& tree) {
    int min = std::numeric_limits<int>::max();

    int cur_v = u;
    while (cur_v != lca_v) {
        int par = tree.getPar(cur_v);

        const auto& par_child = tree.getChildren(par);
        auto it =
            myBinSearch(par_child.begin(), par_child.end(), cur_v,
                        [](auto item, auto val) { return item.first < val; });

        min = std::min(tree.getEdge(par, it - par_child.begin()).second, min);
        cur_v = par;
    }

    cur_v = v;
    while (cur_v != lca_v) {
        int par = tree.getPar(cur_v);

        const auto& par_child = tree.getChildren(par);
        auto it =
            myBinSearch(par_child.begin(), par_child.end(), cur_v,
                        [](auto item, auto val) { return item.first < val; });

        min = std::min(tree.getEdge(par, it - par_child.begin()).second, min);
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

    std::vector<std::vector<int>> jmp(n, std::vector<int>(log_n + 1, 0));

    precalc(jmp, tree, log_n);

    int m;
    iss >> m;
    for (int j = 0; j < m; ++j) {
        int u, v;
        iss >> u >> v;
        auto lca_v = lca(u - 1, v - 1, jmp, d, log_n);
        out << findMin(u - 1, v - 1, lca_v, tree) << "\n";
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr),
        std::cout.tie(nullptr);

    std::ifstream iss("minonpath.in");
    std::ofstream oss("minonpath1.out");

    solution(iss, oss);

    return 0;
}
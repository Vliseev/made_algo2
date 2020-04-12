#include <iostream>
#include <sstream>
#include <vector>

class Tree {
    std::vector<std::vector<int>> edges_{};
    std::vector<int> parent_{};
    int n_{};

 public:
    Tree() = default;
    Tree(int n) : edges_(n), parent_(n, 0), n_(n){};
    void addEdge(int i, int j);
    const std::vector<int>& getChildren(int i) const;
    std::vector<int>& getChildren(int i);

    int getPar(int i) const;
    int getN() const;
};

void Tree::addEdge(int i, int j) {
    edges_[i].push_back(j);
    parent_[j] = i;
}

const std::vector<int>& Tree::getChildren(int i) const {
    return edges_[i];
}

std::vector<int>& Tree::getChildren(int i) {
    return edges_[i];
}
int Tree::getPar(int i) const {
    return parent_[i];
}
int Tree::getN() const {
    return n_;
}

void dfs(const Tree& tree, std::vector<int>& d, int vertex, int depth) {
    if (vertex == 0 && depth)
        return;
    d[vertex] = depth;
    for (auto x : tree.getChildren(vertex)) {
        dfs(tree, d, x, depth + 1);
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

void solution(std::istream& iss) {
    int n;
    iss >> n;

    Tree tree(n);
    for (int i = 1; i < n; ++i) {
        int v;
        iss >> v;
        tree.addEdge(v - 1, i);
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
        std::cout << lca(u - 1, v - 1, jmp, d, log_n) + 1 << "\n";
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr),
        std::cout.tie(nullptr);
    /* std::istringstream s(
         "5\n"
         "1\n"
         "1\n"
         "2\n"
         "2\n"
         "3\n"
         "4 5\n"
         "4 2\n"
         "3 5");

     solution(s);*/
    solution(std::cin);

    return 0;
}
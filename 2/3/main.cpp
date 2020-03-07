#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

struct Tree {
    std::vector<std::vector<int>> tree;

    explicit Tree(int n) : tree(n){};
    void AddChildren(int i, int j) {
        tree[i].push_back(j);
    }

    std::vector<int>& GetChildren(int i) {
        return tree[i];
    }

    const std::vector<int>& GetChildren(int i) const {
        return tree.at(i);
    }
};

class SolutionHelper {
    const Tree& tree_;
    int n_;
    std::vector<int> liis_size_;
    int RecLiis(int vert);

   public:
    explicit SolutionHelper(const Tree& tree)
        : tree_(tree), n_(tree.tree.size()), liis_size_(tree.tree.size(), -1) {}
    int GetSolution(int root);
};

int SolutionHelper::GetSolution(int root) {
    return RecLiis(root);
}
int SolutionHelper::RecLiis(int vert) {
    if (vert == 0) return 0;
    if (liis_size_[vert] != -1) {
        return liis_size_[vert];
    }
    int contain_size = 0;
    int n_contain_size = 1;

    for (auto child : tree_.GetChildren(vert)) {
        contain_size += RecLiis(child);
    }

    for (auto child : tree_.GetChildren(vert)) {
        for (auto g_ch : tree_.GetChildren(child)) {
            n_contain_size += RecLiis(g_ch);
        }
    }

    auto max_val = std::max(contain_size, n_contain_size);
    liis_size_[vert] = max_val;
    return max_val;
}

void Solution(std::istream& iss) {
    int n, root = 0;
    iss >> n;
    Tree tree(n + 1);
    for (int i = 1; i <= n; ++i) {
        int parent;
        iss >> parent;
        if (parent == 0) root = i;
        tree.AddChildren(parent, i);
    }
    SolutionHelper sh(tree);
    std::cout << sh.GetSolution(root);
}

int main() {
    std::istringstream iss(
        "5\n"
        "0\n"
        "1\n"
        "1\n"
        "2\n"
        "3");
    Solution(iss);

    return 0;
}

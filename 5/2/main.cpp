#include <iostream>
#include <sstream>
#include <vector>

class SegmentTree {
    std::vector<int64_t > t_{};
    int x;

    int64_t Rsq(int id, int l, int r, int a, int b);

 public:
    SegmentTree(const std::vector<int64_t> &v) noexcept;
    void Set(int i, int val);
    int64_t Rsq(int a, int b);
};
SegmentTree::SegmentTree(const std::vector<int64_t > &v) noexcept {
    x = 1;

    while (x < v.size()) {
        x *= 2;
    }
    t_ = std::vector<int64_t>(2 * x, 0);

    for (int i = 0; i < v.size(); i++) {
        t_[i + x - 1] = v[i];
    }

    for (int i = x - 2; i >= 0; i--) {
        t_[i] = t_[2 * i + 1] + t_[2 * i + 2];
    }
}
void SegmentTree::Set(int i, int val) {
    int n = t_.size() / 2;
    i += n - 1;
    t_[i] = val;
    while (i) {
        i = (i - 1) / 2;
        t_[i] = t_[2 * i + 1] + t_[2 * i + 2];
    }
}

int64_t SegmentTree::Rsq(int a, int b) {
    return Rsq(0, 0, x - 1, a, b);
}
int64_t SegmentTree::Rsq(int id, int l, int r, int a, int b) {
    if (l > b || r < a)
        return 0;
    if (l >= a && r <= b) {
        return t_[id];
    }

    int mid = (l + r) / 2;

    return Rsq(id * 2 + 1, l, mid, a, b) + Rsq(id * 2 + 2, mid + 1, r, a, b);
}

void solution(std::istream &iss) {
    int n;
    iss >> n;

    std::vector<int64_t> v(n, 0);
    for (int i = 0; i < n; ++i) {
        iss >> v[i];
    }

    SegmentTree segment_tree(v);

    std::string query_type;
    int l, r;
    while (!iss.eof()) {
        iss >> query_type;
        if (iss.eof())
            break;
        iss >> l >> r;
        if (query_type == "sum") {
            std::cout << segment_tree.Rsq(l - 1, r - 1) << "\n";
        } else {
            segment_tree.Set(l - 1, r);
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    solution(std::cin);

    return 0;
}

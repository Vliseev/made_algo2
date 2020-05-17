#include <iostream>
#include <sstream>
#include <vector>

/*
 * https://cp-algorithms.com/data_structures/segment_tree.html
 * https://acm.khpnets.info/w/index.php?title=%D0%94%D0%B5%D1%80%D0%B5%D0%B2%D0%BE_%D0%BE%D1%82%D1%80%D0%B5%D0%B7%D0%BA%D0%BE%D0%B2
 */

class SegmentTree {
    std::vector<int> t_{};

    std::vector<int> update_add_{};

    std::vector<int> update_set_{};
    std::vector<bool> set_mask_{};
    int n_vert_;

    void pushSet(int v, int l, int r);
    void set(int v, int l, int r, int a, int b, int x);

    void pushAdd(int v, int l, int r);
    void add(int v, int l, int r, int a, int b, int x);

    int rmq(int v, int l, int r, int a, int b);
    int get(int v, int l, int r);

 public:
    SegmentTree(const std::vector<int> &v) noexcept;
    void set(int a, int b, int val);
    void add(int a, int b, int val);
    int rmq(int a, int b);
};

SegmentTree::SegmentTree(const std::vector<int> &v) noexcept {
    n_vert_ = 1;

    while (n_vert_ < v.size()) {
        n_vert_ *= 2;
    }
    t_ = std::vector<int>(2 * n_vert_, std::numeric_limits<int>::max());

    update_set_ = std::vector<int>(2 * n_vert_, 0);
    set_mask_ = std::vector<bool>(2 * n_vert_, false);

    update_add_ = std::vector<int>(2 * n_vert_, 0);

    for (int i = 0; i < v.size(); i++) {
        t_[i + n_vert_ - 1] = v[i];
    }

    for (int i = n_vert_ - 2; i >= 0; i--) {
        t_[i] = std::min(t_[2 * i + 1], t_[2 * i + 2]);
    }
}

void SegmentTree::set(int a, int b, int val) {
    set(0, 0, n_vert_ - 1, a, b, val);
}

void SegmentTree::add(int a, int b, int val) {
    add(0, 0, n_vert_ - 1, a, b, val);
}

void SegmentTree::pushAdd(int v, int l, int r) {
    if (update_add_[v] != 0) {
        t_[v] += update_add_[v] * (r - l + 1);
        if (r != l) {
            update_add_[2 * v + 1] += update_add_[v];
            update_add_[2 * v + 2] += update_add_[v];
        }
    }
    update_add_[v] = 0;
}

void SegmentTree::add(int v, int l, int r, int a, int b, int x) {
    if (l > b || r < a) {
        return;
    }
    pushAdd(v, l, r);
    pushSet(v, l, r);
    if (a <= l && r <= b) {
        update_add_[v] += x;
    } else {
        auto m = (l + r) / 2;
        add(2 * v + 1, l, m, a, b, x);
        add(2 * v + 2, m + 1, r, a, b, x);
        t_[v] = std::min(get(2 * v + 1, l, m), get(2 * v + 2, m + 1, r));
    }
}

void SegmentTree::pushSet(int v, int l, int r) {
    if (set_mask_[v]) {
        t_[v] = update_set_[v];
        update_add_[v] = 0;
        if (l != r) {
            update_set_[2 * v + 1] = update_set_[2 * v + 2] = update_set_[v];
            set_mask_[2 * v + 1] = set_mask_[2 * v + 2] = true;
            update_add_[2 * v + 1] = update_add_[2 * v + 2] = 0;
        }
    }
    update_set_[v] = 0;
    set_mask_[v] = false;
}

void SegmentTree::set(int v, int l, int r, int a, int b, int x) {
    if (l > b || r < a) {
        return;
    }
    pushAdd(v, l, r);
    pushSet(v, l, r);
    if (a <= l && r <= b) {
        set_mask_[v] = true;
        update_set_[v] = x;
        update_add_[v] = 0;
    } else {
        auto m = (l + r) / 2;
        set(2 * v + 1, l, m, a, b, x);
        set(2 * v + 2, m + 1, r, a, b, x);
        t_[v] = std::min(get(2 * v + 1, l, m), get(2 * v + 2, m + 1, r));
    }
}

int SegmentTree::rmq(int v, int l, int r, int a, int b) {
    if (l > b || r < a) {
        return std::numeric_limits<int>::max();
    }
    pushAdd(v, l, r);
    pushSet(v, l, r);

    if (l >= a && r <= b) {
        return get(v, l, r);
    }

    auto m = (l + r) / 2;

    auto l_min = rmq(2 * v + 1, l, m, a, b);
    auto r_min = rmq(2 * v + 2, m + 1, r, a, b);

    return std::min(l_min, r_min);
}

int SegmentTree::rmq(int a, int b) {
    return rmq(0, 0, n_vert_ - 1, a, b);
}
int SegmentTree::get(int v, int l, int r) {
    if (set_mask_[v])
        return update_set_[v];
    return t_[v] + update_add_[v];
}

void solution(std::istream &iss) {
    int n;
    iss >> n;

    std::vector<int> v(n, 0);
    for (int i = 0; i < n; ++i) {
        iss >> v[i];
    }

    SegmentTree segment_tree(v);

    std::string query_type;
    int l, r, val;
    while (!iss.eof()) {
        iss >> query_type;
        if (iss.eof())
            break;
        if (query_type == "set") {
            iss >> l >> r >> val;
            segment_tree.set(l - 1, r - 1, val);
        } else if (query_type == "add") {
            iss >> l >> r >> val;
            segment_tree.add(l - 1, r - 1, val);
        } else {
            iss >> l >> r;
            std::cout << segment_tree.rmq(l - 1, r - 1) << "\n";
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    //    std::istringstream iss(
    //        "5\n"
    //        "4 6 1 4 6\n"
    //        "set 2 3 5\n"
    //        "add 1 2 2\n"
    //        "add 2 2 -5\n"
    //        "min 2 5");
    solution(std::cin);
    //     solution(iss);

    return 0;
}

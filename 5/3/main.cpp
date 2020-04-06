#include <iostream>
#include <sstream>
#include <vector>

class SegmentTree {
    std::vector<int64_t> t_{};

    std::vector<int64_t> update_add_{};

    std::vector<int64_t> update_set_{};
    std::vector<bool> set_mask_{};
    int64_t n_vert_;

    void set(int64_t v, int64_t l, int64_t r, int64_t a, int64_t b, int64_t x);

    void push(int64_t v, int64_t l, int64_t r);
    void add(int64_t v, int64_t l, int64_t r, int64_t a, int64_t b, int64_t x);

    int64_t rmq(int64_t v, int64_t l, int64_t r, int64_t a, int64_t b);
    int64_t get(int64_t v);

 public:
    SegmentTree(const std::vector<int64_t> &v);
    void set(int64_t a, int64_t b, int64_t val);
    void add(int64_t a, int64_t b, int64_t val);
    int64_t rmq(int64_t a, int64_t b);
};

SegmentTree::SegmentTree(const std::vector<int64_t> &v) {
    n_vert_ = 1;

    while (n_vert_ < v.size()) {
        n_vert_ *= 2;
    }
    t_ = std::vector<int64_t>(2 * n_vert_, std::numeric_limits<int64_t>::max());

    update_set_ = std::vector<int64_t>(2 * n_vert_, 0);
    set_mask_ = std::vector<bool>(2 * n_vert_, false);

    update_add_ = std::vector<int64_t>(2 * n_vert_, 0);

    for (int64_t i = 0; i < v.size(); i++) {
        t_[i + n_vert_ - 1] = v[i];
    }

    for (int64_t i = n_vert_ - 2; i >= 0; i--) {
        t_[i] = std::min(t_[2 * i + 1], t_[2 * i + 2]);
    }
}

void SegmentTree::set(int64_t a, int64_t b, int64_t val) {
    set(0, 0, n_vert_ - 1, a, b, val);
}

void SegmentTree::add(int64_t a, int64_t b, int64_t val) {
    add(0, 0, n_vert_ - 1, a, b, val);
}

void SegmentTree::push(int64_t v, int64_t l, int64_t r) {
    if (!set_mask_[v]) {
        if (l == r) {
            t_[v] += update_add_[v];
        } else {
            if (set_mask_[2 * v + 1]) {
                update_set_[2 * v + 1] += update_add_[v];
            } else {
                update_add_[2 * v + 1] += update_add_[v];
            }
            if (set_mask_[2 * v + 2]) {
                update_set_[2 * v + 2] += update_add_[v];
            } else {
                update_add_[2 * v + 2] += update_add_[v];
            }
            t_[v] = std::min(get(2 * v + 1), get(2 * v + 2));
        }

    } else {
        if (l == r) {
            t_[v] = update_set_[v];
        } else {
            update_set_[2 * v + 1] = update_set_[2 * v + 2] = update_set_[v];
            set_mask_[2 * v + 1] = set_mask_[2 * v + 2] = true;
            update_add_[2 * v + 1] = update_add_[2 * v + 2] = 0;
            t_[v] = std::min(get(2 * v + 1), get(2 * v + 2));
        }
    }
    update_add_[v] = 0;
    update_set_[v] = 0;
    set_mask_[v] = false;
}

void SegmentTree::add(int64_t v, int64_t l, int64_t r, int64_t a, int64_t b,
                      int64_t x) {
    if (l > b || r < a) {
        return;
    }
    push(v, l, r);
    if (a <= l && r <= b) {
        if (!set_mask_[v]) {
            update_add_[v] += x;
        } else {
            update_set_[v] += x;
        }
    } else {
        auto m = (l + r) / 2;
        add(2 * v + 1, l, m, a, b, x);
        add(2 * v + 2, m + 1, r, a, b, x);
        t_[v] = std::min(get(2 * v + 1), get(2 * v + 2));
    }
}

void SegmentTree::set(int64_t v, int64_t l, int64_t r, int64_t a, int64_t b,
                      int64_t x) {
    if (l > b || r < a) {
        return;
    }
    push(v, l, r);

    if (a <= l && r <= b) {
        if (!set_mask_[v]) {
            set_mask_[v] = true;
            update_add_[v] = 0;
        }
        update_set_[v] = x;

    } else {
        auto m = (l + r) / 2;
        set(2 * v + 1, l, m, a, b, x);
        set(2 * v + 2, m + 1, r, a, b, x);
        t_[v] = std::min(get(2 * v + 1), get(2 * v + 2));
    }
}

int64_t SegmentTree::rmq(int64_t v, int64_t l, int64_t r, int64_t a,
                         int64_t b) {
    if (l > b || r < a) {
        return std::numeric_limits<int64_t>::max();
    }
    push(v, l, r);

    if (l >= a && r <= b) {
        return get(v);
    }

    auto m = (l + r) / 2;

    auto l_min = rmq(2 * v + 1, l, m, a, b);
    auto r_min = rmq(2 * v + 2, m + 1, r, a, b);

    return std::min(l_min, r_min);
}

int64_t SegmentTree::rmq(int64_t a, int64_t b) {
    return rmq(0, 0, n_vert_ - 1, a, b);
}
int64_t SegmentTree::get(int64_t v) {
    if (set_mask_[v])
        return update_set_[v];
    return t_[v] + update_add_[v];
}

void solution(std::istream &iss) {
    int64_t n;
    iss >> n;

    std::vector<int64_t> v(n, 0);
    for (int64_t i = 0; i < n; ++i) {
        iss >> v[i];
    }

    SegmentTree segment_tree(v);

    std::string query_type;
    int64_t l, r, val;
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
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr),
        std::cout.tie(nullptr);
    solution(std::cin);

    return 0;
}

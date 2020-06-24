//
// Created by user on 24.06.2020.
//
#include <bits/stdc++.h>

using namespace std;
static constexpr int64_t MAX_N = 1e5;
static constexpr int64_t mod = 1e9;

int64_t getAi(int64_t a, int64_t l, int64_t m) {
    int64_t a_i = (a * l) % mod;
    a_i += m;
    a_i %= mod;
    return a_i;
}

int64_t n_el(vector<int64_t> v, int64_t a, int64_t b, int64_t k) {
    sort(v.begin() + a, v.begin() + b);
    return v[a + k - 1];
}

class SegmentTree {
 public:
  struct Node;
  using NodePtr = Node *;
  struct Node {
    int64_t val{};
    NodePtr left = nullptr;
    NodePtr right = nullptr;
    Node(int64_t val, NodePtr left = nullptr, NodePtr right = nullptr) : val(val), left(left), right(right) {};
    Node() = default;
  };

  SegmentTree(int64_t N);

  void update(int64_t ver, int64_t pos, int64_t v);
  void update(NodePtr &node, int64_t l, int64_t r, int64_t pos, int64_t v);
  int64_t query(NodePtr &node, int64_t l, int64_t r, int64_t a, int64_t b);
  int64_t query(int64_t version, int64_t l, int64_t r);

  const vector<NodePtr> &getTreeVersion() const {
      return tree_version;
  }

 private:
  void build(NodePtr &node, int64_t l, int64_t r);
  vector<NodePtr> tree_version{};
  int64_t cur_ver = 1;
  int64_t n = 0;
};

SegmentTree::SegmentTree(int64_t N) : tree_version(N + 1), n(N - 1) {
    tree_version[0] = new Node();
    build(tree_version[0], 0, N - 1);
}
void SegmentTree::build(SegmentTree::NodePtr &node, int64_t l, int64_t r) {
    if (l == r) {
        node->val = 0;
        node->left = node->right = nullptr;
        return;
    }
    int64_t mid = (l + r) / 2;
    node->left = new Node();
    build(node->left, l, mid);
    node->right = new Node();
    build(node->right, mid + 1, r);
}
void SegmentTree::update(SegmentTree::NodePtr &node, int64_t l, int64_t r, int64_t pos, int64_t v) {
    if ((pos > r) or (pos < l) or (l > r)) {
        return;
    }
    if (l == r) {
        node->val = v;
        return;
    }
    int64_t mid = (l + r) / 2;
    if (pos <= mid) {
        auto left = new Node();
        *left = *node->left;
        node->left = left;
        update(left, l, mid, pos, v);
    } else {
        auto right = new Node();
        *right = *node->right;
        node->right = right;
        update(right, mid + 1, r, pos, v);
    }
    node->val = node->left->val + node->right->val;
}
void SegmentTree::update(int64_t ver, int64_t pos, int64_t v) {
    auto node = new Node();
    *node = *tree_version[ver - 1];
    tree_version[cur_ver] = node;
    update(tree_version[cur_ver], 0, n, pos, v);
    cur_ver++;
}

int64_t SegmentTree::query(SegmentTree::NodePtr &node, int64_t l, int64_t r, int64_t a, int64_t b) {
    if ((l > b) or (r < a)) {
        return 0;
    }
    if ((l >= a) and (r <= b)) {
        return node->val;
    }
    int64_t mid = (l + r) / 2;
    int64_t q1 = query(node->left, l, mid, a, b);
    int64_t q2 = query(node->right, mid + 1, r, a, b);
    return q1 + q2;

}
int64_t SegmentTree::query(int64_t version, int64_t l, int64_t r) {
    auto &node = tree_version[version];
    return query(node, 0, n, l, r);
}

struct QueryStruct {
  vector<int64_t> id2val{};
  vector<int64_t> &v;
  SegmentTree tree;
  QueryStruct(int64_t N, vector<int64_t> &v);
};

QueryStruct::QueryStruct(int64_t N, vector<int64_t> &v) : tree(N), v(v), id2val(N) {
    vector<pair<int64_t, int64_t>> tmp_vec(N);
    for (int64_t i = 0; i < N; ++i) {
        tmp_vec[i] = {v[i], i};
    }
    sort(tmp_vec.begin(), tmp_vec.end(), [](auto &v1, auto &v2) {
      return v1.first < v2.first;
    });

    for (int64_t i = 0; i < N; i++) {
        id2val[i] = tmp_vec[i].first;
        v[tmp_vec[i].second] = i;
    }
    tmp_vec = {{0, 0}};

    for (int64_t i = 0; i < N; ++i) {
        tree.update(i + 1, v[i], 1);
    }
}

int64_t query(QueryStruct &qs, int64_t a, int64_t b, int64_t k) {
    const auto &tree_v = qs.tree.getTreeVersion();
    auto node_r = tree_v[b];
    auto node_l = tree_v[a];
    int64_t l = 0;
    int64_t r = qs.v.size() - 1;

    while (l != r) {
        int64_t mid = (l + r) / 2;
        auto q1 = qs.tree.query(node_r, l, r, l, mid);
        auto q2 = qs.tree.query(node_l, l, r, l, mid);
        auto res = q1 - q2;
        if (res >= k) {
            r = mid;
            node_l = node_l->left;
            node_r = node_r->left;
        } else {
            l = mid + 1;
            k -= res;
            node_l = node_l->right;
            node_r = node_r->right;
        }
    }
    return qs.id2val[l];
}

void solution(istream &iss) {
    int64_t N;
    iss >> N;
    vector<int64_t> v(N);
    int64_t a, l, m;
    iss >> a >> l >> m;
    v[0] = a;
    for (int64_t i = 1; i < N; ++i) {
        a = getAi(a, l, m);
        v[i] = a;
    }
    QueryStruct qs(v.size(), v);
    uint64_t B;
    iss >> B;
    uint64_t ans1 = 0;

    for (uint64_t i = 0; i < B; ++i) {
        uint64_t G, x, lx, mx, y, ly, my, k, lk, mk;
        uint64_t ig, jg;
        iss >> G >> x >> lx >> mx >> y >> ly >> my >> k >> lk >> mk;
        ig = min(x, y);
        jg = max(x, y);

        uint64_t A1 = query(qs, ig - 1, jg, k);
        ans1 += (uint64_t)A1;
        for (uint64_t gg = 1; gg < G; ++gg) {
            x = ((ig - 1) * lx) % N;
            x += mx;
            x %= N;
            x += 1;

            y = ((jg - 1) * ly) % N;
            y += my;
            y %= N;
            y += 1;

            ig = min(x, y);
            jg = max(x, y);

            k = ((k - 1) * lk);
            k += mk;
            uint64_t mod_k = (jg - ig + 1);
            k %= mod_k;
            k += 1;

            A1 = query(qs, ig - 1, jg, k);
            ans1 += (uint64_t)A1;

        }
    }
    cout << ans1;
}

int main() {

    solution(cin);

    return 0;
}

#include <bits/stdc++.h>

using namespace std;

static constexpr int MAX_N = 1e5;

class PersistArray {
 public:
  struct Node;
  using NodePtr = shared_ptr<Node>;
  struct Node {
    int val{};
    NodePtr left = nullptr;
    NodePtr right = nullptr;
    Node(int val, NodePtr left = nullptr, NodePtr right = nullptr) : val(val), left(left), right(right) {};
    Node() = default;
  };
  void build(const vector<int> &v);
  void build(NodePtr& node, const vector<int> &v, int l, int r);

  void update(int ver, int pos, int v);
  void update(NodePtr& node, int l, int r, int pos, int v);

  int get(const NodePtr& v, int l, int r, int pos);
  int get(int ver, int pos);

  PersistArray() : tree_version(MAX_N) {};

 private:
  vector<NodePtr> tree_version{};
  int cur_ver = 1;
  int n = 0;

};
void PersistArray::build(PersistArray::NodePtr& node, const vector<int> &v, int l, int r) {
    if (l == r) {
        node->val = v[l];
        node->left = node->right = nullptr;
        return;
    }
    int mid = (l + r) / 2;
    node->left = make_shared<Node>();
    build(node->left, v, l, mid);
    node->right = make_shared<Node>();
    build(node->right, v, mid + 1, r);
}
void PersistArray::build(const vector<int> &v) {
    n = v.size() - 1;
    tree_version[0] = make_shared<Node>();;
    build(tree_version[0], v, 0, n);
}
void PersistArray::update(PersistArray::NodePtr& node, int l, int r, int pos, int v) {
    if (l == r) {
        node->val = v;
        return;
    }
    int mid = (l + r) / 2;
    if (pos <= mid) {
        auto left = make_shared<Node>();
        *left = *node->left;
        node->left = left;
        update(left, l, mid, pos, v);
    } else {
        auto right = make_shared<Node>();
        *right = *node->right;
        node->right = right;
        update(right, mid + 1, r, pos, v);
    }

}
void PersistArray::update(int ver, int pos, int v) {
    auto node = make_shared<Node>();
    *node = *tree_version[ver - 1];
    tree_version[cur_ver] = node;
    update(tree_version[cur_ver], 0, n, pos, v);
    cur_ver++;
}
int PersistArray::get(int ver, int pos) {
    auto node = tree_version[ver - 1];
    return get(node, 0, n, pos);
}
int PersistArray::get(const PersistArray::NodePtr& v, int l, int r, int pos) {
    if (l == r) {
        return v->val;
    }
    int mid = (l + r) / 2;
    if (pos <= mid) {
        return get(v->left, l, mid, pos);
    } else {
        return get(v->right, mid + 1, r, pos);
    }
}

void solution(istream &iss) {
    int n;
    iss >> n;
    vector<int> a(n + 1, 0);

    for (int i = 1; i <= n; ++i) {
        iss >> a[i];
    }
    PersistArray pa;
    pa.build(a);
    int m;
    iss >> m;
    string s;
    for (int i = 0; i < m; ++i) {
        iss >> s;
        if (s[0] == 'c') {
            int ver, pos, val;
            iss >> ver >> pos >> val;
            pa.update(ver, pos, val);
        } else {
            int ver, pos;
            iss >> ver >> pos;
            cout << pa.get(ver, pos) << "\n";
        }
    }
}

int main() {
    istringstream iss("6\n"
                      "1 2 3 4 5 6\n"
                      "11\n"
                      "create 1 6 10\n"
                      "create 2 5 8\n"
                      "create 1 5 30\n"
                      "get 1 6\n"
                      "get 1 5\n"
                      "get 2 6\n"
                      "get 2 5\n"
                      "get 3 6\n"
                      "get 3 5\n"
                      "get 4 6\n"
                      "get 4 5");
//    istringstream iss("2\n"
//                      "1 2\n"
//                      "6\n"
//                      "create 1 1 10\n"
//                      "create 2 1 8\n"
//                      "create 1 2 30\n"
//                      "get 1 1\n"
//                      "get 2 1\n"
//                      "get 4 2\n");

    solution(iss);

    return 0;
}

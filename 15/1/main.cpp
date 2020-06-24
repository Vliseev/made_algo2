#include <bits/stdc++.h>
using namespace std;

class PersistStack {
  struct Node;
  using NodePtr = shared_ptr<Node>;
  struct Node {
    int val{};
    NodePtr next{};
    Node() = default;
    Node(int val, NodePtr next = nullptr) : val(val), next(next) {};
  };
  vector<Node> s;
  unordered_map<int, NodePtr> map_index{};

 public:
  void push(int i, int b, int x);
  int pop(int i, int a);
  PersistStack();
};
void PersistStack::push(int i, int b, int x) {
    auto node = map_index[b];
    NodePtr new_node = make_shared<Node>(x);
    new_node->next = node;
    map_index[i + 1] = new_node;
}
int PersistStack::pop(int i, int a) {
    auto node = map_index[a];
    int val = node->val;
    node = node->next;
    map_index[i + 1] = node;
    return val;
}

PersistStack::PersistStack() {
    map_index[0] = nullptr;
}

void solution(istream &iss) {
    int n;
    iss >> n;

    PersistStack ps;

    for (int i = 0; i < n; ++i) {
        int a, b;
        iss >> a >> b;
        if (b != 0) {
            ps.push(i, a, b);
        } else {
            int val = ps.pop(i, a);
            cout << val << "\n";
        }
    }

}

int main() {
    istringstream iss("8\n"
                      "0 1\n"
                      "1 5\n"
                      "2 4\n"
                      "3 2\n"
                      "4 3\n"
                      "5 0\n"
                      "6 6\n"
                      "1 0");

    solution(iss);

    return 0;
}

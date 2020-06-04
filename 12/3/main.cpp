//
// Created by user on 03.06.2020.
//
#include <bits/stdc++.h>

static const int Inf = 1e9;

using namespace std;
class SuffTree {

 public:
  struct Node {
	using ShNode = std::shared_ptr<Node>;
	using WeakNode = std::weak_ptr<Node>;

	int64_t l{};
	int64_t r{};
	WeakNode link{};
	map<char, ShNode> children{};

	Node(int64_t l, int64_t r) : l(l), r(r) {};
	Node() = default;

	int64_t size(int64_t cur_pos = 0) const {
		return min(r, cur_pos + 1) - l;
	}
  };
  SuffTree(int64_t size) : size(size) {
	  root = state.cur_node = make_shared<Node>(-1, -1);
  }
  void addLink(Node::ShNode node) {
	  if (need_link) {
		  need_link->link = node;
	  }
	  need_link = node;
  }

  bool down(Node::ShNode next) {
	  if (state.cur_len >= next->size(pos)) {
		  state.cur_edge += next->size(pos);
		  state.cur_len -= next->size(pos);
		  state.cur_node = next;
		  return true;
	  }
	  return false;
  }

  void addChar(char c) {
	  s += c;
	  pos++;
	  need_link = nullptr;
	  rem++;
	  while (rem > 0) {
		  if (state.cur_len == 0) {
			  state.cur_edge = pos;
		  }
		  if (!state.cur_node->children.count(s[state.cur_edge])) {
			  auto leaf = make_shared<Node>(pos, size);
			  state.cur_node->children[s[state.cur_edge]] = leaf;
			  addLink(state.cur_node);
		  } else {
			  auto next = state.cur_node->children[s[state.cur_edge]];
			  if (down(next)) continue;
			  if (s[next->l + state.cur_len] == c) {
				  state.cur_len++;
				  addLink(state.cur_node);
				  break;
			  }
			  auto split = make_shared<Node>(next->l, next->l + state.cur_len);
			  state.cur_node->children[s[state.cur_edge]] = split;
			  auto leaf = make_shared<Node>(pos, size);
			  split->children[c] = leaf;
			  next->l += state.cur_len;
			  split->children[s[next->l]] = next;
			  addLink(split);
		  }
		  rem--;

		  if (state.cur_node == root && state.cur_len > 0) {
			  state.cur_len--;
			  state.cur_edge = pos - rem + 1;
		  } else {
			  auto link = state.cur_node->link.lock();
			  state.cur_node = link ? link : root;
		  }
	  }
  }

  const Node::ShNode &getRoot() const {
	  return root;
  }

  int64_t getCounter() const {
	  return counter;
  }
  void setCounter(int64_t counter) {
	  SuffTree::counter = counter;
  }

 private:
  Node::ShNode root;
  int64_t pos = -1;
  Node::ShNode need_link{};
  int64_t rem{};
  int64_t size;
  int64_t counter = 1;

  struct State {
	Node::ShNode cur_node{};
	int64_t cur_edge{};
	int64_t cur_len{};
  };

  State state{};
  string s{};
};

void Dfs(SuffTree::Node::ShNode node) {

	for (const auto &c: node->children) {
		Dfs(c.second);
	}

}

int main() {
	string s;
	cin >> s;

	SuffTree st(s.size());

	for (char c : s)
		st.addChar(c);

	Dfs(st.getRoot());

	return 0;
}
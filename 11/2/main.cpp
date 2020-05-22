#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string_view>
#include <tuple>
#include <vector>

using namespace std;
static constexpr int AlphSize = 27;
static constexpr int FirstSym = 96;

int sNum(char sym) {
  return sym - FirstSym;
}

vector<int> sufArr(string_view s) {
  int n = s.size();
  vector<int> p(n), c(n), cnt(max(n, AlphSize), 0);

  for_each(s.begin(), s.end(), [&cnt](auto sym) {
    cnt[sNum(sym)]++;
  });
  partial_sum(cnt.begin(), cnt.end(), cnt.begin());
  for (int i = 0; i < n; i++) {
    p[--cnt[sNum(s[i])]] = i;
  }

  c[p[0]] = 0;
  int classes = 1;
  for (int i = 1; i < n; i++) {
    if (s[p[i]] != s[p[i - 1]])
      classes++;
    c[p[i]] = classes - 1;
  }
  vector<int> pn(n), cn(n);
  for (int h = 0; (1 << h) < n; ++h) {
    for (int i = 0; i < n; i++) {
      pn[i] = p[i] - (1 << h);
      if (pn[i] < 0)
        pn[i] += n;
    }
    fill(cnt.begin(), cnt.begin() + classes, 0);
    for_each(pn.begin(), pn.end(), [&](auto sym) {
      cnt[c[sym]]++;
    });
    partial_sum(cnt.begin(), cnt.begin() + classes, cnt.begin());

    for (int i = n - 1; i >= 0; i--)
      p[--cnt[c[pn[i]]]] = pn[i];
    cn[p[0]] = 0;
    classes = 1;
    for (int i = 1; i < n; i++) {
      int f1 = c[p[i]], f2 = c[p[i - 1]];
      int p1 = c[(p[i] + (1 << h)) % n], p2 = c[(p[i - 1] + (1 << h)) % n];
      if (f1 != f2 || p1 != p2)
        ++classes;
      cn[p[i]] = classes - 1;
    }
    swap(c, cn);
  }
  return p;
}

vector<int> lcp(string_view s, const vector<int> &suf_arr, const vector<int> &pos) {
  int n = s.size(), j = 0;
  int prev = 0;
  vector<int> lcp(n);

  for (int i = 0; i < n - 1; ++i) {
    j = suf_arr[pos[i] - 1];
    auto cur = prev;
    while (s[i + cur] == s[j + cur])
      cur++;
    lcp[pos[i]] = cur;
    prev = max(cur - 1, 0);
  }
  return lcp;
}

bool findString(string_view s, string_view t, const vector<int> &p) {
  int s_size = s.size();
  int t_size = t.size();

  int l = 0, r = s_size - 1;
  while (l <= r) {
    int m = l + (r - l) / 2;
    int cmp = s.compare(p[m], t_size, t);
    if (cmp == 0)
      return true;
    if (cmp > 0) {
      r = m - 1;
    } else {
      l = m + 1;
    }
  }
  return false;
}

int solution(std::istream &iss) {

  int n;
  iss >> n;
  vector<string> vofs(n);
  for (int i = 0; i < n; ++i) {
    iss >> vofs[i];
  }
  std::string s;
  iss >> s;
  s += '`';
  auto suf_arr = sufArr(s);

  for (int i = 0; i < n; ++i) {
    if (findString(s, vofs[i], suf_arr))
      cout << "YES\n";
    else
      cout << "NO\n";
  }

  return 0;
}

int main() {
  istringstream iss("3\n"
                    "abc\n"
                    "abcdr\n"
                    "abcde\n"
                    "xabcdef");
  std::ios::sync_with_stdio(false), std::cin.tie(nullptr),
      std::cout.tie(nullptr);

  solution(iss);

  return 0;
}

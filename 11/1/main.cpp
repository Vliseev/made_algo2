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

int solution(std::istream &iss) {

  std::string s;
  iss >> s;
  s += '`';
  int n = s.length();
  auto suf_arr = sufArr(s);
  for (int i = 1; i < n; i++) {
    cout << suf_arr[i] + 1 << " ";
  }
  cout << "\n";
  vector<int> pos(n);
  for (int i = 0; i < n; i++) {
    pos[suf_arr[i]] = i;
  }

  auto lcp_arr = lcp(s, suf_arr, pos);
  for (int i = 2; i < n; i++) {
    cout << lcp_arr[i] << " ";
  }

  return 0;
}

int main() {
  istringstream iss("ababb");

  solution(iss);

  return 0;
}

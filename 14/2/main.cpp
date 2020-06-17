#include <bits/stdc++.h>
using namespace std;

namespace {
using dcomplex = complex<double>;
using ComplexArray = vector<dcomplex>;
constexpr double PI = 3.141592653589793238462643383279502884;
}

void fft(ComplexArray &a) {
    int n = a.size();
    if (n == 1)
        return;
    ComplexArray a0(n / 2), a1(n / 2);
    for (int i = 0; i < n; ++i) {
        if (i & 1) {
            a1[i >> 1] = a[i];
        } else {
            a0[i >> 1] = a[i];
        }
    }
    fft(a0);
    fft(a1);

    double phi = 2 * PI / n;
    dcomplex w(1.0), wn(cos(phi), sin(phi));

    for (int i = 0; 2 * i < n; ++i) {
        a[i] = a0[i] + w * a1[i];
        a[i + n / 2] = a0[i] - w * a1[i];
        w *= wn;
    }
}

void invFft(ComplexArray &a) {
    reverse(a.begin() + 1, a.end());
    fft(a);
    double n = a.size();
    transform(a.begin(), a.end(), a.begin(), [&n](auto &val) {
      return val / n;
    });
}

template<class T, class OutT>
vector<OutT> mul(const vector<T> &a, const vector<T> &b) {
    ComplexArray ca(a.begin(), a.end());
    ComplexArray cb(b.begin(), b.end());
    int n = 1;
    while (n < a.size() + b.size())
        n <<= 1;
    ca.resize(n);
    cb.resize(n);
    fft(ca);
    fft(cb);
    for (int i = 0; i < n; ++i) {
        ca[i] *= cb[i];
    }
    invFft(ca);
    vector<OutT> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = round(ca[i].real());
    }

    return result;
}

void getNumber(const string &s, vector<bool> &a) {
    for (int i = 0; i < s.size(); i++) {
        a[i] = (s[i] - '0');
    }
}

void solution(istream &iss) {
    string s;

    iss >> s;

    vector<bool> a(s.size());
    getNumber(s, a);

    auto result = mul<bool, int>(a, a);

    size_t ans = 0;
    for (int i = 0; i * 2 < result.size() && i < a.size(); ++i) {
        if (a[i]) {
            ans += (result[2 * i] - 1) / 2;
        }
    }
    cout << ans;
}

int main() {
    istringstream iss("101010101");
    solution(cin);

    return 0;
}


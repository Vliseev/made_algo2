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

vector<int> mul(const vector<int> &a, const vector<int> &b) {
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
    vector<int> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = round(ca[i].real());
    }

    int carry = 0;
    for (int i = 0; i < n; ++i) {
        result[i] += carry;
        carry = result[i] / 10;
        result[i] %= 10;
    }

    int real_n = n - 1;
    while (real_n >= 0 && result[real_n] == 0) {
        --real_n;
    }

    result.resize(real_n + 1);
    reverse(result.begin(), result.end());
    return result;
}

bool getNumber(const string &s, vector<int> &a) {
    if (s[0] == '-') {
        for (int i = 1; i < s.size(); i++) {
            a[i - 1] = (s[i] - '0');
        }
        reverse(a.begin(), a.end() - 1);
        return true;
    } else {
        for (int i = 0; i < s.size(); i++) {
            a[i] = (s[i] - '0');
        }
        reverse(a.begin(), a.end());
        return false;
    }
}

void solution(istream &iss) {
    string s;
    bool negative;

    iss >> s;

    if (s == "0") {
        cout << 0;
        return;
    }

    vector<int> a(s.size());
    negative = getNumber(s, a);

    iss >> s;

    if (s == "0") {
        cout << 0;
        return;
    }

    vector<int> b(s.size());
    negative ^= getNumber(s, b);

    auto result = mul(a, b);

    if (negative) {
        cout << "-";
    }
    for (auto d:result) {
        cout << static_cast<int>(d);
    }
    cout << endl;
}

int main() {
    solution(cin);

    return 0;
}


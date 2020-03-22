from itertools import count
import random
import math
from collections import deque

M_S = int(1e6)


def f(x, n):
    return (x * x + 1) % n


def sieve(n):
    lp = [0] * (n + 1)
    primes = []
    for i in range(2, n + 1):
        if lp[i] == 0:
            lp[i] = i
            primes.append(i)
        for p in primes:
            if p > lp[i] or i * p >= n + 1:
                break
            lp[i * p] = p
    return lp, primes


def pollard_rho(m):
    while True:
        a = random.randint(1, m - 1)
        a2i = f(a, m)

        d = math.gcd(a - a2i, m)
        if 1 < d < m:
            return d, m // d

        for i in count(10000):
            a = f(a, m)
            a2i = f(a2i, m)
            a2i = f(a2i, m)
            d = math.gcd(a - a2i, m)
            if 1 < d < m:
                return d, m // d


def mt_iter(p, q, n, b):
    n1 = n - 1
    r = pow(b, q, n)
    if r == 1 or r == n1:
        return True
    for _ in range(p):
        r = (r * r) % n
        if r == n1:
            return True
    return False


def mr_test(n):
    q = n - 1
    p = 0
    while (not q == 0) and (q % 2 == 0):
        p += 1
        q //= 2

    for _ in range(10):
        b = random.randint(2, n - 1)
        d = math.gcd(b, n)
        if 1 < d < n:
            return False
        if not mt_iter(p, q, n, b):
            return False

    return True


def main():
    n = int(input())

    lp, fact = sieve(int(M_S))
    q = deque([n])
    factors = []
    random.seed(111)

    while len(q) > 0:
        f = q.pop()
        if f < M_S:
            if lp[f] == f:
                factors.append(f)
            else:
                factors.append(lp[f])
                while f != lp[f]:
                    f //= lp[f]
                    factors.append(lp[f])
        else:
            if mr_test(f):
                factors.append(f)
            else:
                a, b = pollard_rho(f)
                q.append(a)
                q.append(b)

    factors.sort()
    print(" ".join(map(str, factors)))


if __name__ == '__main__':
    main()

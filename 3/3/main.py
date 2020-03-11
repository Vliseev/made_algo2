import sys


def egcd(x, y):
    a, b, g, u, v, w = 1, 0, x, 0, 1, y
    while w > 0:
        q = g // w
        a, b, g, u, v, w = u, v, w, a - q * u, b - q * v, g - q * w

    return a, b, g


def factor_n(n):
    i = 2
    while i * i < n:
        if n % i == 0:
            return i
        i += 1

    return 1


def main():
    n = int(input())
    e = int(input())
    C = int(input())

    p = factor_n(n)
    q = n // p
    phi = (p - 1) * (q - 1)

    d, _, _ = egcd(e, phi)
    d = d % phi

    ans = pow(C, d, n)
    print(ans)


if __name__ == '__main__':
    main()

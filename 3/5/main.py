import sys


def egcd(x, y):
    if x == 0:
        return 0, 1, y
    a, b = 1, 0
    g = x
    u, v = 0, 1
    w = y
    while w > 0:
        q = g // w
        a, u = u, a - q * u
        b, v = v, b - q * v
        g, w = w, g - q * w

    return a, b, g


def main():
    N = int(input())
    for _ in range(N):
        a, b, n, m = map(int, input().split())
        u, v, _ = egcd(n, m)
        x = v * m * a + u * n * b
        x = x % (n * m)
        print(x)


if __name__ == '__main__':
    main()

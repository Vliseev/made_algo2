import sys


def egcd(x, y):
    if x==0:
        return 0, 1, y
    a, b, g, u, v, w = 1, 0, x, 0, 1, y
    while w > 0:
        q = g // w
        a, b, g, u, v, w = u, v, w, a - q * u, b - q * v, g - q * w

    return a, b, g


def main():
    A, B, C = map(int, input().split())
    
    C =-C
    u, v, g = egcd(abs(A), abs(B))

    if C%g!=0:
        print(-1)
        return

    x0 = u * C // g
    y0 = v * C // g
    x0 = x0 if A>0 else -x0
    y0 = y0 if B>0 else -y0

    print(x0, y0)


if __name__ == '__main__':
    main()

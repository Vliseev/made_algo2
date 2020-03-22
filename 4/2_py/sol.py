import math


def solution():
    a, b, n = map(int, input().split())
    q = n - 1
    k = int(math.sqrt(n) + 1) % q
    r = q - k

    a1 = pow(a, r, n)
    key = b

    table_lu = {key: 0}

    for i in range(1, k):
        key = (key * a1) % n
        table_lu[key] = i

    key = 1
    for i in range(k + 1):
        if key in table_lu:
            print((k * table_lu[key] + i) % q)
            return
        key = (key * a) % n
    print(-1)


def main():
    solution()


if __name__ == '__main__':
    main()

from dataclasses import dataclass
import io
import math
import sys


@dataclass
class Circle:
    x: int
    y: int
    r: int

    def __add__(self, other):
        return Circle(self.x + other.x, self.y + other.y, self.r + other.r)

    def __sub__(self, other):
        return Circle(self.x - other.x, self.y - other.y, self.r - other.r)


@dataclass
class Point:
    x: float
    y: float

    def __str__(self):
        return f"{self.x} {self.y}"


def circle_create(line):
    x, y, r = tuple(map(int, line.split()))
    return Circle(x, y, r)


def get_intersection(c1: Circle, c2: Circle):
    d_square = (c1.x - c2.x) ** 2 + (c1.y - c2.y) ** 2
    dx = c2.x - c1.x
    dy = c2.y - c1.y
    d = math.sqrt(d_square)

    if d_square > (c1.r + c2.r) ** 2:
        print(0)
    elif d_square < (c2.r - c1.r) ** 2:
        print(0)
    else:
        a = (c1.r ** 2 - c2.r ** 2 + d_square) / (2 * math.sqrt(d_square))
        h = math.sqrt(float(c1.r ** 2 - a ** 2))
        cx = float(c1.x) + float(a * dx) / float(d)
        cy = float(c1.y) + float(a * dy) / float(d)

        if d_square == (c1.r + c2.r) ** 2 or d_square == (c1.r - c2.r) ** 2:
            print(1)
            print(Point(cx, cy))
            return

        point1 = Point(cx - float(h * dy) / float(d),
                       cy + float(h * dx) / float(d))

        point2 = Point(cx + float(h * dy) / float(d),
                       float(cy) - float(h * dx) / float(d))

        print(2)
        print(Point(cx, cy))
        print(f"{abs(a)} {abs(h)}")
        print(point1)
        print(point2)
        return


def solution():
    n = int(input())

    circles = []
    for _ in range(n):
        x0, y0, r0 = map(float, input().split())
        x1, y1, r1 = map(float, input().split())
        circles.append((Circle(x0, y0, r0), Circle(x1, y1, r1)))

    for c1, c2 in circles:
        if (c1.x, c1.y) == (c2.x, c2.y):
            if c1.r == c2.r:
                print(3)
            else:
                print(0)
            continue

        get_intersection(c1, c2)


if __name__ == '__main__':
    solution()

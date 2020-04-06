import random
import pytest
import random
import io
from subprocess import Popen, PIPE

QUERY_TYPE = {1: "add", 2: "set", 3: "min"}


def get_test_case(num_q, num_el, seed=123):
    random.seed(seed)
    ans = []
    query = []
    numbers = [random.randint(1, 6) for _ in range(num_el)]
    base = numbers[:]
    for _ in range(num_q):
        type = random.randint(1, 3)
        if type == 1:
            l = random.randint(0, num_el - 1)
            r = random.randint(l, num_el - 1)
            val = random.randint(-5, 5)
            for i in range(l, r + 1):
                numbers[i] += val
            query.append(f"add {l + 1} {r + 1} {val}")
        elif type == 2:
            l = random.randint(0, num_el - 1)
            r = random.randint(l, num_el - 1)
            val = random.randint(-5, 5)
            for i in range(l, r + 1):
                numbers[i] = val
            query.append(f"set {l + 1} {r + 1} {val}")
        else:
            l = random.randint(0, num_el - 1)
            r = random.randint(l, num_el - 1)
            ans.append(min(numbers[l:r + 1]))
            query.append(f"min {l + 1} {r + 1}")

    return base, query, ans


def run_prog(inp):
    p = Popen("cmake-build-debug/3", stdin=PIPE, stdout=PIPE)
    out, err = p.communicate(inp.encode())
    if not err is None:
        return "-10000"
    return out.decode()


def test_1():
    n = 3
    for i in range(100):
        seed = random.randint(0, 100000)
        numbers, query, ans = get_test_case(6, n, seed)
        inp_str = f"{n}\n"
        inp_str += " ".join(map(str, numbers)) + "\n"
        inp_str += "\n".join(query)
        ans_1 = run_prog(inp_str)
        if len(ans_1) > 1:
            ans_1 = list(map(int, ans_1[:-1].split("\n")))
            assert ans == ans_1, f"{i} input val: {seed}"
        else:
            assert len(ans) == 0, f"{i} input val: {seed}"


def t(seed):
    n = 5
    numbers, query, ans = get_test_case(4, n, seed)
    inp_str = f"{n}\n"
    inp_str += " ".join(map(str, numbers)) + "\n"
    inp_str += "\n".join(query)
    ans_1 = run_prog(inp_str)
    if len(ans_1) > 1:
        ans_1 = list(map(int, ans_1[:-1].split("\n")))
        assert ans == ans_1, f"input val: {seed}"
    else:
        assert len(ans) == 0, f"input val: {seed}"


if __name__ == '__main__':
    n = 4
    numbers, query, ans = get_test_case(4, n, 93125)
    inp_str = f"{n}\n"
    inp_str += " ".join(map(str, numbers)) + "\n"
    inp_str += "\n".join(query)
    print(inp_str)
    print(ans)

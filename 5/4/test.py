import random
import pytest
import random
import io
import tqdm
from subprocess import Popen, PIPE


def get_test_case(seed):
    random.seed(seed)
    n = random.randint(1, 10 ** 3)
    m = random.randint(1, 10 ** 3)
    a1 = random.randint(0, 16714589)
    u, v = random.randint(1, n), random.randint(1, n)
    return n, m, a1, u, v


def run_prog(inp):
    p = Popen("cmake-build-debug/4", stdin=PIPE, stdout=PIPE)
    out, err = p.communicate(inp.encode())
    ret_code = p.wait()
    return out.decode(), err.decode() if err else "", ret_code


def test_1():
    n = 3
    for i in tqdm.tqdm(range(100)):
        seed = random.randint(0, 100000)
        n, m, a1, u, v = get_test_case(seed)
        inp_str = f"{n} {m} {a1}\n{u} {v}"
        out, err, ret_code = run_prog(inp_str)
        if ret_code != 0:
            print(f"error {seed}")


if __name__ == '__main__':
    # test_1()
    n, m, a1, u, v = get_test_case(23497)
    print(f"{n} {m} {a1}\n{u} {v}")

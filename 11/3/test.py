import io
import random
import string
from subprocess import PIPE, Popen
import tqdm
from itertools import combinations


def randomString(stringLength=8):
    letters = string.ascii_lowercase
    return ''.join(random.choice(letters) for i in range(stringLength))


def get_ans(random_str):
    s = [random_str[x:y] for x, y in combinations(
        range(len(random_str) + 1), r=2)]

    return len(set(s))


def run_prog(inp):
    p = Popen("cmake-build-debug/1", stdin=PIPE, stdout=PIPE)
    out, err = p.communicate(inp.encode())
    ret_code = p.wait()
    return int(out.decode())


def test_1():
    random.seed(123)
    for i in tqdm.tqdm(range(10000)):
        inp = randomString(random.randint(10, 200))
        ans = run_prog(inp)
        true_ans = get_ans(inp)
        if true_ans != ans:
            print(f"error {inp}")
            break


test_1()

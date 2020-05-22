import io
import random
import string
from subprocess import PIPE, Popen
import tqdm


def randomString(stringLength=8):
    letters = string.ascii_lowercase
    return ''.join(random.choice(letters) for i in range(stringLength))


def run_prog(inp):
    p = Popen("cmake-build-debug/1", stdin=PIPE, stdout=PIPE)
    out, err = p.communicate(inp.encode())
    ret_code = p.wait()
    return ret_code


def test_1():
    for i in tqdm.tqdm(range(1000)):
        inp = randomString(random.randint(10,30))
        ret_code = run_prog(inp)
        if ret_code != 0:
            print(f"error {inp}")
            break

test_1()

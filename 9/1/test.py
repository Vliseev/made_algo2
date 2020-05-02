import io
import random
from subprocess import PIPE, Popen

import networkx as nx
import pytest
import tqdm
from matplotlib import pyplot as plt


def gen_test_cases(n_iter=100, seed=123):
    random.seed(seed)
    n = 8

    for _ in range(n_iter):
        while True:
            m = random.randint(n, 50)
            DG = nx.gnm_random_graph(n, m, seed=random.randint(1000, 10000), directed=True)

            WG = nx.DiGraph()
            WG.add_nodes_from(DG.nodes())

            for (u, v) in DG.edges():
                WG.add_edge(u, v, capacity=random.randint(
                    1, 8), weight=random.randint(1, 3))

            inp = io.StringIO()
            inp.write(f"{n} {m}\n")
            for u, v in WG.edges():
                edge = WG[u][v]
                inp.write(f"{u + 1} {v + 1} {edge['capacity']} {edge['weight']}\n")

            flow = nx.max_flow_min_cost(WG, 0, n - 1)
            cost = nx.cost_of_flow(WG, flow)
            if cost:
                break

        yield inp.getvalue(), cost, flow


def run_prog(inp):
    p = Popen("cmake-build-debug/1", stdin=PIPE, stdout=PIPE)
    out, err = p.communicate(inp.encode())
    ret_code = p.wait()
    return out.decode(), err.decode() if err else "", ret_code


def test_1():
    for i, (inp, cost, flow) in enumerate(gen_test_cases(1000)):
        out, err, ret_code = run_prog(inp)
        if ret_code != 0:
            print(f"error {inp}")
        assert cost == int(out), f"{cost} {out}\n{inp}\n{flow}"


test_1()
#
#
# if __name__ == '__main__':
#     # test_1()
#     n, m, a1, u, v = get_test_case(23497)
#     print(f"{n} {m} {a1}\n{u} {v}")

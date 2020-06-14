from random import randrange

from tools import Graph, Edge, get_edge, prim
from sys import getsizeof as sizeof
import time

from sys import stdin, stderr
from typing import List, Callable, Optional


class Result:
    def __init__(self,
                 steps: int,
                 weight: float,
                 memory: int,
                 path: List[Edge] = [],
                 time: float = 0.0
                 ) -> None:
        self.steps: int = steps
        self.weight: float = weight
        self.memory: int = memory
        self.path: List[Edge] = path
        self.time: float = time

    def __str__(self):
        return "{:d} {:.2f} {:d} {:.2f}".format(
            self.steps,
            self.weight,
            self.memory,
            self.time
        )

    def __repr__(self):
        return str(self)


def with_time(G: Graph, f: Callable[[Graph], Result]) -> Result:
    t0 = time.perf_counter()
    res = f(G)
    t1 = time.perf_counter()
    res.time = (t1 - t0) * 1000
    return res


def random_walk(G: Graph) -> Result:
    def run(G: Graph) -> Result:
        memory: int = 0
        k: int = 0
        W: float = 0.0
        path: List[Edge] = []
        vertices: List[int] = G.vertices()
        memory += sizeof(vertices)

        i: int = randrange(len(vertices))
        start: int = vertices[i]
        del vertices[i]

        u: int = start
        while len(vertices) > 0:
            i = randrange(len(vertices))
            v: int = vertices[i]
            del vertices[i]
            e = G.get_edge(u, v)
            path.append(e)
            W += e.w
            k += 1
            u = v

        e = G.get_edge(u, start)
        path.append(e)
        W += e.w
        k += 1

        return Result(k, W, memory, path=path)

    return with_time(G, run)


def greedy_edge(G: Graph) -> Result:
    def run(G: Graph) -> Result:
        memory: int = 0
        k: int = 0
        W: float = 0.0
        path: List[Edge] = []
        vertices: List[int] = G.vertices()
        memory += sizeof(vertices)

        i: int = randrange(len(vertices))
        start: int = vertices[i]
        del vertices[i]

        G.order_edges()

        u: int = start
        while len(vertices) > 0:
            next_e: Optional[Edge] = next(
                (e for e in G.get_incident_edges(u) if e.snd in vertices),
                None
            )

            if next_e is None:
                raise Exception('None')
            v: int = next_e.snd

            vertices.remove(v)

            e = G.get_edge(u, v)
            path.append(e)
            W += e.w
            k += 1

            u = v

        e = G.get_edge(u, start)
        path.append(e)
        W += e.w
        k += 1

        return Result(k, W, memory, path=path)

    return with_time(G, run)


def use_mst(G: Graph) -> Result:
    class Counter:
        def __init__(self):
            self.c = 0

        def inc(self):
            self.c += 1

        def get(self):
            return self.c

    def run(G: Graph) -> Result:
        c: Counter = Counter()
        mst, _ = prim(G, 1)
        tree = Graph(G.N, directed=False)
        tree.add_edges(mst)

        visited: List[bool] = [False for _ in range(G.N + 1)]
        walk: List[int] = []

        def DFS(u: int):
            visited[u] = True
            walk.append(u)
            c.inc()
            for e in tree.get_incident_edges(u):
                v = e.snd
                if not visited[v]:
                    DFS(v)

        DFS(1)

        path: List[Edge] = []
        w: float = 0.0
        first: int = walk[0]
        u: int = first

        walk.append(first)

        for v in walk[1:]:
            c.inc()
            e = G.get_edge(u, v)
            path.append(e)
            w += e.w
            u = v

        memory: int = 0
        memory += sizeof(mst)
        memory += sizeof(tree)
        memory += sizeof(visited)
        memory += sizeof(walk)

        return Result(c.get(), w, memory, path=path)

    return with_time(G, run)

if __name__ == '__main__':
    n: int = 0
    try:
        n = int(stdin.readline())
    except ValueError:
        print('Wrong \'n\' format')
        exit(-1)

    G: Graph = Graph(n, directed=False)
    m: int = n * (n - 1) // 2

    try:
        for _ in range(m):
            line = stdin.readline()
            u, v, w = get_edge(line)
            edge = Edge(u, v, w)
            G.add_edge(edge)
    except ValueError:
        print("Wrong \'u v w\' input format")
        exit(-1)

    results: List[Result] = [
        random_walk(G),
        greedy_edge(G),
        use_mst(G)
    ]

    for result in results:
        print(result)
        print(result.path, file=stderr)
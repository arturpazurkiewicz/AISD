
from tools import Graph, Edge, get_edge, dijkstra, get_path
from sys import stdin, stderr
import time


if __name__ == "__main__":
    n = 0
    try:
        n = int(stdin.readline())
    except ValueError:
        print("Wrong \'n\' input format")
        exit(-1)

    G: Graph = Graph(n, directed=True)

    m = 0
    try:
        m = int(stdin.readline())
    except ValueError:
        print("Wrong \'m\' input format")
        exit(-1)

    try:
        for _ in range(m):
            line = stdin.readline()
            u, v, w = get_edge(line)
            edge = Edge(u, v, w)
            G.add_edge(edge)
    except ValueError:
        print("Wrong \'u v w\' input format")
        exit(-1)

    src: int = 0
    try:
        src = int(stdin.readline())
    except ValueError:
        print("Wrong \'src\' input format")
        exit(-1)

    t0 = time.perf_counter()

    d, prev = dijkstra(G, src)

    for v in range(1, n + 1):
        path = get_path(G, prev, v)
        print("{:d} {:.2f}".format(v, d[v]))
        print(path, file=stderr)

    t1 = time.perf_counter()

    print("Time: ",(t1 - t0) * 1000, file=stderr)


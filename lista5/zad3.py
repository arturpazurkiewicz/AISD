from tools import Graph, Edge, get_edge, kruskal, prim
from sys import argv, stdin, stderr
from typing import List, Tuple

if __name__ == '__main__':
    algorithm: str = ""

    if len(argv) < 2:
        print('Not enough arguments!')
        print('Usage: main (-k|-p)')
        exit(-1)
    elif argv[1] == '-k':
        algorithm = 'kruskal'
    elif argv[1] == "-p":
        algorithm = 'prim'
    else:
        print('Unknown algorithm')
        exit(-1)

    n: int = 0
    try:
        n = int(stdin.readline())
    except ValueError:
        print("Wrong \'n\' input format")
        exit(-1)

    G: Graph = Graph(n, directed=False)

    m: int = 0
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

    mst: List[Edge] = None
    weight: float = 0
    if algorithm == 'kruskal':
        mst, weight = kruskal(G)
    elif algorithm == 'prim':
        mst, weight = prim(G, 1)
    else:
        raise Exception('Unknown algorithm')

    for e in mst:
        fst, snd, w = e.unpack()
        u, v = (fst, snd) if fst < snd else (snd, fst)
        print(u, v, w)

    print(weight)

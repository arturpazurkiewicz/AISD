from collections import deque
from typing import List, Tuple, Dict, TypeVar, Generic
import math


def parent(i: int) -> int:
    return (i - 1) // 2


def left(i: int) -> int:
    return 2 * i + 1


def right(i: int) -> int:
    return 2 * i + 2


Item = TypeVar('Item')


def update_item(item: Tuple[Item, float], newPr: float) -> Tuple[Item, float]:
    val, _ = item
    return val, newPr


class HeapSort(Generic[Item]):
    def __init__(self) -> None:
        self.data: List[Tuple[Item, float]] = []
        self.size: int = 0
        self.keys: Dict[Item, int] = {}
        pass

    def swap(self, i: int, j: int) -> None:
        di = self.data[i]
        dj = self.data[j]
        self.data[j], self.data[i] = di, dj
        self.keys[di[0]] = j
        self.keys[dj[0]] = i

    def min_heapify(self, i: int) -> None:
        l = left(i)
        r = right(i)
        minimal = l if (
                l < self.size and self.data[l][1] < self.data[i][1]) else i
        minimal = r if (r < self.size and self.data[r][1] < self.data[minimal][
            1]) else minimal

        if minimal != i:
            self.swap(i, minimal)
            self.min_heapify(minimal)

    def empty(self) -> bool:
        return self.size == 0

    def insert(self, item: Item, pr: float) -> None:
        tup = (item, math.inf)
        if len(self.data) > self.size:
            self.data[self.size] = tup
        else:
            self.data.append(tup)

        self.keys[item] = self.size
        self.decrease_key(self.size, pr)
        self.size += 1

    def top(self) -> Item:
        if self.empty():
            raise Exception('Empty heap')
        return self.data[0][0]

    def in_queue(self, item: Item) -> bool:
        return item in self.keys

    def pop(self) -> Item:
        if self.empty():
            raise Exception('Empty heap')
        min = self.data[0][0]
        self.keys.pop(min)
        self.data[0] = self.data[self.size - 1]
        self.size -= 1
        self.min_heapify(0)
        return min

    def pop_with_priority(self) -> Tuple[Item, float]:
        if self.empty():
            raise Exception('Empty heap')
        popped = self.data[0]
        self.pop()
        return popped

    def decrease_key(self, idx: int, newPr: float) -> None:
        if newPr > self.data[idx][1]:
            return

        self.data[idx] = update_item(self.data[idx], newPr)
        i: int = idx
        while i > 0 and self.data[parent(i)][1] > self.data[i][1]:
            self.swap(i, parent(i))
            i = parent(i)

    def priority(self, item: Item, newPr: float) -> None:
        if item in self.keys.keys():
            self.decrease_key(self.keys[item], newPr)
        else:
            print('not ok')

    def get_data(self) -> List[Tuple[Item, float]]:
        return self.data[0:self.size]


EdgeTuple = Tuple[int, int, float]


def get_edge(line: str) -> Tuple[int, int, float]:
    v = line.rstrip().split(' ')
    return int(v[0]), int(v[1]), float(v[2])


class Forest:
    def __init__(self) -> None:
        self.parent: Dict[int, int] = dict()
        self.rank: Dict[int, int] = dict()

    def make_set(self, v: int) -> None:
        self.parent[v] = v
        self.rank[v] = 0

    def find(self, v: int) -> int:
        if self.parent[v] != v:
            self.parent[v] = self.find(self.parent[v])
        return self.parent[v]

    def union(self, u: int, v: int) -> None:
        rep_u: int = self.find(u)
        rep_v: int = self.find(v)
        self.link(rep_u, rep_v)

    def link(self, u: int, v: int) -> None:
        if self.rank[u] > self.rank[v]:
            self.parent[v] = u
        else:
            self.parent[u] = v
            if self.rank[u] == self.rank[v]:
                self.rank[v] += 1


class Edge:
    def __init__(self, fst: int, snd: int, w: float = 1) -> None:
        self.fst: int = fst
        self.snd: int = snd
        self.w: float = w

    def __str__(self) -> str:
        return str(self.unpack())

    def __repr__(self) -> str:
        return str(self)

    def unpack(self) -> EdgeTuple:
        return self.fst, self.snd, self.w

    def reversed(self):
        return Edge(self.snd, self.fst, w=self.w)


class Graph:
    def __init__(self, N: int, directed: bool = False) -> None:
        self.N: int = N
        self.edges_by_src: List[List[Edge]] = [[] for i in range(N + 1)]
        self.directed: bool = directed

    def vertices(self) -> List[int]:
        return list(range(1, self.N + 1))

    def edges(self) -> List[Edge]:
        if self.directed:
            return [e
                    for v in self.vertices()
                    for e in self.edges_by_src[v]
                    ]
        else:
            return [e
                    for v in self.vertices()
                    for e in self.edges_by_src[v]
                    if e.fst < e.snd
                    ]

    def add_edge(self, edge: Edge) -> None:
        self.edges_by_src[edge.fst].append(edge)
        if not self.directed:
            self.edges_by_src[edge.snd].append(edge.reversed())

    def add_edges(self, edges: List[Edge]) -> None:
        for e in edges:
            self.add_edge(e)

    def get_incident_edges(self, u: int) -> List[Edge]:
        return self.edges_by_src[u]

    def get_edge(self, u: int, v: int) -> Edge:
        edge = [e for e in self.edges_by_src[u] if e.snd == v]

        if len(edge) > 0:
            return edge[0]
        else:
            raise Exception('No edge ' + str(u) + "->" + str(v))

    def order_edges(self) -> None:
        for e in self.edges_by_src:
            e.sort(key=lambda f: f.w)


def get_path(G: Graph, prev: List[int], dest: int) -> List[Tuple[int, int]]:
    path: deque = deque()
    vert = dest

    while prev[vert] != 0:
        pred = prev[vert]
        weight = G.get_edge(pred, vert).w
        segment = vert, weight
        path.appendleft(segment)
        vert = pred

    return list(path)


def dijkstra(G: Graph, src: int) -> Tuple[List[float], List[int]]:
    d = [math.inf for x in range(G.N + 1)]
    d[src] = 0.0

    prev = [0 for x in range(G.N + 1)]
    Q = HeapSort[int]()

    for v in G.vertices():
        Q.insert(v, d[v])

    idx = 0

    while not Q.empty():
        idx += 1
        u = Q.pop()
        if u is math.inf:
            return d, prev

        for e in G.get_incident_edges(u):
            _, v, w = e.unpack()
            new_d = d[u] + w
            if d[v] > new_d:
                d[v] = new_d
                prev[v] = u
                Q.priority(v, new_d)

    return d, prev


def kruskal(G: Graph) -> Tuple[List[Edge], float]:
    mst: List[Edge] = []
    weight: float = 0
    forest: Forest = Forest()

    edges = G.edges()
    edges.sort(key=lambda x: x.w)

    for v in G.vertices():
        forest.make_set(v)

    for e in edges:
        u, v, w = e.unpack()
        if forest.find(u) != forest.find(v):
            mst.append(e)
            weight += w
            forest.union(u, v)

    return mst, weight


def prim(G: Graph, r: int) -> Tuple[List[Edge], float]:
    key: List[float] = [math.inf for _ in range(G.N + 1)]
    pred: List[int] = [0 for _ in range(G.N + 1)]

    Q: HeapSort = HeapSort[int]()

    key[r] = 0

    for v in G.vertices():
        Q.insert(v, key[v])

    while not Q.empty():
        u = Q.pop()
        for e in G.get_incident_edges(u):
            _, v, w = e.unpack()
            if Q.in_queue(v) and e.w < key[v]:
                pred[v] = u
                key[v] = e.w
                Q.priority(v, e.w)

    mst: List[Edge] = [
        Edge(u, pred[u], key[u])
        for u in range(1, G.N + 1)
        if u != r
    ]

    weight: float = 0
    for v in range(1, G.N + 1):
        weight += key[v]

    return mst, weight

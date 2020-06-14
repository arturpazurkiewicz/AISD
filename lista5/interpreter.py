from tools import HeapSort
from typing import List, Callable, Tuple


def empty_pom(heap: HeapSort, _: List[str]) -> None:
    print(1 if heap.empty() else 0)


def insert_pom(heap: HeapSort, tokens: List[str]) -> None:
    try:
        item = int(tokens[1])
        priority = int(tokens[2])
        heap.insert(item, priority)
    except ValueError:
        print("Invalid input format")


def top_pom(heap: HeapSort, _: List[str]) -> None:
    print(heap.top() if not heap.empty() else "")


def pop_pom(heap: HeapSort, _: List[str]) -> None:
    print(heap.pop() if not heap.empty() else "")


def priority_pom(heap: HeapSort, tokens: List[str]) -> None:
    try:
        item = int(tokens[1])
        priority = int(tokens[2])
        heap.priority(item, priority)
    except ValueError:
        print("Invalid input format")


def print_pom(heap: HeapSort, _: List[str]) -> None:
    print(heap.data)


def choose_function(heap: HeapSort, tokens: List[str]) -> None:
    actions: List[Tuple[str, Callable]] = [
        ("insert", insert_pom),
        ("empty", empty_pom),
        ("top", top_pom),
        ("pop", pop_pom),
        ("priority", priority_pom),
        ("print", print_pom)
    ]

    for (cmd, act) in actions:
        if cmd == tokens[0]:
            act(heap, tokens)
            break
    else:
        print("Unknown command: " + tokens[0])


def interpret(heap: HeapSort, input: str) -> None:
    tokens = input.rstrip().split(' ')
    choose_function(heap, tokens)
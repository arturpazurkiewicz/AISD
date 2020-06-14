
from tools import HeapSort
from sys import stdin
from interpreter import interpret


if __name__ == "__main__":
    n = 0
    try:
        n = int(stdin.readline())
    except ValueError:
        print("Type valid n!")
        exit(-1)
    priority_heap = HeapSort[int]()
    for i in range(n):
        interpret(priority_heap, stdin.readline())

import math
import sys
import copy


def solve() -> int:
    sys.setrecursionlimit(200000)
    file = open("input.txt", "r")
    read = file.readlines()
    stripped = map(lambda s: s.replace("\n", ""), read)
    for i, l in enumerate(stripped):
        pass


if __name__ == "main":
    print(solve())

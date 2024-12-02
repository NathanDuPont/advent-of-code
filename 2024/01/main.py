import sys
from pathlib import Path


def solve() -> int:
    sys.setrecursionlimit(200000)
    file = open(Path(__file__).resolve().parent / "data/input.txt", "r")
    read = file.readlines()
    stripped = map(lambda s: s.replace("\n", ""), read)

    left = []
    right = []
    right_occurrences = {}
    for i, l in enumerate(stripped):
        a, b = l.split("   ")
        left.append(int(a))
        right.append(int(b))

        right_occurrences[int(b)] = right_occurrences.get(int(b), 0) + 1

    sorted_left = sorted(left)
    sorted_right = sorted(right)

    total = 0
    similarity = 0
    for i, num in enumerate(sorted_left):
        total += abs(sorted_left[i] - sorted_right[i])
        similarity += num * right_occurrences.get(num, 0)

    print(f"Difference: {total}")
    print(f"Similarity: {similarity}")


if __name__ == "__main__":
    solve()

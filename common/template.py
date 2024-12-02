from pathlib import Path
import sys


def configure():
    sys.setrecursionlimit(200000)


def solve():
    file_content = []
    with open(Path(__file__).resolve().parent / "data/input1.txt", "r") as f:
        lines = f.readlines()
        content = map(lambda s: s.replace("\n", ""), lines)

    print(f"Output: {'TODO'}")


if __name__ == "__main__":
    configure()
    solve()

# value, win, tie
mapping = {"X": (1, "C", "A"), "Y": (2, "A", "B"), "Z": (3, "B", "C")}


def execute():
    total = 0
    with open("input.txt", "r") as f:
        for line in f:
            opp, val = line.strip("\n").split(" ")

            self_val = mapping[val]

            win = 6 if opp == self_val[1] else 3 if opp == self_val[2] else 0

            total += self_val[0] + win

    print(total)


if __name__ == "__main__":
    execute()

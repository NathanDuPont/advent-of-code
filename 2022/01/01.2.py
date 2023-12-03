from queue import PriorityQueue

def execute():
    vals = PriorityQueue()
    with open("input.txt", "r") as f:
        cum_sum = 0
        for line in f:
            line = line.strip('\n')

            if len(line) == 0:
                vals.put(-1 * cum_sum)
                cum_sum = 0
            else:
                cum_sum += int(line)

    total_sum = 0
    for i, _ in enumerate(vals.queue):
        val = -1 * vals.get()

        print(f"{i}: {val + total_sum}")
        total_sum += val


if __name__ == "__main__":
    execute()
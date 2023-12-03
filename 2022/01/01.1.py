def execute():
    max_val = 0
    with open("input.txt", "r") as f:
        cum_sum = 0
        for line in f:
            line = line.strip('\n')

            if len(line) == 0:
                max_val = max(cum_sum, max_val)
                cum_sum = 0
            else:
                cum_sum += int(line)

    print(max_val)


if __name__ == "__main__":
    execute()
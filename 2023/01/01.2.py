import re 

num_pairs = [
    (r"one", "on1ne"),
    (r"two", "tw2wo"),
    (r"three", "thre3hree"),
    (r"four", "fou4our"),
    (r"five", "fiv5ive"),
    (r"six", "si6ix"),
    (r"seven", "seve7even"),
    (r"eight", "eigh8ight"),
    (r"nine", "nin9ine"),
]

if __name__ == "__main__":
    _sum_val = 0
    with open("input.txt", "r") as f:
        for line in f:
            for pattern, replace in num_pairs:
                line = re.sub(pattern, replace, line)                

            num_str = re.sub(r"\D", "", line)
            _sum_val += int(num_str[0] + num_str[-1])
    
    print(_sum_val)

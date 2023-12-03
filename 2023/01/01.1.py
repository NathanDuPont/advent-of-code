import re 

if __name__ == "__main__":
    _sum_val = 0
    with open("input.txt", "r") as f:
        for line in f:
            num_str = re.sub(r"\D", "", line)
            _sum_val += int(num_str[0] + num_str[-1])
    
    print(_sum_val)

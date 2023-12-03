import re 

# [(max red, max green, max blue)]

if __name__ == "__main__":
    _sum = 0
    with open("input.txt", "r") as f:
        for line in f:
            game_regex = re.compile(r"Game (\d+): ())", "", line)
            _sum += int(num_str[0] + num_str[-1])
    
    print(_sum)
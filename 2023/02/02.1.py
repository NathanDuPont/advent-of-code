from collections import namedtuple

if __name__ == "__main__":
    _max_cubes = []
    with open("input.txt", "r") as f:
        for line in f:
            max_colors = {"red": 0, "green": 0, "blue": 0}

            results = line.split(": ")[1].strip()
            game_results = results.split("; ")

            for game in game_results:
                game_values = game.split(", ")

                for value in game_values:
                    count, color = value.split(" ")

                    max_colors[color] = max(max_colors[color], int(count))

            _max_cubes.append(max_colors)

    MAX_RED = 12
    MAX_GREEN = 13
    MAX_BLUE = 14

    _game_id_sum = 0
    for i, game_results in enumerate(_max_cubes):
        if (
            game_results["red"] <= MAX_RED
            and game_results["green"] <= MAX_GREEN
            and game_results["blue"] <= MAX_BLUE
        ):
            _game_id_sum += i + 1

    print(_game_id_sum)

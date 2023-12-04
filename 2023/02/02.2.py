from collections import namedtuple

if __name__ == "__main__":
    _max_cubes = []
    with open("input.txt", "r") as f:
        for line in f:
            max_colors = {
                "red": 0,
                "green": 0,
                "blue": 0
            }

            results = line.split(": ")[1].strip()
            game_results = results.split("; ")

            for game in game_results:
                game_values = game.split(", ")

                for value in game_values:
                    count, color = value.split(" ")
                    
                    max_colors[color] = max(max_colors[color], int(count))
            
            _max_cubes.append(max_colors)

    _game_powers = 0
    for i, game_results in enumerate(_max_cubes):
        _game_powers += game_results["red"] * game_results["green"] * game_results["blue"]

    print(_game_powers)
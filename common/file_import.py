def read_file(file_path: str):
    lines = None
    with open(file_path, "r") as f:
        lines = f.readlines()

    return lines

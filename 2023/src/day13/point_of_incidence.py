def read_input(file_path):
    # Read the content of the file and split it into blocks separated by empty lines
    return [
        {
            (y, x)
            for y, line in enumerate(block.split("\n"))
            for x, c in enumerate(line)
            if c == "#"
        }
        for block in open(file_path).read().split("\n\n")
    ]


def find_reflection(grid, smudges=0):
    # Find the maximum x-coordinate in the grid
    max_x = max(x for y, x in grid)

    # Iterate through possible reflection lines
    for reflection_line in range(1, max_x + 1):
        # Calculate the reflection of the grid based on the current reflection line
        reflection = {(y, 2 * reflection_line - x - 1) for y, x in grid}

        # Determine the length of the reflection line within the grid bounds
        reflection_len = min(reflection_line, max_x - reflection_line + 1)

        # Find the remaining smudges after applying the reflection
        remaining = {
            (y, x)
            for y, x in (grid - reflection)
            if -reflection_len <= x - reflection_line < reflection_len
        }

        # If the number of remaining smudges matches the target, return the reflection line
        if len(remaining) == smudges:
            return reflection_line

    # If no reflection line is found, return 0
    return 0


def calculate_result(grid, rotated_grid, smudges):
    # Calculate the result based on the reflection of the original and rotated grids
    return 100 * find_reflection(rotated_grid, smudges) + find_reflection(grid, smudges)


def main():
    input_file = "input.txt"
    # Read the input grids from the file
    input_grid = read_input(input_file)
    # Create rotated versions of each input grid
    input_rotated_grid = [{(x, y) for y, x in grid} for grid in input_grid]

    # Calculate results for both scenarios (smudges=0 and smudges=1)
    result1 = sum(
        calculate_result(grid, rotated_grid, 0)
        for grid, rotated_grid in zip(input_grid, input_rotated_grid)
    )
    result2 = sum(
        calculate_result(grid, rotated_grid, 1)
        for grid, rotated_grid in zip(input_grid, input_rotated_grid)
    )

    # Print the results
    print(result1)
    print(result2)


if __name__ == "__main__":
    main()

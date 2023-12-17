if __name__ == "__main__":
    # Read the input from the file and transpose it to make rows columns and vice versa
    with open("input.txt") as file:
        dish = [list(line) for line in zip(*file.read().splitlines())]

    load_on_beams = 0
    # Iterate through each line in the transposed dish
    for line in dish:
        last = 0
        # Iterate through each character in the line
        for i, char in enumerate(line):
            # If the character is "O", swap it with the previous ".", updating load_on_beams
            if char == "O":
                line[i] = "."
                line[last] = "O"
                load_on_beams += len(line) - last
                last += 1
            # If the character is "#", update the last index
            elif char == "#":
                last = i + 1

    # Print the result for Part 1
    print("Part 1:", load_on_beams)


def spin_cycle(dish) -> tuple:
    # Convert the input dish to a list of lists
    dish_list = list(map(list, dish))
    # Perform four spin cycles (90-degree rotations)
    for _ in range(4):
        for line in dish_list:
            last = 0
            # Iterate through each character in the line
            for i, char in enumerate(line):
                # If the character is "O", swap it with the previous ".", updating load_on_beams
                if char == "O":
                    line[i] = "."
                    line[last] = "O"
                    last += 1
                # If the character is "#", update the last index
                elif char == "#":
                    last = i + 1
        # Reverse the order of the rows to simulate a 90-degree rotation
        dish_list = [list(line) for line in reversed(list(zip(*dish_list)))]
    # Convert the rotated dish back to a tuple of tuples
    return tuple(map(tuple, dish_list))


if __name__ == "__main__":
    # Read the original dish from the file and transpose it
    with open("input.txt") as file:
        original_dish = tuple(zip(*file.read().splitlines()))

    dish_states = [original_dish]
    # Find the repeating pattern by repeatedly applying spin_cycle
    while True:
        new_dish = spin_cycle(dish_states[-1])
        # If the new state is already in dish_states, a loop is found
        if new_dish in dish_states:
            break
        dish_states.append(new_dish)

    # Identify the start of the loop and calculate the loop length
    loop_start = dish_states.index(new_dish)
    loop_length = len(dish_states) - loop_start
    rest_rounds = (10**9 - loop_start) % loop_length
    # Calculate the final state after 10^9 rounds
    final_dish = dish_states[loop_start + rest_rounds]

    load_on_beams = 0
    # Iterate through the final state to calculate load_on_beams
    for line in final_dish:
        for i, char in enumerate(line):
            if char == "O":
                load_on_beams += len(line) - i

    # Print the result for Part 2
    print("Part 2:", load_on_beams)

# Function to read input from a file and parse it into a list of tuples
def read_input(file_path):
    with open(file_path) as file:
        # Each line is split into a tuple containing springs and groups
        return [
            (springs, list(map(int, groups.split(","))))
            for springs, groups in (line.split() for line in file.read().splitlines())
        ]


# Function to solve the problem using memoization
def solve_memoization(springs, groups, i, j, cache):
    # Base case: if all springs are processed
    if i == len(springs):
        return j == len(groups), cache

    # Base case: if all groups are placed
    if j == len(groups):
        return "#" not in springs[i:], cache

    # Check if the result for the current state is already computed and stored in the cache
    if (i, j) in cache:
        return cache[(i, j)], cache

    total = 0
    # If the current spring is either '.' or '?'
    if springs[i] in ".?":
        # Recursively call the function with the next spring and the same group index
        new_total, cache = solve_memoization(springs, groups, i + 1, j, cache)
        total += new_total

    # If the current spring is '#' or '?'
    current_group_len = groups[j]
    if (
        springs[i] in "#?"
        and len(springs) >= i + current_group_len
        and "." not in springs[i : i + current_group_len]
        and springs[i + current_group_len] != "#"
    ):
        # Recursively call the function with the next available position and the next group
        new_total, cache = solve_memoization(
            springs, groups, i + current_group_len + 1, j + 1, cache
        )
        total += new_total

    # Store the computed result in the cache
    cache[(i, j)] = total
    return total, cache


# Function to solve the problem using dynamic programming
def solve_dp(springs, groups):
    # Add padding to the springs to simplify boundary conditions
    springs = ".." + springs.strip(".")
    # Initialize a list to store the intermediate results
    dp = [0] * len(springs)

    # Find the index of the first '#' in springs
    first_hash_index = next(
        (i for i, c in enumerate(springs) if c == "#"), len(springs)
    )
    # Initialize the dp list with 1s for positions before the first '#'
    for i in range(first_hash_index):
        dp[i] = 1

    # Iterate through each group size
    for count in groups:
        # Create a new dp list for the current group size
        n_dp = [0] * len(springs)

        # Iterate through each position in springs
        for i, c in enumerate(springs):
            if c == "#":
                continue

            # Copy the value from the previous position
            n_dp[i] = n_dp[i - 1] if i > 0 else 0

            # Update the value based on the current group size
            if i - count >= 0 and springs[i - count] != "#":
                n_dp[i] += dp[i - count - 1]

        # Update the dp list for the current group size
        dp = n_dp

    # The last value in dp contains the total number of valid arrangements
    return dp[-1]


# Main function
def main():
    input_file = "input.txt"
    input_data = read_input(input_file)

    # Solve the problem for each input and sum the results
    result1 = sum(solve_dp(spring, group) for spring, group in input_data)
    result2 = sum(
        solve_dp("?".join([spring] * 5), group * 5) for spring, group in input_data
    )

    # Print the results
    print(f"Result 1: {result1}")
    print(f"Result 2: {result2}")


# Entry point of the script
if __name__ == "__main__":
    main()

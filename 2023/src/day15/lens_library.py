# Function to calculate a hash value for a given string
def get_hash(string):
    current_value = 0
    # Iterate through each character in the string
    for char in string:
        # Update the current hash value using a simple hashing algorithm
        current_value = ((current_value + ord(char)) * 17) % 256
    return current_value


if __name__ == "__main__":
    # Read the lines from the input file, remove leading/trailing whitespace, and split by commas
    with open("input.txt") as file:
        lines = file.read().strip().split(",")
    # Calculate the sum of hash values for each line and print the result for Part 1
    print("Part 1:", sum(get_hash(line) for line in lines))


# Function to calculate a hash value for a given string and manage a list of boxes
def get_hash(string):
    current_value = 0
    # Iterate through each character in the string
    for char in string:
        # Update the current hash value using a simple hashing algorithm
        current_value = ((current_value + ord(char)) * 17) % 256
    return current_value


if __name__ == "__main__":
    # Read the lines from the input file, remove leading/trailing whitespace, and split by commas
    with open("input.txt") as file:
        lines = file.read().strip().split(",")

    # Initialize a list of dictionaries (boxes)
    boxes_list = [{} for _ in range(256)]

    # Iterate through each line in the input
    for line in lines:
        # Check if the line ends with a hyphen
        if line[-1] == "-":
            # If it does, remove the hyphen and use the label to find the corresponding box
            label = line[:-1]
            box = boxes_list[get_hash(label)]
            # If the label is already in the box, remove it
            if label in box:
                box.pop(label)
        else:
            # If the line doesn't end with a hyphen, split it into label and number
            label, number = line.split("=")
            number = int(number)
            # Use the label to find the corresponding box and update its content
            box = boxes_list[get_hash(label)]
            box[label] = number

    # Calculate the focusing power sum for Part 2
    focusing_power_sum = 0
    for box_num, box in enumerate(boxes_list):
        for slot_num, focal_length in enumerate(box.values(), 1):
            focusing_power_sum += (box_num + 1) * slot_num * focal_length

    # Print the result for Part 2
    print("Part 2:", focusing_power_sum)

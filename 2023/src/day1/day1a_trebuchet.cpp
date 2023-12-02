#include <iostream>
#include <fstream>
#include <string>

int main() {
    // Open the input file
    std::ifstream input_file("input.txt");

    // Check if the file is opened successfully
    if (!input_file.is_open()) {
        std::cerr << "Error opening input file!" << std::endl;
        return EXIT_FAILURE;
    }

    // Variable to store the sum of calibration values
    int sum = 0;

    // Iterate through each line in the calibration document
    std::string line;
    while (std::getline(input_file, line)) {
        // Find the first and last characters in the line
        char first_digit = 0;
        char last_digit = 0;

        for (char ch : line) {
            if (std::isdigit(ch)) {
                if (first_digit == 0) {
                    first_digit = ch;
                }
                last_digit = ch;
            }
        }

        // Convert characters to integers and add to the sum
        if (first_digit != 0 && last_digit != 0) {
            int value = (first_digit - '0') * 10 + (last_digit - '0');
            sum += value;
        }
    }

    // Close the input file
    input_file.close();

    // Print the sum of calibration values
    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}

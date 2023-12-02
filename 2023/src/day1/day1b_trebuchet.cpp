#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
    // Open the input file
    std::ifstream input_file("input.txt");

    // Check if the file is opened successfully
    if (!input_file.is_open()) {
        std::cerr << "Error opening input file!" << std::endl;
        return EXIT_FAILURE;
    }

    // Define a vector of strings representing the names of numbers
    const std::vector<std::string> numbers{"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    // Vector to store the calculated calibration values
    std::vector<int> calibration_values;

    // Read each line from the input file
    std::string line;
    while (std::getline(input_file, line)) {
        // Vector to store digits extracted from the line
        std::vector<int> digits;

        // Iterate through each character in the line
        for (int i = 0; i < line.size(); ++i) {
            const auto &c = line[i];

            // Check if the character is a digit
            if (std::isdigit(c)) {
                digits.push_back(c - '0');
            }

            // Check if the substring matches any of the named numbers
            for (int j = 0; j < numbers.size(); ++j) {
                const auto &n = numbers[j];
                if (line.size() >= i + n.size() && line.substr(i, n.size()) == n) {
                    digits.push_back(j + 1);
                }
            }
        }

        // Calculate and store the calibration value for the current line
        calibration_values.push_back(digits[0] * 10 + digits.back());
    }

    // Calculate the sum of calibration values using std::accumulate
    const auto sum = std::accumulate(std::begin(calibration_values), std::end(calibration_values), 0);

    // Print the sum of calibration values
    std::cout << sum << '\n';

    return EXIT_SUCCESS;
}

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <unordered_map>

// Helper function to convert spelled-out digits to numeric values
int wordToDigitValue(const std::string& word) {
    std::unordered_map<std::string, int> wordToDigit = {
        {"one", 1},
        {"two", 2},
        {"three", 3},
        {"four", 4},
        {"five", 5},
        {"six", 6},
        {"seven", 7},
        {"eight", 8},
        {"nine", 9}
    };

    auto it = wordToDigit.find(word);
    return (it != wordToDigit.end()) ? it->second : -1;
}

int main() {
    // Open the input file
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    int sum = 0;

    // Loop through each line in the calibration document
    std::string line;
    while (std::getline(inputFile, line)) {
        int start = -1;
        int end = -1;

        // Find the first and last digit on each line
        for (size_t i = 0; i < line.length(); ++i) {
            if (std::isdigit(line[i])) {
                if (start == -1) {
                    start = line[i] - '0';
                }
                end = line[i] - '0';
            } else if (std::isalpha(line[i])) {
                // Check for spelled-out digits
                std::string word;
                while (std::isalpha(line[i])) {
                    word += line[i];
                    ++i;
                }

                int wordDigitValue = wordToDigitValue(word);
                if (wordDigitValue != -1) {
                    // Found a spelled-out digit
                    if (start == -1) {
                        start = wordDigitValue;
                    }
                    end = wordDigitValue;
                }
            }
        }

        // Calculate and add the calibration value to the sum
        if (start != -1 && end != -1) {
            int value = start * 10 + end;
            sum += value;
        }
    }

    // Close the input file
    inputFile.close();

    // Output the sum of all calibration values
    std::cout << "Sum of calibration values: " << sum << std::endl;

    return 0;
}

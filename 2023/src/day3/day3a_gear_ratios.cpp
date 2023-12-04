#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>

/// @brief Struct to represent a number and its location in the map
struct Number
{
    int row;   ///< Row index in the map
    int col;   ///< Column index in the map
    int value; ///< Numeric value of the number
    int order; ///< Number of digits in the number
};

/// @brief Function to add a number to the 'numbers' vector
void addNumber(std::vector<Number> &numbers, int row, int col, int value, int order)
{
    Number n{row, col, value, order};
    numbers.push_back(n);
}

int main(int argc, char *argv[])
{
    std::ifstream inputFile("input.txt");

    // Check if the file is opened successfully
    if (!inputFile.is_open())
    {
        std::cerr << "Error opening input file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    std::vector<std::string> map; ///< Vector to store lines of the map
    std::vector<Number> numbers;  ///< Vector to store numbers and their location in the map

    // Process each line in the file
    while (std::getline(inputFile, line))
    {
        map.push_back(line);
        bool isParsingNumber = false;
        int currentNumber = -1;
        int startCol = 0;
        int currentOrder = 0;

        // Parse each character in the line
        for (int i = 0; i < line.size(); i++)
        {
            const char currentChar = line[i];

            if (std::isdigit(currentChar))
            {
                if (!isParsingNumber)
                {
                    currentNumber = 0;
                    startCol = i;
                    currentOrder = 0;
                }
                isParsingNumber = true;
                currentNumber = (currentNumber * 10) + static_cast<int>(currentChar - '0');
                currentOrder++;
            }
            else
            {
                isParsingNumber = false;
            }

            if (currentNumber != -1 && !isParsingNumber)
            {
                addNumber(numbers, map.size() - 1, startCol, currentNumber, currentOrder);
                currentNumber = -1;
                startCol = 0;
                currentOrder = 0;
            }
        }

        if (currentNumber != -1)
        {
            addNumber(numbers, map.size() - 1, startCol, currentNumber, currentOrder);
        }
    }

    std::vector<int> overlappingNumbers;

    // Check for overlapping numbers in the map
    for (const auto &num : numbers)
    {
        bool stillSearching = true;

        // Check the points around the number in the map
        for (int row = num.row - 1; (row <= num.row + 1) && stillSearching; row++)
        {
            if (row < 0 || row >= map.size())
                continue;

            // Only need to check the first and last element in the row that contains the number
            const int colIncrement = (row == num.row) ? num.order + 1 : 1;

            for (int col = num.col - 1; (col <= num.col + num.order) && stillSearching; col += colIncrement)
            {
                if (col < 0 || col >= map[row].size())
                    continue;

                // Check if the current point in the map contains a digit or not
                if (map[row][col] != '.' && !(std::isdigit(map[row][col])))
                {
                    stillSearching = false;
                    overlappingNumbers.push_back(num.value);
                }
            }
        }
    }

    // Calculate and print the total of overlapping numbers
    const auto total = std::accumulate(std::begin(overlappingNumbers), std::end(overlappingNumbers), 0);
    std::cout << total << std::endl;

    return EXIT_SUCCESS;
}

#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

/// @brief Struct to represent a number and its location in the map
struct Number
{
    int col_;    ///< Column index in the map
    int row_;    ///< Row index in the map
    int number_; ///< Numeric value of the number
    int order_;  ///< Number of digits in the number
};

/// @brief Struct to represent a gear in the map
struct Gear
{
    int n_ = 0;                ///< Number of numbers on the gear
    std::vector<int> numbers_; ///< Vector to store numbers present on the gear
};

/// @brief Function to add a number to the 'numbers' vector
void addNumber(std::vector<Number> &numbers, int row, int start, int number, int order)
{
    Number n;
    n.row_ = row;
    n.col_ = start;
    n.number_ = number;
    n.order_ = order;
    numbers.push_back(n);
}

int main(int argc, char *argv[])
{
    std::ifstream input_file("input.txt");

    // Check if the file is opened successfully
    if (!input_file.is_open())
    {
        std::cerr << "Error opening input file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    std::vector<std::string> map;
    std::vector<Number> numbers; // store numbers and their location

    // Process each line in the file
    while (std::getline(input_file, line))
    {
        map.push_back(line);
        bool is_number = false;
        int number = -1;
        int start = 0;
        int order = 0;

        // Lambda function to add the parsed number to the 'numbers' vector
        const auto add_util = [&]()
        {
            addNumber(numbers, map.size() - 1, start, number, order);
            number = -1;
            start = 0;
            order = 0;
        };

        // Parse each character in the line
        for (int i = 0; i < line.size(); i++)
        {
            const char c = line[i];

            if (c >= '0' && c <= '9')
            {
                if (is_number == false)
                {
                    number = 0;
                    start = i;
                    order = 0;
                }
                is_number = true;
                number = (number * 10) + int(c - '0');
                order++;
            }
            else
            {
                is_number = false;
            }

            if (number != -1 && !is_number)
            {
                add_util();
            }
        }

        if (number != -1)
        {
            add_util();
        }
    }

    std::unordered_map<int, Gear> gears;

    // Check for gears in the map and count the numbers on each gear
    for (const auto &n : numbers)
    {
        for (int row = n.row_ - 1; (row <= n.row_ + 1); row++)
        {
            if (row < 0 || row >= map.size())
                continue;

            const int col_increment = (row == n.row_) ? n.order_ + 1 : 1;

            for (int col = n.col_ - 1; (col <= n.col_ + n.order_); col += col_increment)
            {
                if (col < 0 || col >= map[row].size())
                    continue;

                if (map[row][col] == '*')
                {
                    const int id = map[0].size() * row + col; // assumes each row is the same length

                    if (gears.find(id) == gears.end())
                    {
                        Gear g;
                        g.n_ = 1;
                        g.numbers_.push_back(n.number_);
                        gears[id] = g;
                    }
                    else
                    {
                        gears[id].n_++;
                        gears[id].numbers_.push_back(n.number_);
                    }
                }
            }
        }
    }

    std::size_t ans = 0;

    // Calculate and print the product of numbers on gears with exactly two numbers
    for (const auto &[id, gear] : gears)
    {
        if (gear.n_ == 2)
        {
            ans += (gear.numbers_[0] * gear.numbers_[1]);
        }
    }

    std::cout << ans << std::endl;

    return EXIT_SUCCESS;
}

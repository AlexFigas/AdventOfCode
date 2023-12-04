#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

// Offset for extracting card ID from the input line
const std::size_t kCardIdOffset = 5;

/// @brief Struct to represent a card with its ID, winning numbers, numbers on the card, and copies
struct Card
{
    int id;                           ///< Unique identifier for the card
    std::vector<int> winning_numbers; ///< Winning numbers associated with the card
    std::vector<int> numbers_on_card; ///< Numbers present on the card
    std::size_t copies;               ///< Number of copies of the card
};

/// @brief Function to extract numbers from a string and return them as a vector of integers
std::vector<int> extract_numbers(const std::string &s)
{
    std::cout << s << '\n'; // Print the input string (for debugging purposes)
    std::vector<int> numbers;

    // Iterate through the string, extracting two-digit numbers at intervals of 3 characters
    for (std::size_t i = 0; i < s.size(); i += 3)
    {
        // Determine the starting position for extracting the two-digit number
        std::size_t start = (s[i] == ' ') ? i + 1 : i;
        // Extract two-digit number and convert it to an integer, then add to the vector
        numbers.push_back(std::stoi(s.substr(start, 2)));
    }

    return numbers;
}

int main()
{
    std::ifstream input_file("input.txt");

    // Check if the input file is successfully opened
    if (!input_file.is_open())
    {
        std::cerr << "Error opening file.\n";
        return EXIT_FAILURE;
    }

    std::size_t total_points = 0; // Accumulator for total points
    std::vector<Card> cards;

    // Iterate through each line in the input file
    std::string line;
    while (std::getline(input_file, line))
    {
        Card c;
        std::size_t end = line.find(':');
        c.id = std::stoi(line.substr(kCardIdOffset, end - kCardIdOffset));

        // Find the starting position for extracting winning numbers and extract them
        std::size_t start = end + 2;
        end = line.find('|', start);
        c.winning_numbers = extract_numbers(line.substr(start, end - start));

        // Find the starting position for extracting card numbers and extract them
        c.numbers_on_card = extract_numbers(line.substr(end + 2));

        c.copies = 1;
        cards.emplace_back(c);
    }

    // Calculate the total points based on the winning and card numbers
    for (const auto &card : cards)
    {
        int count = std::count_if(card.numbers_on_card.begin(), card.numbers_on_card.end(),
                                  [&card](int num)
                                  {
                                      return std::find(card.winning_numbers.begin(), card.winning_numbers.end(), num) !=
                                             card.winning_numbers.end();
                                  });

        if (count > 0)
        {
            total_points += std::pow(2, count - 1);
        }
    }

    // Print the total points
    std::cout << total_points << std::endl;

    return EXIT_SUCCESS;
}

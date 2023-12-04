#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>

/// @brief Function to extract numbers from a string and return them as a vector of integers
std::vector<int> extract_numbers(const std::string &s)
{
    std::istringstream iss(s);
    std::vector<int> numbers;
    int num;
    while (iss >> num)
    {
        numbers.push_back(num);
    }
    return numbers;
}

/// @brief Struct to represent a card with its ID, winning numbers, numbers on the card, and copies
struct Card
{
    int id;                           ///< Unique identifier for the card
    std::vector<int> winning_numbers; ///< Winning numbers associated with the card
    std::vector<int> numbers_on_card; ///< Numbers present on the card
    std::size_t copies;               ///< Number of copies of the card
};

int main()
{
    std::ifstream file("input.txt");

    std::size_t total_points = 0;
    std::vector<Card> cards;

    std::string line;
    while (std::getline(file, line))
    {
        Card c;
        std::size_t start = line.find(':') + 2;
        std::size_t end = line.find('|', start);

        c.id = std::stoi(line.substr(5, end - 5));
        c.winning_numbers = extract_numbers(line.substr(start, end - start));
        c.numbers_on_card = extract_numbers(line.substr(end + 2));

        c.copies = 1;
        cards.emplace_back(c);
    }

    // Iterate through each card and update the number of copies for overlapping cards
    for (const auto &card : cards)
    {
        int count = std::count_if(card.numbers_on_card.begin(), card.numbers_on_card.end(),
                                  [&card](int num)
                                  {
                                      return std::find(card.winning_numbers.begin(), card.winning_numbers.end(), num) !=
                                             card.winning_numbers.end();
                                  });

        for (std::size_t i = card.id; i < std::min(cards.size(), static_cast<std::size_t>(card.id + count)); ++i)
        {
            cards[i].copies += card.copies;
        }
    }

    // Calculate and print the total number of copies
    std::cout << std::accumulate(cards.begin(), cards.end(), 0, [](std::size_t total, const Card &card)
                                 { return total + card.copies; })
              << std::endl;

    return EXIT_SUCCESS;
}

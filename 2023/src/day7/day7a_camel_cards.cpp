#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>

/// @brief Represents a hand of cards
struct Hand
{
    std::string cards;
    std::array<int, 5> labelValues; ///< Numeric values corresponding to card labels
    int type;                       ///< Type of hand (e.g., straight, full house, etc.)
};

constexpr std::array<char, 13> labels{{'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'}};

/// @brief Determines the type of hand based on the given cards
int getType(const std::string &cards)
{
    std::unordered_map<char, int> cardCount;
    for (const auto card : cards)
    {
        if (cardCount.find(card) == cardCount.end())
        {
            cardCount[card] = 0;
        }
        cardCount[card]++;
    }

    if (cardCount.size() == 1)
    {
        return 6; // All cards of the same rank
    }
    if (cardCount.size() == 2 && (cardCount[cards[0]] == 4 || cardCount[cards[0]] == 1))
    {
        return 5; // Four of a kind or a single card
    }
    if (cardCount.size() == 2 && (cardCount[cards[0]] == 3 || cardCount[cards[0]] == 2))
    {
        return 4; // Full house or two pairs
    }
    int countPairs = 0;
    int countThreeOfAKind = 0;
    for (const auto &[card, count] : cardCount)
    {
        if (count == 2)
            countPairs++;
        if (count == 3)
            countThreeOfAKind++;
    }
    if (countThreeOfAKind == 1)
        return 3; // Three of a kind
    if (countPairs == 2)
        return 2; // Two pairs
    if (countPairs == 1)
        return 1; // One pair
    return 0; // High card
}

/// @brief Gets the numeric value of a card label
int getValue(const char label)
{
    return std::distance(std::begin(labels), std::find(std::begin(labels), std::end(labels), label));
}

/// @brief Parses a line to extract a hand and bid value
std::pair<Hand, int> getHandAndBid(const std::string &line)
{
    const auto idx = line.find(' ');
    Hand hand;
    hand.cards = line.substr(0, idx);
    hand.type = getType(hand.cards);
    for (int i = 0; i < hand.cards.size(); i++)
    {
        hand.labelValues[i] = getValue(hand.cards[i]);
    }

    return std::make_pair(hand, std::stoi(line.substr(idx, line.size() - idx)));
}

/// @brief Compares two hands for sorting
bool compareHands(const Hand &hand1, const Hand &hand2)
{
    if (hand1.type > hand2.type)
    {
        return true;
    }
    else if (hand1.type < hand2.type)
    {
        return false;
    }
    for (int i = 0; i < 5; i++)
    {
        if (hand1.labelValues[i] > hand2.labelValues[i])
            return true;
        if (hand1.labelValues[i] < hand2.labelValues[i])
            return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    std::ifstream file("input.txt");
    if (!file.is_open())
    {
        std::cerr << "Error opening input file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    std::vector<std::pair<Hand, int>> handsAndBids;
    while (std::getline(file, line))
    {
        handsAndBids.emplace_back(getHandAndBid(line));
    }

    std::sort(std::begin(handsAndBids),
              std::end(handsAndBids),
              [](const auto &handAndBid1, const auto &handAndBid2)
              {
                  return !compareHands(handAndBid1.first, handAndBid2.first);
              });

    std::size_t result = 0;
    for (int i = 0; i < handsAndBids.size(); i++)
    {
        result += (i + 1) * handsAndBids[i].second;
    }
    std::cout << result << '\n';
    return EXIT_SUCCESS;
}

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>

struct Hand
{
    std::string cards;
    std::array<int, 5> labelValues;
    int type;
};

constexpr std::array<char, 13> labels{{'J', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'Q', 'K', 'A'}};

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
        return 6;
    }
    if (cardCount.size() == 2 && (cardCount['J'] != 0))
    {
        return 6;
    }

    if (cardCount.size() == 2 && (cardCount[cards[0]] == 4 || cardCount[cards[0]] == 1))
    {
        return 5;
    }
    if (cardCount.size() == 3)
    {
        for (const auto &[card, n] : cardCount)
        {
            if (card == 'J')
                continue;
            if (n + cardCount['J'] == 4)
                return 5;
        }
    }

    if (cardCount.size() == 2 && (cardCount[cards[0]] == 3 || cardCount[cards[0]] == 2))
    {
        return 4;
    }
    if (cardCount.size() == 3)
    {
        for (const auto &[card, n] : cardCount)
        {
            if (card == 'J')
                continue;
            if (n + cardCount['J'] == 3)
                return 4;
        }
    }

    int countPairs = 0;
    int countThreeOfAKind = 0;
    int single = 0;
    for (const auto &[card, n] : cardCount)
    {
        if (card == 'J')
            continue;
        if (n == 1)
            single++;
        if (n == 2)
            countPairs++;
        if (n == 3)
            countThreeOfAKind++;
    }
    if (countThreeOfAKind == 1)
        return 3;
    for (const auto [card, n] : cardCount)
    {
        if (card == 'J')
            continue;
        if (n + cardCount['J'] == 3)
            return 3;
    }
    if (countPairs > 0 && cardCount['J'] > 0)
        return 3;
    if (countPairs == 2)
        return 2;
    if (countPairs + std::min(single, cardCount['J']) == 2)
        return 2;
    if (countPairs == 1)
        return 1;
    for (const auto [card, n] : cardCount)
    {
        if (card == 'J')
            continue;
        if (n + cardCount['J'] == 2)
            return 1;
    }
    return 0;
}

int getValue(const char label)
{
    return std::distance(std::begin(labels), std::find(std::begin(labels), std::end(labels), label));
}

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

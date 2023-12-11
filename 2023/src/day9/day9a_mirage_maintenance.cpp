#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Recursively finds the next value in a sequence.
 *
 * @param valueSequence The sequence of values.
 * @return The next value in the sequence.
 */
int findNextValue(std::vector<int> &valueSequence)
{
    // If there's only one element, duplicate it
    if (valueSequence.size() == 1)
    {
        valueSequence.push_back(valueSequence.back());
        return valueSequence.back();
    }

    // Calculate the difference between consecutive elements
    std::vector<int> newValueSequence;
    newValueSequence.reserve(valueSequence.size() - 1);

    for (size_t i = 0; i < valueSequence.size() - 1; ++i)
    {
        newValueSequence.push_back(valueSequence[i + 1] - valueSequence[i]);
    }

    // If there are non-zero differences, recursively find the next value
    if (std::any_of(valueSequence.begin(), valueSequence.end(), [](const auto &ele)
                    { return ele != 0; }))
    {
        const auto valueToAdd = findNextValue(newValueSequence);
        valueSequence.push_back(valueSequence.back() + valueToAdd);
        return valueSequence.back();
    }

    // If all differences are zero, duplicate the last element
    valueSequence.push_back(valueSequence.back());
    return valueSequence.back();
}

/**
 * @brief Extracts numbers from a space-separated string.
 *
 * @param s The input string.
 * @return A vector of integers extracted from the string.
 */
std::vector<int> extractNumber(const std::string &s)
{
    std::vector<int> numbers;
    size_t start = 0;
    size_t end = s.find(' ', start);

    // Extract numbers separated by spaces
    while (end != std::string::npos)
    {
        numbers.push_back(std::stoi(s.substr(start, end - start)));
        start = end + 1;
        end = s.find(' ', start);
    }

    // Extract the last number
    numbers.push_back(std::stoi(s.substr(start, s.size() - start)));
    return numbers;
}

int main(int argc, char *argv[])
{
    std::string line;
    std::ifstream file("input.txt");
    if (!file.is_open())
    {
        std::cerr << "Error opening input file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<std::vector<int>> valueSequences;

    // Read lines from the file and extract numbers into sequences
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        valueSequences.push_back(extractNumber(line));
    }

    int total = 0;

    // Calculate the total of the next values for each sequence
    for (auto &valueSequence : valueSequences)
    {
        const int newValue = findNextValue(valueSequence);
        total += newValue;
    }

    // Print the total
    std::cout << total << '\n';

    return 0;
}

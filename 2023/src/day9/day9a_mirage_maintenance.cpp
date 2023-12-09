#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int findNextValue(std::vector<int> &valueSequence)
{
    if (valueSequence.size() == 1)
    {
        valueSequence.push_back(valueSequence.back());
        return valueSequence.back();
    }

    std::vector<int> newValueSequence;
    newValueSequence.reserve(valueSequence.size() - 1);

    for (size_t i = 0; i < valueSequence.size() - 1; ++i)
    {
        newValueSequence.push_back(valueSequence[i + 1] - valueSequence[i]);
    }

    if (std::any_of(valueSequence.begin(), valueSequence.end(), [](const auto &ele)
                    { return ele != 0; }))
    {
        const auto valueToAdd = findNextValue(newValueSequence);
        valueSequence.push_back(valueSequence.back() + valueToAdd);
        return valueSequence.back();
    }

    valueSequence.push_back(valueSequence.back());
    return valueSequence.back();
}

std::vector<int> extractNumber(const std::string &s)
{
    std::vector<int> numbers;
    size_t start = 0;
    size_t end = s.find(' ', start);

    while (end != std::string::npos)
    {
        numbers.push_back(std::stoi(s.substr(start, end - start)));
        start = end + 1;
        end = s.find(' ', start);
    }

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

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        valueSequences.push_back(extractNumber(line));
    }

    int total = 0;

    for (auto &valueSequence : valueSequences)
    {
        const int newValue = findNextValue(valueSequence);
        total += newValue;
    }

    std::cout << total << '\n';

    return 0;
}

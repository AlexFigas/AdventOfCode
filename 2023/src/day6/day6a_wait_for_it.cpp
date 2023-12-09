#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

std::vector<int> extractNumbers(const std::string &s, const int delta)
{
    std::vector<int> numbers;
    std::size_t start = s.find_first_not_of(' ', delta);
    std::size_t end = s.find(' ', start);
    while (end != std::string::npos)
    {
        numbers.push_back(std::stoi(s.substr(start, end - start)));
        start = s.find_first_not_of(' ', end + 1);
        end = s.find(' ', start);
    }
    numbers.push_back(std::stoi(s.substr(start, s.size() - start)));
    return numbers;
}

int main(int argc, char *argv[])
{
    std::fstream file("input.txt");
    if (!file.is_open())
    {
        std::cerr << "Error opening input file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    std::getline(file, line);
    const auto raceTimes = extractNumbers(line, 5);

    std::getline(file, line);
    const auto currentRecords = extractNumbers(line, 9);

    std::size_t result = 1;
    for (int i = 0; i < currentRecords.size(); i++)
    {
        const double discriminantSqrt = std::sqrt(raceTimes[i] * raceTimes[i] - 4 * (1) * (currentRecords[i]));

        int possibleRoot1 = std::floor(raceTimes[i] + discriminantSqrt) / 2;
        int possibleRoot2 = std::ceil(raceTimes[i] - discriminantSqrt) / 2;

        if (possibleRoot1 * (raceTimes[i] - possibleRoot1) <= currentRecords[i])
        {
            possibleRoot1--;
        }
        if (possibleRoot2 * (raceTimes[i] - possibleRoot2) <= currentRecords[i])
        {
            possibleRoot2++;
        }

        result *= (possibleRoot1 - possibleRoot2 + 1);
    }

    std::cout << result << '\n';
    return EXIT_SUCCESS;
}

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

long long extractNumber(const std::string &s, const int delta)
{
    std::string number;
    std::size_t start = s.find_first_not_of(' ', delta);
    std::size_t end = s.find(' ', start);
    while (end != std::string::npos)
    {
        number += s.substr(start, end - start);
        start = s.find_first_not_of(' ', end + 1);
        end = s.find(' ', start);
    }
    number += s.substr(start, s.size() - start);
    return std::stoll(number);
}

int main(int argc, char *argv[])
{
    std::string line;
    std::fstream file("input.txt");

    if (!file.is_open())
    {
        std::cerr << "Error opening input file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::getline(file, line);
    const auto raceTime = extractNumber(line, 5);

    std::getline(file, line);
    const auto currentRecord = extractNumber(line, 9);

    const double discriminantSqrt = std::sqrt(raceTime * raceTime - 4 * (1) * (currentRecord));
    long long possibleRoot1 = std::floor(raceTime + discriminantSqrt) / 2;
    long long possibleRoot2 = std::ceil(raceTime - discriminantSqrt) / 2;

    if (possibleRoot1 * (raceTime - possibleRoot1) <= currentRecord)
    {
        possibleRoot1--;
    }
    if (possibleRoot2 * (raceTime - possibleRoot2) <= currentRecord)
    {
        possibleRoot2++;
    }

    std::cout << (possibleRoot1 - possibleRoot2 + 1) << '\n';
    return EXIT_SUCCESS;
}

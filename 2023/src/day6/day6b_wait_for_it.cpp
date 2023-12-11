#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

/// @brief Extracts a single number from a string, considering a given delta
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
    // Open input file
    std::string line;
    std::fstream file("input.txt");
    if (!file.is_open())
    {
        std::cerr << "Error opening input file!" << std::endl;
        return EXIT_FAILURE;
    }

    // Read race time from the first line
    std::getline(file, line);
    const auto raceTime = extractNumber(line, 5);

    // Read current record from the second line
    std::getline(file, line);
    const auto currentRecord = extractNumber(line, 9);

    // Calculate the range of possible roots
    const double discriminantSqrt = std::sqrt(raceTime * raceTime - 4 * (1) * (currentRecord));
    long long possibleRoot1 = std::floor(raceTime + discriminantSqrt) / 2;
    long long possibleRoot2 = std::ceil(raceTime - discriminantSqrt) / 2;

    // Adjust possible roots based on conditions
    if (possibleRoot1 * (raceTime - possibleRoot1) <= currentRecord)
    {
        possibleRoot1--;
    }
    if (possibleRoot2 * (raceTime - possibleRoot2) <= currentRecord)
    {
        possibleRoot2++;
    }

    // Output the count of possible roots
    std::cout << (possibleRoot1 - possibleRoot2 + 1) << '\n';
    return EXIT_SUCCESS;
}

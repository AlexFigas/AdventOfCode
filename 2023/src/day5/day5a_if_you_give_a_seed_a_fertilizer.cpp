#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

/// @brief Represents a mapping from a source range to a destination range
struct Mapping
{
    long long destinationStart; ///< Start of the destination range
    long long sourceStart;      ///< Start of the source range
    long long range;            ///< Range of the mapping

    /// @brief Check if a value is within the destination range
    bool inDestinationRange(const long long n) const
    {
        return n >= destinationStart && n <= destinationStart + range;
    }

    /// @brief Check if a value is within the source range
    bool inSourceRange(const long long n) const
    {
        return n >= sourceStart && n <= sourceStart + range;
    }

    /// @brief Get the mapped value for a given source value
    long long getMapping(const long long n) const
    {
        return destinationStart + (n - sourceStart);
    }
};

/// @brief Represents a mapping from one map to another
struct Map
{
    std::string mapFrom;         ///< Name of the source map
    std::string mapTo;           ///< Name of the destination map
    std::vector<Mapping> mappings; ///< List of mappings
};

/// @brief Extracts numbers from a string and returns them as a vector of long long
std::vector<long long> extract(const std::string &s)
{
    std::vector<long long> numbers;
    long long start = 0;
    long long end = s.find(' ');
    while (end != std::string::npos)
    {
        numbers.push_back(std::stoll(s.substr(start, end - start)));
        start = end + 1;
        end = s.find(' ', start);
    }
    numbers.push_back(std::stoll(s.substr(start, s.size() - start)));
    return numbers;
}

/// @brief Recursively gets the final location from the maps
long long getLocation(long long value, const std::vector<Map> &maps, const std::string &mapFrom)
{
    if (mapFrom == "location")
    {
        return value;
    }

    const auto map = *std::find_if(
        std::begin(maps),
        std::end(maps),
        [&mapFrom](const auto &map)
        {
            return map.mapFrom == mapFrom;
        });

    for (const auto &mapping : map.mappings)
    {
        if (mapping.inSourceRange(value))
        {
            return getLocation(mapping.getMapping(value), maps, map.mapTo);
        }
    }

    return getLocation(value, maps, map.mapTo);
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

    std::vector<Map> maps;        ///< List of maps
    std::getline(file, line);
    std::vector<long long> seeds = extract(line.substr(7, line.size() - 7));

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            std::getline(file, line);
            Map map;
            long long start = 0;
            long long end = line.find('-');
            map.mapFrom = line.substr(start, end - start);
            start = end + 4;
            end = line.find(' ', start);
            map.mapTo = line.substr(start, end - start);
            maps.push_back(map);
            std::getline(file, line);
        }

        const auto numbers = extract(line);
        Mapping mapping;
        mapping.destinationStart = numbers[0];
        mapping.sourceStart = numbers[1];
        mapping.range = numbers[2];
        maps.back().mappings.push_back(mapping);
    }

    std::vector<long long> locations;
    for (const auto &seed : seeds)
    {
        locations.push_back(getLocation(seed, maps, "seed"));
    }

    std::cout << *std::min_element(std::begin(locations), std::end(locations)) << '\n';
    return 0;
}

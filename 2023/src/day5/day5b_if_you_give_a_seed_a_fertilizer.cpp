#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
#include <array>

/// @brief Represents a mapping from one range to another
struct Mapping
{
    std::array<long long, 2> from; ///< Source range [from[0], from[1]]
    std::array<long long, 2> to;   ///< Destination range [to[0], to[1]]

    /// @brief Apply the mapping to a value
    long long applyMap(const long long n) const
    {
        return to[0] + (n - from[0]);
    }
};

/// @brief Represents a map with a name, mappings, and a destination map name
struct Map
{
    std::string mapFrom;           ///< Source map name
    std::string mapTo;             ///< Destination map name
    std::vector<Mapping> mappings; ///< List of mappings
};

/// @brief Extracts numbers from a string and returns them as a vector of long long
std::vector<long long> extract(const std::string &s)
{
    std::vector<long long> numbers;
    std::size_t start = 0;
    std::size_t end = s.find(' ');
    while (end != std::string::npos)
    {
        numbers.push_back(std::stoll(s.substr(start, end - start)));
        start = end + 1;
        end = s.find(' ', start);
    }
    numbers.push_back(std::stoll(s.substr(start, s.size() - start)));
    return numbers;
}

/// @brief Applies a filter to relevant ranges using mappings
std::vector<std::pair<long long, long long>> applyFilter(std::vector<std::pair<long long, long long>> relevantRanges, const std::vector<Mapping> &mappings)
{
    std::vector<std::pair<long long, long long>> newRelevantValues;

    for (const auto &relevantRange : relevantRanges)
    {
        std::vector<std::pair<long long, long long>> rangesWhereFilterApplied;
        std::vector<std::pair<long long, long long>> valuesWhenFilterApplied;

        for (const auto &mapping : mappings)
        {
            // Check if the relevant range intersects with the mapping source range
            if (relevantRange.second < mapping.from[0] || relevantRange.first > mapping.from[1])
                continue;

            // Apply different cases based on the intersection of ranges
            if (relevantRange.first >= mapping.from[0] && relevantRange.second <= mapping.from[1])
            {
                rangesWhereFilterApplied.emplace_back(relevantRange.first, relevantRange.second);
            }
            else if (relevantRange.first >= mapping.from[0] && relevantRange.second >= mapping.from[1])
            {
                rangesWhereFilterApplied.emplace_back(relevantRange.first, mapping.from[1]);
            }
            else if (relevantRange.first <= mapping.from[0] && relevantRange.second <= mapping.from[1])
            {
                rangesWhereFilterApplied.emplace_back(mapping.from[0], relevantRange.second);
            }
            else if (relevantRange.first <= mapping.from[0] && relevantRange.second >= mapping.from[1])
            {
                rangesWhereFilterApplied.emplace_back(mapping.from[0], mapping.from[1]);
            }
            else
            {
                std::cout << "?!" << '\n';
                exit(0);
            }

            valuesWhenFilterApplied.emplace_back(mapping.applyMap(rangesWhereFilterApplied.back().first), mapping.applyMap(rangesWhereFilterApplied.back().second));
        }

        auto currentStart = relevantRange.first;

        for (int i = 0; i < rangesWhereFilterApplied.size(); i++)
        {
            newRelevantValues.emplace_back(currentStart, rangesWhereFilterApplied[i].first - 1);
            newRelevantValues.emplace_back(valuesWhenFilterApplied[i].first, valuesWhenFilterApplied[i].second);
            currentStart = rangesWhereFilterApplied[i].second + 1;
        }

        if (currentStart <= relevantRange.second)
        {
            newRelevantValues.emplace_back(currentStart, relevantRange.second);
        }
    }

    // Filter out invalid ranges
    std::vector<std::pair<long long, long long>> filteredNewRelevantValues;
    std::copy_if(std::begin(newRelevantValues), std::end(newRelevantValues), std::back_inserter(filteredNewRelevantValues), [](const auto &ele)
                 { return ele.first <= ele.second; });

    // Sort the filtered ranges
    std::sort(std::begin(filteredNewRelevantValues), std::end(filteredNewRelevantValues), [](const auto &v1, const auto &v2)
              { return v1.first < v2.first; });

    return filteredNewRelevantValues;
}

int main(int argc, char *argv[])
{
    std::string line;
    std::ifstream file("input.txt");
    std::vector<Map> maps;

    std::getline(file, line);
    std::vector<long long> inputRanges = extract(line.substr(7, line.size() - 7));
    std::vector<std::pair<long long, long long>> seeds;

    // Convert input ranges into pairs
    for (int i = 0; i < inputRanges.size(); i += 2)
    {
        seeds.emplace_back(inputRanges[i], inputRanges[i] + inputRanges[i + 1] - 1);
    }

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
        mapping.to = {{numbers[0], numbers[0] + numbers[2] - 1}};
        mapping.from = {{numbers[1], numbers[1] + numbers[2] - 1}};
        maps.back().mappings.push_back(mapping);
    }

    // Sort mappings within each map
    for (auto &map : maps)
    {
        std::sort(std::begin(map.mappings), std::end(map.mappings), [](const auto &m1, const auto &m2)
                  { return m1.from[0] < m2.from[0]; });
    }

    // Check if consecutive maps are connected
    for (int i = 0; i < maps.size() - 1; i++)
    {
        assert(maps[i + 1].mapFrom == maps[i].mapTo);
    }

    std::vector<std::pair<long long, long long>> relevantValues = seeds;

    // Apply each map's filter to relevant values
    for (const auto &map : maps)
    {
        relevantValues = applyFilter(relevantValues, map.mappings);
    }

    // Output the minimum value from the relevant values
    std::cout << std::min_element(std::begin(relevantValues), std::end(relevantValues))->first << '\n';

    return 0;
}

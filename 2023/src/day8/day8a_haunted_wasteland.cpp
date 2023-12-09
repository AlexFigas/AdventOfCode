#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <array>
#include <regex>

std::string getNextState(const std::unordered_map<std::string, std::array<std::string, 2>> &map, const std::string &current, char instruction)
{
    return (instruction == 'L') ? map.at(current)[0] : map.at(current)[1];
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

    std::string instructions;
    std::getline(file, instructions);

    std::unordered_map<std::string, std::array<std::string, 2>> stateMap;
    const std::regex maskPattern(R"(([A-Z]+) = \(([A-Z]+), ([A-Z]+)\))");
    std::smatch maskMatch;

    while (std::getline(file, line))
    {
        std::regex_search(line, maskMatch, maskPattern);
        stateMap[maskMatch[1]] = {{maskMatch[2], maskMatch[3]}};
    }

    std::string currentState = "AAA";
    int instructionIndex = 0;
    std::size_t numSteps = 0;

    while (currentState != "ZZZ")
    {
        char currentInstruction = instructions[instructionIndex];
        currentState = getNextState(stateMap, currentState, currentInstruction);
        instructionIndex = (instructionIndex + 1) % instructions.size();
        numSteps++;
    }

    std::cout << numSteps << '\n';

    return 0;
}

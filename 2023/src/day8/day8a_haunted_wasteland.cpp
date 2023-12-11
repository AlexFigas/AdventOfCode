#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <array>
#include <regex>

/// @brief Gets the next state based on the current state and instruction
std::string getNextState(const std::unordered_map<std::string, std::array<std::string, 2>> &map,
                         const std::string &current, char instruction)
{
    // If the instruction is 'L', return the first state from the map, otherwise return the second state
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

    // Read the first line of the file, which contains instructions
    std::string instructions;
    std::getline(file, instructions);

    // Initialize a map to store pairs of states for each instruction
    std::unordered_map<std::string, std::array<std::string, 2>> stateMap;

    // Define a regex pattern to match lines containing state transitions
    const std::regex maskPattern(R"(([A-Z]+) = \(([A-Z]+), ([A-Z]+)\))");
    std::smatch maskMatch;

    // Read the rest of the file to populate the stateMap with state transitions
    while (std::getline(file, line))
    {
        // Use regex to extract information from the line and update the stateMap
        std::regex_search(line, maskMatch, maskPattern);
        stateMap[maskMatch[1]] = {{maskMatch[2], maskMatch[3]}};
    }

    // Initialize variables to track the current state, instruction index, and number of steps
    std::string currentState = "AAA";
    int instructionIndex = 0;
    std::size_t numSteps = 0;

    // Continue processing instructions until the current state becomes "ZZZ"
    while (currentState != "ZZZ")
    {
        // Get the current instruction and update the current state
        char currentInstruction = instructions[instructionIndex];
        currentState = getNextState(stateMap, currentState, currentInstruction);

        // Increment the instruction index and update the number of steps
        instructionIndex = (instructionIndex + 1) % instructions.size();
        numSteps++;
    }

    // Print the total number of steps taken
    std::cout << numSteps << '\n';

    return 0;
}

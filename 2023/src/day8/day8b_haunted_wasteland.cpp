#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_set>
#include <cassert>

// Constants
constexpr char NODE_END_A = 'A';
constexpr char NODE_END_Z = 'Z';
constexpr char LEFT_TURN = 'L';
constexpr char RIGHT_TURN = 'R';

// Define a struct to represent a state with node, index, and number of steps
struct State
{
    std::string node;
    int idx;
    int n_steps;

    // Define equality operator for State struct
    bool operator==(const State &s) const
    {
        return node == s.node && idx == s.idx;
    }
};

// Define a hash function for the State struct
struct Hasher
{
    long long operator()(const State &s) const
    {
        return s.idx;
    }
};

// Function to perform integer division and modulus
std::pair<long long, long long> divmod(const long long a, const long long b)
{
    return {a / b, a % b};
}

// Function to calculate extended GCD (Greatest Common Divisor)
std::pair<long long, long long> extended_gcd(const long long a, const long long b)
{
    // Extended Euclidean Algorithm
    // Returns GCD, coefficients s and t for the equation sa + tb = GCD(a, b)
    // where GCD is the greatest common divisor of a and b
    long long old_r = a;
    long long r = b;
    long long old_s = 1;
    long long s = 0;
    long long old_t = 0;
    long long t = 1;
    long long temp;
    while (r != 0)
    {
        const auto [quotient, remainder] = divmod(old_r, r);
        old_r = r;
        r = remainder;
        temp = old_s;
        old_s = s;
        s = temp - quotient * s;
        temp = old_t;
        old_t = t;
        t = temp - quotient * t;
    }
    return {old_r, old_s};
}

// Function to combine phased rotations
std::pair<long long, long long> combine_phased_rotations(const long long a_period, const long long a_phase, const long long b_period, const long long b_phase)
{
    // Combine two rotations with given periods and phases
    // Returns the combined period and phase
    const auto [gcd, s] = extended_gcd(a_period, b_period);
    const auto phase_difference = a_phase - b_phase;
    const auto [pd_mult, pd_remainder] = divmod(phase_difference, gcd);

    // Check if rotation reference points synchronize
    if (pd_remainder != 0)
    {
        std::cout << "Rotation reference points never synchronize." << '\n';
        std::exit(EXIT_FAILURE);
    }

    const long long combined_period = (a_period / gcd) * b_period;
    const long long combined_phase = (a_phase - s * pd_mult * a_period) % combined_period;

    return {combined_period, combined_phase};
}

// Main function
int main(int argc, char *argv[])
{
    // Open and read input file
    std::string line;
    std::ifstream file("input.txt");
    if (!file.is_open())
    {
        std::cerr << "Error opening input file!" << std::endl;
        return EXIT_FAILURE;
    }

    // Read instructions from the file
    std::string instructions;
    std::getline(file, instructions);

    // Define a map to store node transitions
    std::unordered_map<std::string, std::array<std::string, 2>> map;

    // Define a regex pattern for parsing lines in the file
    const std::regex maskPattern(R"(([0-9A-Z]+) = \(([0-9A-Z]+), ([0-9A-Z]+)\))");
    std::smatch maskMatch;
    std::vector<std::string> nodes;

    // Parse the file and populate the map
    while (std::getline(file, line))
    {
        std::regex_search(line, maskMatch, maskPattern);
        map[maskMatch[1]] = {{maskMatch[2], maskMatch[3]}};
    }

    // Filter nodes ending in 'A'
    for (const auto &[node, options] : map)
    {
        if (node.back() == NODE_END_A)
        {
            nodes.push_back(node);
        }
    }

    // Initialize data structures
    std::unordered_set<State, Hasher> seenMapPrime;
    std::vector<long long> offset;
    std::vector<long long> loopSize;
    std::vector<std::pair<std::string, long long>> nodesEndingInZ;

    // Iterate over nodes and compute offsets and loop sizes
    for (const auto &node : nodes)
    {
        long long countZ = 0;
        auto seenMap = seenMapPrime;
        State current{node, 0, 0};
        int idx = 0;
        long long n_steps = 0;

        // Simulate the state transitions until a loop is detected
        while (true)
        {
            if (current.node.back() == NODE_END_Z)
            {
                nodesEndingInZ.emplace_back(current.node, current.n_steps);
                countZ++;
            }

            if (seenMap.find(current) != seenMap.end())
            {
                const auto &original = *seenMap.find(current);
                offset.push_back(original.n_steps);
                loopSize.push_back(n_steps - original.n_steps);
                break;
            }

            seenMap.insert(current);  // Insert the current state directly
            const int turn = (instructions[idx] == LEFT_TURN ? 0 : 1);
            idx = (idx + 1) % instructions.size();
            n_steps++;
            current.node = map[current.node][turn];
            current.n_steps = n_steps;
            current.idx = idx;
        }
    }

    // Combine phased rotations for nodes ending in 'Z'
    long long currentPeriod = loopSize[0];
    long long currentPhase = nodesEndingInZ[0].second;

    for (int i = 1; i < nodesEndingInZ.size(); i++)
    {
        const auto [newPeriod, newPhase] = combine_phased_rotations(currentPeriod, currentPhase, loopSize[i], nodesEndingInZ[i].second);
        currentPeriod = newPeriod;
        currentPhase = newPhase;
    }

    // Output the result
    std::cout << currentPeriod << '\n';

    return 0;
}

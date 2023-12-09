#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_set>
#include <cassert>

struct State
{
    std::string node;
    int idx;
    int n_steps;

    bool operator==(const State &s) const
    {
        return node == s.node && idx == s.idx;
    }
};

struct Hasher
{
    long long operator()(const State &s) const
    {
        return s.idx;
    }
};

std::pair<long long, long long> divmod(const long long a, const long long b)
{
    return {a / b, a % b};
}

std::pair<long long, long long> extended_gcd(const long long a, const long long b)
{
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

std::pair<long long, long long> combine_phased_rotations(const long long a_period, const long long a_phase, const long long b_period, const long long b_phase)
{
    const auto [gcd, s] = extended_gcd(a_period, b_period);
    const auto phase_difference = a_phase - b_phase;
    const auto [pd_mult, pd_remainder] = divmod(phase_difference, gcd);
    if (pd_remainder != 0)
    {
        std::cout << "Rotation reference points never synchronize." << '\n';
        exit(0);
    }
    const long long combined_period = (a_period / gcd) * b_period;
    const long long combined_phase = (a_phase - s * pd_mult * a_period) % combined_period;
    return {combined_period, combined_phase};
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

    std::unordered_map<std::string, std::array<std::string, 2>> map;
    const std::regex maskPattern(R"(([0-9A-Z]+) = \(([0-9A-Z]+), ([0-9A-Z]+)\))");
    std::smatch maskMatch;
    std::vector<std::string> nodes;

    while (std::getline(file, line))
    {
        std::regex_search(line, maskMatch, maskPattern);
        map[maskMatch[1]] = {{maskMatch[2], maskMatch[3]}};
    }

    for (const auto &[node, options] : map)
    {
        if (node[node.size() - 1] == 'A')
        {
            nodes.push_back(node);
        }
    }

    std::unordered_set<State, Hasher> seenMapPrime;
    std::vector<long long> offset;
    std::vector<long long> loopSize;
    std::vector<std::pair<std::string, long long>> nodesEndingInZ;

    for (const auto &node : nodes)
    {
        long long countZ = 0;
        auto seenMap = seenMapPrime;
        State current;
        current.node = node;
        current.idx = 0;
        current.n_steps = 0;
        int idx = 0;
        long long n_steps = 0;

        while (true)
        {
            if (current.node[current.node.size() - 1] == 'Z')
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
            seenMap.insert(current);
            const int turn = (instructions[idx] == 'L' ? 0 : 1);
            idx = (idx + 1) % instructions.size();
            n_steps++;
            current.node = map[current.node][turn];
            current.n_steps = n_steps;
            current.idx = idx;
        }
    }

    long long currentPeriod = loopSize[0];
    long long currentPhase = nodesEndingInZ[0].second;

    for (int i = 1; i < nodesEndingInZ.size(); i++)
    {
        const auto [newPeriod, newPhase] = combine_phased_rotations(currentPeriod, currentPhase, loopSize[i], nodesEndingInZ[i].second);
        currentPeriod = newPeriod;
        currentPhase = newPhase;
    }

    std::cout << currentPeriod << '\n';

    return 0;
}

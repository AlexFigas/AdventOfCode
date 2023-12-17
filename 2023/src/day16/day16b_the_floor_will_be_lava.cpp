#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

// Structure representing a point with row and column coordinates
struct Point
{
    int row;
    int col;

    Point(const int row, const int col) : row(row), col(col) {}

    // Operator overloading to support addition of points
    Point operator+(const Point &p) const
    {
        return Point(p.row + row, p.col + col);
    }

    // Operator overloading to check equality of points
    bool operator==(const Point &p) const
    {
        return p.row == row && p.col == col;
    }
};

// Structure representing a beam with a head point and a direction
struct Beam
{
    Point head;
    int direction;

    Beam(const Point &head, const int direction) : head(head), direction(direction) {}

    // Operator overloading to check equality of beams
    bool operator==(const Beam &b) const
    {
        return head == b.head && direction == b.direction;
    }
};

// Hasher class for custom hashing of Point and Beam in unordered_set
struct hasher
{
    std::size_t operator()(const Point &p) const
    {
        return std::max(p.row + p.col, 0);
    }

    std::size_t operator()(const Beam &b) const
    {
        return std::max(b.head.row + b.head.col, 0);
    }
};

// Possible motions (up, right, down, left)
const std::vector<Point> motions = {
    Point(-1, 0),
    Point(0, 1),
    Point(1, 0),
    Point(0, -1),
};

// Function to check if a given row and column are within the limits of the map
bool in_limits(const std::vector<std::string> &map, const int row, const int col)
{
    return row >= 0 && row < map.size() && col >= 0 && col < map[0].size();
}

// Function to simulate firing a beam from a given starting point and direction
std::size_t fire_beam(const std::vector<std::string> &map, const int start_row, const int start_col, const int direction)
{
    // Counter for the number of beams that hit each point
    std::vector<std::vector<int>> counter(map.size(), std::vector<int>(map[0].size(), 0));

    // Queue for beams to explore
    std::queue<Beam> beams;
    beams.emplace(Point(start_row, start_col), direction);

    // Set to keep track of seen beams to avoid processing duplicates
    std::unordered_set<Beam, hasher> seen;

    while (!beams.empty())
    {
        const auto beam = beams.front();
        auto current = beam.head;
        beams.pop();

        // Skip if the beam has already been seen
        if (seen.find(beam) != seen.end())
        {
            continue;
        }
        seen.insert(beam);

        // Increment counter for the current point if within map limits
        if (in_limits(map, current.row, current.col))
            counter[current.row][current.col]++;

        // Move along the beam's direction until an obstacle is encountered
        current = current + motions[beam.direction];
        while (in_limits(map, current.row, current.col) &&
               (map[current.row][current.col] == '.' ||
                ((map[current.row][current.col] == '-') && (beam.direction == 1 || beam.direction == 3)) ||
                ((map[current.row][current.col] == '|') && (beam.direction == 0 || beam.direction == 2))))
        {
            counter[current.row][current.col]++;
            current = current + motions[beam.direction];
        }

        // Skip if the current point is out of map limits
        if (!in_limits(map, current.row, current.col))
        {
            continue;
        }

        // Increment counter for the final point along the beam's direction
        counter[current.row][current.col]++;

        // Add new beams based on the type of obstacle encountered
        if (map[current.row][current.col] == '|' && (beam.direction == 1 || beam.direction == 3))
        {
            beams.push(Beam(current, 0));
            beams.push(Beam(current, 2));
        }
        else if (map[current.row][current.col] == '-' && (beam.direction == 0 || beam.direction == 2))
        {
            beams.push(Beam(current, 1));
            beams.push(Beam(current, 3));
        }
        else if (map[current.row][current.col] == '\\')
        {
            if (beam.direction == 1)
                beams.push(Beam(current, 2));
            else if (beam.direction == 2)
                beams.push(Beam(current, 1));
            else if (beam.direction == 0)
                beams.push(Beam(current, 3));
            else if (beam.direction == 3)
                beams.push(Beam(current, 0));
            else
            {
                std::cout << __LINE__ << " This should not happen" << '\n';
                exit(0);
            }
        }
        else if (map[current.row][current.col] == '/')
        {
            if (beam.direction == 1)
                beams.push(Beam(current, 0));
            else if (beam.direction == 0)
                beams.push(Beam(current, 1));
            else if (beam.direction == 2)
                beams.push(Beam(current, 3));
            else if (beam.direction == 3)
                beams.push(Beam(current, 2));
            else
            {
                std::cout << __LINE__ << " This should not happen" << '\n';
                exit(0);
            }
        }
    }

    // Calculate the total number of points hit by beams
    std::size_t total = 0;
    for (const auto &row : counter)
    {
        for (const auto ele : row)
        {
            total += (ele != 0);
        }
    }
    return total;
}

int main(int argc, char *argv[])
{
    // Default input file
    std::string input = "input.txt";
    if (argc > 1)
    {
        input = argv[1];
    }

    std::string line;
    std::fstream file(input);
    std::vector<std::string> map;

    // Read lines from the file to populate the map
    while (std::getline(file, line))
    {
        map.push_back(line);
    }

    std::size_t total = 0;

    // Fire beams from the top and bottom edges
    for (int start_row = 0; start_row < map.size(); start_row++)
    {
        total = std::max(total, fire_beam(map, start_row, -1, 1));
        total = std::max(total, fire_beam(map, start_row, map[0].size(), 3));
    }

    // Fire beams from the left and right edges
    for (int start_col = 0; start_col < map[0].size(); start_col++)
    {
        total = std::max(total, fire_beam(map, -1, start_col, 2));
        total = std::max(total, fire_beam(map, map.size(), start_col, 0));
    }

    // Print the total for Part 2
    std::cout << total << '\n';

    return 0;
}

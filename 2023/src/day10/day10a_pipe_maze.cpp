#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <vector>
#include <array>

// Structure to represent a point with row and column
struct Point
{
    int row;
    int col;

    // Comparison operators for points
    bool operator==(const Point &p) const
    {
        return p.row == row && p.col == col;
    }

    bool operator!=(const Point &p) const
    {
        return !(p == *this);
    }

    Point(const int row = 0, const int col = 0) : row(row), col(col) {}
};

// Function to check if a point is within the limits of the map
bool in_limits(const Point &p, const std::vector<std::string> &map)
{
    return p.row >= 0 && p.row < map.size() && p.col >= 0 && p.col < map[p.row].size();
}

// Array of deltas representing possible movements in four directions
const std::array<Point, 4> deltas = {{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}};

// Function to get the next point based on the current point, previous point, map, and direction map
Point get_next_point(const Point &point, const Point &previous_point, const std::vector<std::string> &map, const std::vector<std::vector<std::array<int, 4>>> &direction_map)
{
    std::vector<Point> next_points;
    const auto &point_directions = direction_map[point.row][point.col];

    for (int i = 0; i < 4; i++)
    {
        if (point_directions[i])
        {
            const Point next_point(point.row + deltas[i].row, point.col + deltas[i].col);

            // Check if the next point is within limits and has a valid reverse direction
            if (in_limits(next_point, map) && direction_map[next_point.row][next_point.col][(i + 2) % 4] && next_point != previous_point)
            {
                return next_point;
            }
        }
    }
    throw std::runtime_error("Unexpected condition in get_next_point");
}

// Structure to represent a Point and a step count
struct PointStep
{
    Point point;
    int step;

    // Comparison operator for PointStep
    bool operator==(const PointStep ps) const
    {
        return ps.point == point;
    }
};

int main(int argc, char *argv[])
{
    // Read input filename from command line arguments or use default
    std::string input = "input.txt";
    if (argc > 1)
    {
        input = argv[1];
    }

    // Open the input file
    std::fstream file(input);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << input << '\n';
        return 1;
    }

    std::string line;
    std::vector<std::string> map;
    std::vector<std::vector<std::array<int, 4>>> direction_map;
    Point S;
    Point previous = {-1, -1};

    // Read the map and create the direction_map based on the characters
    while (std::getline(file, line))
    {
        map.push_back(line);
        direction_map.push_back({});
        direction_map.back().reserve(line.size() * 4);

        for (const auto c : line)
        {
            switch (c)
            {
            case '|':
                direction_map.back().push_back({{true, false, true, false}});
                break;
            case '-':
                direction_map.back().push_back({{false, true, false, true}});
                break;
            case 'L':
                direction_map.back().push_back({{true, true, false, false}});
                break;
            case 'J':
                direction_map.back().push_back({{true, false, false, true}});
                break;
            case '7':
                direction_map.back().push_back({{false, false, true, true}});
                break;
            case 'F':
                direction_map.back().push_back({{false, true, true, false}});
                break;
            case '.':
                direction_map.back().push_back({{false, false, false, false}});
                break;
            case 'S':
                direction_map.back().push_back({{true, true, true, true}});
                S.row = direction_map.size() - 1;
                for (int i = 0; i < line.size(); i++)
                {
                    if (line[i] == 'S')
                    {
                        S.col = i;
                        break;
                    }
                }
                break;
            default:
                direction_map.back().push_back({{false, false, false, false}});
            }
        }
    }

    PointStep current_ps;
    current_ps.point = S;
    current_ps.step = 0;
    std::vector<PointStep> visited;

    // Traverse the map until we reach the starting point again
    while (current_ps.point != S || current_ps.step == 0)
    {
        visited.push_back(current_ps);
        const auto new_current_point = get_next_point(current_ps.point, previous, map, direction_map);
        previous = current_ps.point;
        current_ps.point = new_current_point;
        current_ps.step++;
    }

    // Print the step count at the middle point
    std::cout << visited[visited.size() / 2].step << '\n';

    return 0;
}

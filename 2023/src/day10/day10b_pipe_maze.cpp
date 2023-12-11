#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <array>

/// @todo Did not finish the puzzle correctly

// Constants defining the number of directions and an invalid group ID.
constexpr int NUM_DIRECTIONS = 4;
constexpr int INVALID_GROUP_ID = -1;

// Structure representing a point with row and column coordinates.
struct Point {
    int row;
    int col;

    // Equality operators for Point.
    bool operator==(const Point& p) const {
        return p.row == row && p.col == col;
    }

    bool operator!=(const Point& p) const {
        return !(p == *this);
    }

    // Constructor with default values.
    Point(const int row = 0, const int col = 0) : row(row), col(col) {}
};

// Hasher for the Point struct to be used in unordered_set.
struct Hasher {
    std::size_t operator()(const Point& p) const {
        return p.row * 10000 + p.col;
    }
};

// Array of Point objects representing movement deltas in different directions.
const std::array<Point, NUM_DIRECTIONS> deltas{
    Point(-1, 0),
    Point(0, 1),
    Point(1, 0),
    Point(0, -1)
};

// Function to check if a point is within the limits of the map.
bool inLimits(const Point& p, const std::vector<std::string>& map) {
    return p.row >= 0 && p.row < map.size() && p.col >= 0 && p.col < map[p.row].size();
}

// Breadth-first search function to explore connected components on the map.
int bfs(const Point& p, std::vector<std::string>& map, std::vector<std::vector<int>>& groups,
        const std::vector<std::vector<bool>>& isPartOfLoop, const int groupId) {
    std::queue<Point> q;
    q.push(p);
    int count = 0;
    std::unordered_set<Point, Hasher> seen;
    while (!q.empty()) {
        const auto current = q.front();
        q.pop();
        if (isPartOfLoop[current.row][current.col]) continue;
        if (seen.find(current) != seen.end()) continue;
        seen.insert(current);
        count++;
        groups[current.row][current.col] = groupId;
        for (const auto& delta : deltas) {
            Point newPoint{
                current.row + delta.row,
                current.col + delta.col
            };
            if (inLimits(newPoint, map) && groups[newPoint.row][newPoint.col] == INVALID_GROUP_ID) {
                q.push(newPoint);
            }
        }
    }
    return count;
}

// Function to get the next point in the path based on current and previous points.
std::pair<Point, int> getNextPoint(const Point& point, const Point& previousPoint,
                                  const std::vector<std::string>& map,
                                  const std::vector<std::vector<std::array<int, NUM_DIRECTIONS>>>& directionMap) {
    const auto& pointDirections = directionMap[point.row][point.col];
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        if (pointDirections[i]) {
            const Point nextPoint{point.row + deltas[i].row, point.col + deltas[i].col};
            if (inLimits(nextPoint, map) &&
                directionMap[nextPoint.row][nextPoint.col][(i + 2) % NUM_DIRECTIONS] &&
                nextPoint != previousPoint) {
                return {nextPoint, i};
            }
        }
    }
    // In case this point is reached, it indicates an unexpected scenario.
    Point invalidPoint(-1, -1);
    std::cout << "This should not happen" << '\n';
    exit(1);
    return {invalidPoint, 0};
}

// Structure representing a path step with direction information.
struct PathStepDirection {
    Point point;
    int step; // No longer necessary
    int direction; // For debug only

    // Equality operator for PathStepDirection.
    bool operator==(const PathStepDirection& psd) const {
        return psd.point == point;
    }
};

// Main function for the program.
int main(int argc, char* argv[]) {
    // Default input filename.
    std::string input = "input.txt";
    // Check if a different input filename is provided as a command line argument.
    if (argc > 1) {
        input = argv[1];
    }

    // N, E, S, W
    std::string line;
    std::fstream file(input);
    std::vector<std::string> map;
    std::vector<std::vector<std::array<int, NUM_DIRECTIONS>>> directionMap; // Travel allowed in these directions (Order is NESW)
    Point startingPoint;
    Point previousPoint(-1, -1);

    // Read the map and direction information from the file.
    while (std::getline(file, line)) {
        map.push_back(line);
        directionMap.push_back({});
        directionMap.back().reserve(line.size() * NUM_DIRECTIONS);
        for (const char c : line) {
            // Populate the direction map based on characters in the input line.
            if (c == '|') {
                directionMap.back().push_back({{true, false, true, false}});
            } else if (c == '-') {
                directionMap.back().push_back({{false, true, false, true}});
            } else if (c == 'L') {
                directionMap.back().push_back({{true, true, false, false}});
            } else if (c == 'J') {
                directionMap.back().push_back({{true, false, false, true}});
            } else if (c == '7') {
                directionMap.back().push_back({{false, false, true, true}});
            } else if (c == 'F') {
                directionMap.back().push_back({{false, true, true, false}});
            } else if (c == '.') {
                directionMap.back().push_back({{false, false, false, false}});
            } else if (c == 'S') {
                directionMap.back().push_back({{true, true, true, true}});
                startingPoint.row = directionMap.size() - 1;
                for (int i = 0; i < line.size(); i++) {
                    if (line[i] == 'S') {
                        startingPoint.col = i;
                        break;
                    }
                }
            }
        }
    }

    // Initialize variables for path traversal.
    PathStepDirection currentPSD;
    currentPSD.point = startingPoint;
    currentPSD.direction = 1; // Update direction of start, though not used
    currentPSD.step = 0;
    std::vector<PathStepDirection> loop;
    std::vector<std::vector<bool>> isPartOfLoop(map.size(), std::vector<bool>(map[0].size(), false));

    // Traverse the loop until the starting point is reached again.
    while (currentPSD.point != startingPoint || currentPSD.step == 0) {
        isPartOfLoop[currentPSD.point.row][currentPSD.point.col] = true;
        const auto [newCurrentPoint, direction] = getNextPoint(currentPSD.point, previousPoint, map, directionMap);
        currentPSD.direction = direction;
        loop.push_back(currentPSD);
        previousPoint = currentPSD.point;
        currentPSD.point = newCurrentPoint;
        currentPSD.step++;
    }

    // Vector representing the point to the right of each direction.
    const std::vector<Point> pointToTheRightOfDirection{
        Point(0, 1),  // N->E
        Point(1, 0),  // E->S
        Point(0, -1), // S->W
        Point(-1, 0)  // W->N
    };

    // Vector to store group IDs for each point on the map.
    std::vector<std::vector<int>> groups(map.size(), std::vector<int>(map[0].size(), INVALID_GROUP_ID));
    int groupId = 1;
    std::unordered_map<int, int> counts;

    // Identify connected components (groups) on the map using BFS.
    for (int row = 0; row < groups.size(); row++) {
        for (int col = 0; col < groups[0].size(); col++) {
            if (groups[row][col] == INVALID_GROUP_ID) {
                const auto p = Point(row, col);
                counts[groupId] = bfs(p, map, groups, isPartOfLoop, groupId);
                groupId++;
            }
        }
    }

    // Set to store group IDs to the right of the loop.
    std::unordered_set<int> groupsToTheRightOfLoop;

    // Identify groups to the right of the loop.
    for (int idx = 1; idx < loop.size(); idx++) {
        const auto& p = loop[idx];
        const auto deltas = std::vector<Point>{Point(loop[idx + 1].point.row - p.point.row, loop[idx + 1].point.col - p.point.col),
                                              Point(p.point.row - loop[idx - 1].point.row, p.point.col - loop[idx - 1].point.col)};

        for (const auto& delta : deltas) {
            int direction = -1;

            // Determine the direction based on the delta.
            if (delta == Point(-1, 0)) direction = 0;
            else if (delta == Point(0, 1)) direction = 1;
            else if (delta == Point(1, 0)) direction = 2;
            else if (delta == Point(0, -1)) direction = 3;

            const auto deltaToGetRightPoint = pointToTheRightOfDirection[direction];
            const auto pointToRight = Point(p.point.row + deltaToGetRightPoint.row, p.point.col + deltaToGetRightPoint.col);

            if (!inLimits(pointToRight, map)) continue;

            if (groups[pointToRight.row][pointToRight.col] != INVALID_GROUP_ID) {
                groupsToTheRightOfLoop.insert(groups[pointToRight.row][pointToRight.col]);
            }
        }
    }

    int totalRight = 0;
    for (const auto groupToTheRightOfLoop : groupsToTheRightOfLoop) {
        totalRight += counts[groupToTheRightOfLoop];
    }

    int totalLeft = 0;
    for (const auto& [groupId, count] : counts) {
        if (groupsToTheRightOfLoop.find(groupId) == groupsToTheRightOfLoop.end()) {
            totalLeft += count;
        }
    }

    // Check if any group touches the left or right edge of the map.
    for (const auto& row : groups) {
        if (groupsToTheRightOfLoop.find(row[0]) != groupsToTheRightOfLoop.end() || groupsToTheRightOfLoop.find(row[row.size() - 1]) != groupsToTheRightOfLoop.end()) {
            std::cout << totalLeft << '\n';
            return 0;
        }
    }

    // Output the total for the right side.
    std::cout << totalRight << '\n';
    return 0;
}

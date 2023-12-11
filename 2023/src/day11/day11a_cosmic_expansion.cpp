#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Represents a 2D coordinate with row and column values.
struct Coordinate
{
    int row;
    int col;

    // Constructor for Coordinate with default values.
    Coordinate(const int row = 0, const int col = 0) : row(row), col(col) {}
};

// Calculates the total distance between two galaxies, considering the empty rows and columns.
int calculateTotalDistance(const Coordinate &g1, const Coordinate &g2, int emptyRowsDiff, int emptyColsDiff)
{
    return std::abs(g2.row - g1.row) + std::abs(g2.col - g1.col) +
           std::abs(emptyRowsDiff) + std::abs(emptyColsDiff);
}

int main(int argc, char *argv[])
{
    std::string input = "input.txt";
    if (argc > 1)
    {
        input = argv[1];
    }

    std::fstream file(input);
    std::vector<Coordinate> galaxies;
    std::vector<std::string> image;
    std::vector<int> emptyRows;
    std::vector<int> emptyCols;
    std::string line;

    // Read the input file, populate image, galaxies, emptyRows, and emptyCols.
    while (std::getline(file, line))
    {
        image.push_back(line);
        bool hasGalaxy = false;

        // Check each column for galaxies.
        for (int col = 0; col < line.size(); col++)
        {
            if (line[col] == '#')
            {
                galaxies.emplace_back(image.size() - 1, col);
                hasGalaxy = true;
            }
        }

        // If no galaxy in the current row, add the row to emptyRows.
        if (!hasGalaxy)
        {
            emptyRows.push_back(image.size());
        }
    }

    // Check each column for galaxies and add empty columns to emptyCols.
    for (int col = 0; col < image[0].size(); col++)
    {
        bool hasGalaxy = false;
        for (const auto &line : image)
        {
            if (line[col] == '#')
            {
                hasGalaxy = true;
                break;
            }
        }

        // If no galaxy in the current column, add the column to emptyCols.
        if (!hasGalaxy)
        {
            emptyCols.push_back(col);
        }
    }

    // Calculate the number of empty rows and columns before each galaxy.
    std::vector<int> emptyRowsBeforeGalaxy;
    std::vector<int> emptyColsBeforeGalaxy;
    for (const auto &galaxy : galaxies)
    {
        emptyRowsBeforeGalaxy.push_back(std::distance(std::begin(emptyRows),
                                                      std::find_if(std::begin(emptyRows), std::end(emptyRows),
                                                                   [&galaxy](const auto &row)
                                                                   { return galaxy.row < row; })));

        emptyColsBeforeGalaxy.push_back(std::distance(std::begin(emptyCols),
                                                      std::find_if(std::begin(emptyCols), std::end(emptyCols),
                                                                   [&galaxy](const auto &col)
                                                                   { return galaxy.col < col; })));
    }

    int total = 0;

    // Calculate the total distance between each pair of galaxies.
    for (int firstGalaxyIndex = 0; firstGalaxyIndex < galaxies.size(); ++firstGalaxyIndex)
    {
        const auto &g1 = galaxies[firstGalaxyIndex];
        for (int secondGalaxyIndex = firstGalaxyIndex + 1; secondGalaxyIndex < galaxies.size(); ++secondGalaxyIndex)
        {
            const auto &g2 = galaxies[secondGalaxyIndex];
            total += calculateTotalDistance(g1, g2, emptyRowsBeforeGalaxy[secondGalaxyIndex] - emptyRowsBeforeGalaxy[firstGalaxyIndex],
                                            emptyColsBeforeGalaxy[secondGalaxyIndex] - emptyColsBeforeGalaxy[firstGalaxyIndex]);
        }
    }

    // Output the total distance.
    std::cout << total << '\n';
    return 0;
}

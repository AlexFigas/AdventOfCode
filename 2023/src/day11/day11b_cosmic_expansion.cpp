#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

// A struct representing the coordinates of a galaxy.
struct Galaxy
{
    int row;
    int col;

    // Constructor for Galaxy with default values.
    Galaxy(int row = 0, int col = 0) : row(row), col(col) {}
};

// A constant representing a multiplicative factor.
constexpr std::size_t multiplicative_factor = (1000000 - 1);

// Function to read input from a file and return a vector of strings.
std::vector<std::string> readInputFile(const std::string &filename)
{
    std::vector<std::string> lines;
    std::fstream file(filename);

    // Check if the file is open.
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << '\n';
        exit(EXIT_FAILURE);
    }

    std::string line;

    // Read each line from the file and store it in the vector.
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }

    return lines;
}

// Function to find galaxies in the image and populate empty rows and columns.
std::vector<Galaxy> findGalaxies(const std::vector<std::string> &image, std::vector<int> &empty_rows, std::vector<int> &empty_cols)
{
    std::vector<Galaxy> galaxies;

    // Iterate over each row in the image.
    for (int row = 0; row < image.size(); ++row)
    {
        const std::string &line = image[row];
        bool has_galaxy = false;

        // Iterate over each column in the row.
        for (int col = 0; col < line.size(); ++col)
        {
            // If '#' is found, a galaxy is present.
            if (line[col] == '#')
            {
                galaxies.emplace_back(row, col);
                has_galaxy = true;
            }
        }

        // If no galaxy is found in the row, add the row to empty_rows.
        if (!has_galaxy)
        {
            empty_rows.push_back(row);
        }
    }

    // Iterate over each column in the image.
    for (int col = 0; col < image[0].size(); ++col)
    {
        bool has_galaxy = false;

        // Iterate over each row in the column.
        for (const auto &line : image)
        {
            // If '#' is found, a galaxy is present.
            if (line[col] == '#')
            {
                has_galaxy = true;
                break;
            }
        }

        // If no galaxy is found in the column, add the column to empty_cols.
        if (!has_galaxy)
        {
            empty_cols.push_back(col);
        }
    }

    return galaxies;
}

int main(int argc, char *argv[])
{
    std::string input = "input.txt";

    // If command line arguments are provided, use the provided filename.
    if (argc > 1)
    {
        input = argv[1];
    }

    // Read the input from the file.
    std::vector<std::string> image = readInputFile(input);

    // Containers to store galaxies, empty rows, and empty columns.
    std::vector<Galaxy> galaxies;
    std::vector<int> empty_rows;
    std::vector<int> empty_cols;

    // Find galaxies in the image and populate empty rows and columns.
    galaxies = findGalaxies(image, empty_rows, empty_cols);

    // Containers to store the positions of empty rows and columns before each galaxy.
    std::vector<int> empty_rows_before_galaxy;
    std::vector<int> empty_cols_before_galaxy;

    // Calculate the positions of empty rows and columns before each galaxy.
    for (const auto &galaxy : galaxies)
    {
        auto row_it = std::lower_bound(empty_rows.begin(), empty_rows.end(), galaxy.row);
        auto col_it = std::lower_bound(empty_cols.begin(), empty_cols.end(), galaxy.col);

        empty_rows_before_galaxy.push_back(std::distance(empty_rows.begin(), row_it));
        empty_cols_before_galaxy.push_back(std::distance(empty_cols.begin(), col_it));
    }

    std::size_t total = 0;

    // Calculate the total using the given formula.
    for (int idx_1 = 0; idx_1 < galaxies.size(); idx_1++)
    {
        const auto &g1 = galaxies[idx_1];
        for (int idx_2 = idx_1 + 1; idx_2 < galaxies.size(); idx_2++)
        {
            const auto &g2 = galaxies[idx_2];

            // Calculate the total distance for each pair of galaxies.
            total += std::abs(g2.row - g1.row) +
                     std::abs(g2.col - g1.col) +
                     std::abs(empty_rows_before_galaxy[idx_2] - empty_rows_before_galaxy[idx_1]) * multiplicative_factor +
                     std::abs(empty_cols_before_galaxy[idx_2] - empty_cols_before_galaxy[idx_1]) * multiplicative_factor;
        }
    }

    // Output the total.
    std::cout << total << '\n';
    return 0;
}

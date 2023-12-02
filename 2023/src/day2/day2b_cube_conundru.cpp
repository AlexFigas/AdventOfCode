#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/// @brief Function to update the color values in a round based on the color data
void update_colour(std::vector<int> &round, const std::string_view colour_data) {
    size_t i = colour_data.find(' ');
    std::string_view num_str = colour_data.substr(0, i);
    int n = std::stoi(std::string(num_str));

    const auto colour = colour_data.substr(i + 1);

    // Update the corresponding color value in the round
    if (colour == "red") {
        round[0] = n;
    } else if (colour == "green") {
        round[1] = n;
    } else if (colour == "blue") {
        round[2] = n;
    } else {
        std::cerr << "Error: Unexpected color" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

/// @brief Function to parse a round string and extract color data
std::vector<int> parse_round(const std::string_view round_str) {
    std::size_t start = 0;
    std::size_t end = round_str.find(',');
    std::vector<int> round(3, 0);

    // Parse each color data in the round
    while (end != std::string::npos) {
        const std::string_view colour_data = round_str.substr(start, end - start);
        update_colour(round, colour_data);
        start = end + 2;
        end = round_str.find(',', start);
    }

    // Parse the last color data in the round
    const std::string_view colour_data = round_str.substr(start);
    update_colour(round, colour_data);

    return round;
}

/// @brief Struct to represent a game with an ID and rounds
struct Game {
    int id;
    std::vector<std::vector<int>> rounds;
};

/// @brief Function to parse a game string and extract game information
Game parse_game(const std::string_view game_str) {
    std::size_t start = 0;
    std::size_t end = game_str.find(':');
    int game_number = 0;

    // Extract the game number from the game string
    for (const auto ele : game_str.substr(5, end - 5)) {
        game_number = game_number * 10 + (ele - '0');
    }

    start = end + 1;
    end = game_str.find(';', start);
    Game game;
    game.id = game_number;

    // Parse each round in the game
    while (end != std::string::npos) {
        const std::string_view round_data = game_str.substr(start + 1, end - start - 1);
        game.rounds.push_back(parse_round(round_data));
        start = end + 1;
        end = game_str.find(';', start);
    }

    // Parse the last round in the game
    const std::string_view round_data = game_str.substr(start + 1, game_str.size() - start - 1);
    game.rounds.push_back(parse_round(round_data));

    return game;
}

int main() {
    // Open the input file
    std::ifstream input_file("input.txt");

    // Check if the file is opened successfully
    if (!input_file.is_open()) {
        std::cerr << "Error opening input file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    std::vector<Game> games;

    // Read each line from the input file and parse game information
    while (std::getline(input_file, line)) {
        games.emplace_back(parse_game(line));
    }

    std::size_t total = 0;

    // Calculate the total minimum cubes required for all games
    for (const auto &game : games) {
        std::vector<int> min_cubes{0, 0, 0};

        // Iterate through each round and update the minimum cubes
        for (const auto &round : game.rounds) {
            for (int i = 0; i < 3; i++) {
                min_cubes[i] = std::max(min_cubes[i], round[i]);
            }
        }

        // Update the total with the product of minimum cubes
        total += min_cubes[0] * min_cubes[1] * min_cubes[2];
    }

    // Print the total minimum cubes required
    std::cout << total << '\n';
    return EXIT_SUCCESS;
}

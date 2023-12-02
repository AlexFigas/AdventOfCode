#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <map>

const int MAX_RED = 12;
const int MAX_GREEN = 13;
const int MAX_BLUE = 14;

/// @brief Function to update the color values in a round based on the color data
void update_colour(std::vector<int> &round, const std::string_view colour_data) {
    const auto i = colour_data.find(' ');
    const auto colour = colour_data.substr(i + 1);
    const int n = std::stoi(std::string(colour_data.substr(0, i)));
    std::map<std::string_view, int> colour_map = {{"red", 0}, {"green", 1}, {"blue", 2}};

    // Find the color in the map and update the corresponding value in the round
    auto iter = colour_map.find(colour);
    if (iter != colour_map.end()) {
        round[iter->second] = n;
    } else {
        std::cout << "Error: Unexpected color type." << __LINE__ << '\n';
        exit(0);
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

// Function to parse a game string and extract game information
Game parse_game(const std::string_view game_str) {
    const int game_number = std::stoi(std::string(game_str.substr(5, game_str.find(':') - 5)));
    std::size_t start = game_str.find(':') + 1;
    std::size_t end = game_str.find(';');
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
    const std::string_view round_data = game_str.substr(start + 1);
    game.rounds.push_back(parse_round(round_data));

    return game;
}

int main(int argc, char *argv[]) {
    // Open the input file
    std::ifstream input_file("input.txt");

    // Check if the file is opened successfully
    if (!input_file.is_open()) {
        std::cerr << "Error opening input file!" << std::endl;
        return EXIT_FAILURE;
    }

    // Vector to store parsed game information
    std::vector<Game> games;

    // Read each line from the input file and parse game information
    std::string line;
    while (std::getline(input_file, line)) {
        games.emplace_back(parse_game(line));
    }

    // Calculate the total count for playable games
    int count = 0;
    for (const auto &game : games) {
        bool playable = true;
        for (const auto &round : game.rounds) {
            // Check if the color values in each round are within the limits
            if (round[0] > MAX_RED || round[1] > MAX_GREEN || round[2] > MAX_BLUE) {
                playable = false;
                break;
            }
        }

        // If the game is playable, add its ID to the total count
        if (playable) {
            count += game.id;
        }
    }

    // Print the total count of playable games
    std::cout << count << '\n';

    return EXIT_SUCCESS;
}

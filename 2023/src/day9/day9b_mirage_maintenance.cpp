#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Function to find the next value in a sequence
int find_next_value(std::vector<int>& value_sequence) {
  // If there's only one element, duplicate it
  if (value_sequence.size() == 1) {
    value_sequence.emplace_back(value_sequence.back());
    return value_sequence.back();
  }

  // Calculate the difference between consecutive elements
  std::vector<int> new_value_sequence;
  new_value_sequence.reserve(value_sequence.size() - 1);

  for (int i = 0; i < value_sequence.size() - 1; i++) {
    new_value_sequence.emplace_back(value_sequence[i + 1] - value_sequence[i]);
  }

  // If there are non-zero differences, recursively find the next value
  if (std::any_of(value_sequence.begin(), value_sequence.end(), [](const auto& ele) { return ele != 0; })) {
    const auto value_to_add = find_next_value(new_value_sequence);
    value_sequence.emplace_back(value_sequence.back() + value_to_add);
    return value_sequence.back();
  }

  // If all differences are zero, duplicate the last element
  value_sequence.emplace_back(value_sequence.back());
  return value_sequence.back();
}

// Function to extract numbers from a space-separated string
std::vector<int> extract_number(const std::string& s) {
  std::vector<int> numbers;
  std::istringstream iss(s);

  int number;
  // Extract numbers separated by spaces
  while (iss >> number) {
    numbers.emplace_back(number);
  }

  return numbers;
}

int main() {
  std::string input = "input.txt";
  std::ifstream file(input);

  if (!file.is_open()) {
    std::cerr << "Error opening file: " << input << '\n';
    return 1;
  }

  std::vector<std::vector<int>> value_sequences;
  std::string line;

  // Read lines from the file and extract numbers into sequences
  while (std::getline(file, line)) {
    if (!line.empty()) {
      value_sequences.emplace_back(extract_number(line));
    }
  }

  int total = 0;

  // Calculate the total of the next values for each reversed sequence
  for (auto& value_sequence : value_sequences) {
    std::reverse(value_sequence.begin(), value_sequence.end());
    total += find_next_value(value_sequence);
  }

  // Print the total
  std::cout << total << '\n';

  return 0;
}

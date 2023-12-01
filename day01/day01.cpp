#include <iostream>
#include <string_view>
#include <vector>

#include "input.h"

const std::vector<char> numbers = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
const std::vector<std::string_view> numbers_spelled = {
    "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
};

size_t find_forwards(std::string_view str) {
    for (; str.size() >= 1; str.remove_prefix(1)) {
        for (size_t i = 0; i < numbers.size(); i++) {
            if (str.starts_with(numbers[i])) {
                return i + 1;
            }
        }
    }
    return 0;
}
size_t find_backwards(std::string_view str) {
    for (; str.size() >= 1; str.remove_suffix(1)) {
        for (size_t i = 0; i < numbers.size(); i++) {
            if (str.ends_with(numbers[i])) {
                return i + 1;
            }
        }
    }
    return 0;
}

size_t part1(const std::vector<std::string_view> &input) {
    size_t sum{0};
    for (std::string_view str : input) {
        size_t first{find_forwards(str)};
        size_t last{find_backwards(str)};
        sum += 10 * first + last;
    }
    return sum;
}

size_t find_forwards_spelled(std::string_view str) {
    for (; str.size() >= 1; str.remove_prefix(1)) {
        for (size_t i = 0; i < numbers.size(); i++) {
            if (str.starts_with(numbers[i]) ||
                str.starts_with(numbers_spelled[i])) {
                return i + 1;
            }
        }
    }
    return 0;
}
size_t find_backwards_spelled(std::string_view str) {
    for (; str.size() >= 1; str.remove_suffix(1)) {
        for (size_t i = 0; i < numbers.size(); i++) {
            if (str.ends_with(numbers[i]) ||
                str.ends_with(numbers_spelled[i])) {
                return i + 1;
            }
        }
    }
    return 0;
}

size_t part2(const std::vector<std::string_view> &input) {
    size_t sum{0};
    for (std::string_view str : input) {
        size_t first{find_forwards_spelled(str)};
        size_t last{find_backwards_spelled(str)};
        sum += 10 * first + last;
    }
    return sum;
}

int main() {
    std::cout << "Part 1: " << part1(input) << "\n";
    std::cout << "Part 2: " << part2(input) << "\n";
}

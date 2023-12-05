#include "input.h"

#include <charconv>

size_t number_length(std::string_view str, size_t start) {
    str.remove_prefix(start);
    size_t length = str.find_first_of(NONDIGIT);
    if (length == std::string_view::npos) {
        return str.length();
    } else {
        return length;
    }
}

bool is_symbol(char c) {
    return std::string_view{SYMBOLS}.find(c) != std::string_view::npos;
}

bool is_digit(std::string_view str, int pos) {
    if (pos < 0 || pos >= static_cast<int>(str.length())) {
        return false;
    } else {
        return std::string_view{DIGIT}.find(str.at(pos)) !=
               std::string_view::npos;
    }
}
std::vector<point> get_gears(const std::vector<std::string_view> &input) {
    std::vector<point> result{};
    for (size_t row{0}; row < input.size(); ++row) {
        const auto str = input[row];
        for (size_t col{0}; col < str.length(); ++col) {
            if (str.at(col) == '*') {
                result.push_back({.row = static_cast<int>(row),
                                  .col = static_cast<int>(col)});
            }
        }
    }
    return result;
}
std::vector<point> get_symbols(const std::vector<std::string_view> &input) {
    std::vector<point> result{};
    for (size_t row{0}; row < input.size(); ++row) {
        const auto str = input[row];
        for (size_t col{0}; col < str.length(); ++col) {
            if (is_symbol(str.at(col))) {
                result.push_back({.row = static_cast<int>(row),
                                  .col = static_cast<int>(col)});
            }
        }
    }
    return result;
}
std::vector<part_number> get_part_numbers(
    const std::vector<std::string_view> &input) {
    std::vector<part_number> result{};
    for (size_t row{0}; row < input.size(); ++row) {
        const auto str = input[row];
        for (size_t col{0}; col < str.length(); ++col) {
            if (is_digit(str, col)) {
                // length of the current number
                size_t size{number_length(str, col)};
                int number;
                std::from_chars(str.data() + col, str.data() + col + size,
                                number);
                result.push_back({.number = number,
                                  .row = static_cast<int>(row),
                                  .start = static_cast<int>(col),
                                  .end = static_cast<int>(col + size - 1)});
                // Skip rest of number
                col += size;
            }
        }
    }
    return result;
}

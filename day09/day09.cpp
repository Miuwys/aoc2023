
#include <charconv>
#include <iostream>
#include <vector>
#include <ranges>

#include "input.h"

using problem = std::vector<std::vector<int64_t>>;

std::vector<int64_t> get_differences(std::vector<int64_t> values) {
    std::vector<int64_t> result{};
    for (size_t i{1}; i < values.size(); ++i) {
        result.emplace_back(values[i] - values[i - 1]);
    }
    return result;
}

bool all_zero(std::vector<int64_t> values) {
    return std::all_of(values.begin(), values.end(),
                       [](int64_t v) { return v == 0; });
}

int64_t part1(problem p) {
    int64_t sum{0};
    for (std::vector<int64_t> value : p) {
        std::vector<std::vector<int64_t>> tree{value};
        while (!all_zero(tree.back())) {
            tree.emplace_back(get_differences(tree.back()));
        }
        int64_t new_value{0};
        for (auto t : tree) {
            new_value += t.back();
        }
        sum += new_value;
    }
    return sum;
}

int64_t part2(const problem p) {
    int64_t sum{0};
    for (std::vector<int64_t> value : p) {
        std::vector<std::vector<int64_t>> tree{value};
        while (!all_zero(tree.back())) {
            tree.emplace_back(get_differences(tree.back()));
        }
        int64_t new_value{0};
        for (auto t : tree | std::views::reverse) {
            new_value = t.at(0) - new_value;
            std::cout << new_value << "\n";
        }
        sum += new_value;
    }
    return sum;
}

std::vector<std::string_view> split(std::string_view str, char split_char) {
    std::vector<std::string_view> result{};
    for (size_t idx = str.find(split_char); idx != std::string_view::npos;
         idx = str.find(split_char)) {
        if (idx > 0) {
            result.push_back(str.substr(0, idx));
        }
        str.remove_prefix(idx + 1);
    }
    result.push_back(str);
    return result;
}

int64_t extract_number(std::string_view str) {
    size_t start = str.find_first_of("-0123456789");
    size_t stop = str.find_last_of("0123456789") + 1;
    int64_t result;
    std::from_chars(str.data() + start, str.data() + stop, result);
    return result;
}

problem parse_input(const std::vector<std::string_view> &input) {
    problem result{};
    for (auto l : input) {
        std::vector<int64_t> values{};
        auto numbers = split(l, ' ');
        for (auto n : numbers) {
            values.emplace_back(extract_number(n));
        }
        result.emplace_back(values);
    }
    return result;
}

int main() {
    const problem p = parse_input(input);
    int64_t result_1{part1(p)};
    std::cout << "Part 1: " << result_1 << "\n";
    int64_t result_2{part2(p)};
    std::cout << "Part 2: " << result_2 << "\n";
}

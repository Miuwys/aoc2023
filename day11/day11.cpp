
#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>

#include "input.h"

using problem = std::map<std::pair<size_t, size_t>, size_t>;

problem parse_input(const std::vector<std::string_view> &input, size_t empty_space_size) {
    problem result{};
    std::map<size_t, bool> empty_rows{};
    std::map<size_t, bool> empty_columns{};

    // Find empty space
    for (size_t i{0}; i < input.size(); ++i) {
        auto is_empty = [](char c) { return c == '.'; };
        auto &row = input.at(i);
        if (std::all_of(row.begin(), row.end(), is_empty)) {
            empty_rows[i] = true;
        }
    }
    for (size_t j{0}; j < input.size(); ++j) {
        auto is_empty_col = [=](const std::string_view &row) {
            return row.at(j) == '.';
        };
        if (std::all_of(input.begin(), input.end(), is_empty_col)) {
            empty_columns[j] = true;
        }
    }

    // Read points
    size_t galaxy_number{1};
    for (size_t i{0}, actual_row{0}; i < input.size(); ++i, ++actual_row) {
        const auto &row = input.at(i);
        if (empty_rows.contains(i)) {
            actual_row += empty_space_size-1;
        }
        for (size_t j{0}, actual_col{0}; j < row.size(); ++j, ++actual_col) {
        if (empty_columns.contains(j)) {
            actual_col += empty_space_size-1;
        }
            if (row.at(j) == '#') {
                result[{actual_row, actual_col}] = galaxy_number++;
            }
        }
    }
    return result;
}

template <typename T>
T abs_diff(T a, T b) {
    return a > b ? a - b : b - a;
}

size_t sum_of_distances(problem p) {
    size_t sum{0};
    for (auto i{p.begin()}; i != p.end(); ++i) {
        for (auto j{i}; j != p.end(); ++j) {
            std::pair<size_t, size_t> i_coord = i->first;
            std::pair<size_t, size_t> j_coord = j->first;
            auto distance = abs_diff(i_coord.first, j_coord.first) +
                   abs_diff(i_coord.second, j_coord.second);
            sum += distance;
        }
    }
    return sum;
}

int main() {
    const problem p1 = parse_input(input, 2);
    size_t result_1{sum_of_distances(p1)};
    std::cout << "Part 1: " << result_1 << "\n";
    const problem p2 = parse_input(input, 1000000);
    size_t result_2{sum_of_distances(p2)};
    std::cout << "Part 2: " << result_2 << "\n";
}

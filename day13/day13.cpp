

#include <algorithm>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include "input.h"

using map = std::vector<std::string>;
using problem = std::vector<map>;

struct Mirror {
    Mirror(bool horizontal, size_t position)
        : horizontal{horizontal}, position{position} {}
    bool operator==(const Mirror &other) const = default;
    bool operator!=(const Mirror &other) const { return !(*this == other); }
    bool horizontal;
    size_t position;
};

problem parse_input(const std::vector<std::string_view> &input) {
    problem result{};
    std::vector<std::string> map{};
    for (auto row : input) {
        if (row.empty()) {
            result.push_back(map);
            map = {};
        } else {
            map.push_back(std::string{row});
        }
    }
    result.push_back(map);
    return result;
}

template <typename T, class InputIt>
bool stack_matches(InputIt first, InputIt last, std::stack<T> values) {
    for (; first != last; ++first) {
        if (values.empty()) {
            // Found match
            return true;
        } else if (values.top() != *first) {
            // No match
            return false;
        }
        values.pop();
    }
    // Iterator empty, all match
    return true;
}

template <typename T, class InputIt>
std::vector<size_t> find_mirror_axes(InputIt first, InputIt last) {
    std::vector<size_t> mirrors{};
    std::stack<T> previous_values{};
    for (auto it{first}; it != last; ++it) {
        if (previous_values.size() > 0 && *it == previous_values.top() &&
            stack_matches(it, last, previous_values)) {
            mirrors.push_back(std::distance(first, it));
        }
        previous_values.push(*it);
    }
    return mirrors;
}

std::vector<size_t> get_vertical_split(map m) {
    std::vector<size_t> possible_mirrors =
        find_mirror_axes<char>(m.at(0).begin(), m.at(0).end());
    for (auto row : m) {
        auto mirrors = find_mirror_axes<char>(row.begin(), row.end());
        for (auto it{possible_mirrors.begin()}; it != possible_mirrors.end();) {
            if (std::find(mirrors.begin(), mirrors.end(), *it) ==
                mirrors.end()) {
                it = possible_mirrors.erase(it);
            } else {
                ++it;
            }
        }
        if (possible_mirrors.size() == 0) {
            return possible_mirrors;
        }
    }
    return possible_mirrors;
}

std::vector<Mirror> get_all_mirrors(map m) {
    std::vector<Mirror> result{};
    // Find horizontal mirrors
    for (size_t axis : find_mirror_axes<std::string_view>(m.begin(), m.end())) {
        result.emplace_back(Mirror{true, axis});
    }
    // Find vertical mirrors
    std::vector<size_t> possible_mirrors =
        find_mirror_axes<char>(m.at(0).begin(), m.at(0).end());
    for (auto row : m) {
        auto mirrors = find_mirror_axes<char>(row.begin(), row.end());
        for (auto it{possible_mirrors.begin()}; it != possible_mirrors.end();) {
            if (std::find(mirrors.begin(), mirrors.end(), *it) ==
                mirrors.end()) {
                it = possible_mirrors.erase(it);
            } else {
                ++it;
            }
        }
        if (possible_mirrors.size() == 0) {
            break;
        }
    }
    for (size_t axis : possible_mirrors) {
        result.emplace_back(Mirror{false, axis});
    }

    return result;
}

size_t part1(problem p) {
    size_t sum{0};
    for (map m : p) {
        Mirror mirr = get_all_mirrors(m).at(0);
        if (mirr.horizontal) {
            sum += 100 * mirr.position;
        } else {
            sum += mirr.position;
        }
    }
    return sum;
}

size_t get_flipped_mirror(map m) {
    auto old_mirror = get_all_mirrors(m).at(0);
    // Try to flip every symbol
    for (size_t i{0}; i < m.size(); ++i) {
        for (size_t j{0}; j < m[i].length(); ++j) {
            map m_copy{m};
            if (m[i][j] == '.') {
                m_copy[i][j] = '#';
            } else {
                m_copy[i][j] = '.';
            }
            auto new_mirrors = get_all_mirrors(m_copy);
            // Find a different mirror to the old one
            auto different_mirror =
                std::find_if(new_mirrors.begin(), new_mirrors.end(),
                             [=](Mirror m) { return m != old_mirror; });
            // If a different mirror exists, return the value
            if (different_mirror != new_mirrors.end()) {
                if (different_mirror->horizontal) {
                    return different_mirror->position * 100;
                } else {
                    return different_mirror->position;
                }
            }
        }
    }
    throw std::logic_error("Found no matches");
}

size_t part2(problem p) {
    size_t sum{0};
    for (map m : p) {
        sum += get_flipped_mirror(m);
    }
    return sum;
}

int main() {
    const problem p = parse_input(input);
    size_t result_1{part1(p)};
    std::cout << "Part 1: " << result_1 << "\n";
    size_t result_2{part2(p)};
    std::cout << "Part 2: " << result_2 << "\n";
}

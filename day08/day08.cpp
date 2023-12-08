#include <algorithm>
#include <charconv>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <string_view>
#include <utility>

#include "input.h"

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

std::string_view extract_node(std::string_view str) {
    auto result{str};
    size_t start = result.find_first_of("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    result.remove_prefix(start);
    size_t stop =
        result.find_last_of("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ") + 1;
    result.remove_suffix(result.length() - stop);
    return result;
}

bool contains(std::string_view str, std::string_view search) {
    return str.find(search) != std::string_view::npos;
}

using hand = std::string_view;
struct problem {
    std::string_view steps;
    std::map<std::string_view, std::pair<std::string_view, std::string_view>>
        network;
};

problem parse_input(const std::vector<std::string_view> &input) {
    problem result{input.at(0), {}};
    for (std::string_view line : input) {
        if (!contains(line, "=")) {
            continue;
        }
        auto equals_split = split(line, '=');
        auto node = equals_split.at(0);
        auto values = split(equals_split.at(1), ',');
        result.network[extract_node(node)] = {extract_node(values.at(0)),
                                              extract_node(values.at(1))};
    }
    return result;
}

size_t part1(problem p) {
    std::string_view current{"AAA"};
    size_t steps{0};
    while (current != "ZZZ") {
        char next_step = p.steps[steps % p.steps.length()];
        if (next_step == 'L') {
            current = p.network[current].first;
        } else {
            current = p.network[current].second;
        }
        ++steps;
    }
    return steps;
}

size_t gcd(size_t a, size_t b) {
    if (a == 0) return b;
    return gcd(b % a, a);
}

size_t lcm(size_t a, size_t b) { return (a * b) / gcd(a, b); }

bool all_end_with(std::vector<std::string_view> nodes, char c) {
    return std::all_of(nodes.begin(), nodes.end(),
                       [=](std::string_view n) { return n[2] == c; });
}

size_t steps2(problem p, std::string_view start) {
    std::string_view current = start;
    size_t steps{0};
    while (current[2] != 'Z') {
        char next_step = p.steps[steps % p.steps.length()];
        if (next_step == 'L') {
            current = p.network[current].first;
        } else {
            current = p.network[current].second;
        }
        ++steps;
    }
    return steps;
}

size_t part2(problem p) {
    std::vector<std::string_view> current{};
    size_t current_lcm{1};
    for (auto n : p.network) {
        if (n.first[2] == 'A') {
            current.push_back(n.first);
            current_lcm = lcm(current_lcm, steps2(p, n.first));
        }
    }
    return current_lcm;
}

int main() {
    const problem p = parse_input(input);
    size_t result_1{part1(p)};
    std::cout << "Part 1: " << result_1 << "\n";
    size_t result_2{part2(p)};
    std::cout << "Part 2: " << result_2 << "\n";
}

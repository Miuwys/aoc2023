
#include <algorithm>
#include <array>
#include <charconv>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <stdexcept>
#include <vector>

enum class Comparison { less, greater };
struct Condition {
    char variable;
    Comparison comp;
    size_t value;
};
using Instructions =
    std::vector<std::pair<std::optional<Condition>, std::string>>;

struct Problem {
    std::map<std::string, Instructions> workflows;
    std::vector<std::array<size_t, 4>> parts;
    size_t count_accepting(std::array<std::pair<size_t, size_t>, 4> xmas, std::string workflow) {
        if (workflow == "R") {
            return 0;
        } else if (workflow == "A") {
            return (xmas[0].second - xmas[0].first + 1) * (xmas[1].second - xmas[1].first + 1) *
                   (xmas[2].second - xmas[2].first + 1) * (xmas[3].second - xmas[3].first + 1);
        } else {
            Instructions instructions = workflows[workflow];
            size_t possibilities{0};
            for (auto &[cond, next] : instructions) {
                if (!cond.has_value()) {
                    possibilities += count_accepting(xmas, next);
                    return possibilities;
                } else {
                    Condition condition = *cond;
                    size_t i{0};
                    switch (condition.variable) {
                        case 'x':
                            i = 0;
                            break;
                        case 'm':
                            i = 1;
                            break;
                        case 'a':
                            i = 2;
                            break;
                        case 's':
                            i = 3;
                            break;
                    }
                    // Check the branch
                    if (condition.comp == Comparison::greater) {
                        // Greater
                        size_t old_min = xmas[i].first;
                        xmas[i].first = condition.value + 1;
                        if (xmas[i].first <= xmas[i].second) {
                            possibilities += count_accepting(xmas, next);
                        }
                        xmas[i].first = old_min;
                        xmas[i].second = condition.value;
                    } else {
                        // Less
                        size_t old_max = xmas[i].second;
                        xmas[i].second = condition.value - 1;
                        if (xmas[i].first <= xmas[i].second) {
                            possibilities += count_accepting(xmas, next);
                        }
                        xmas[i].second = old_max;
                        xmas[i].first = condition.value;
                    }
                    if (xmas[i].first > xmas[i].second) {
                        break;
                    }
                }
            }
            return possibilities;
        }
    }
};

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

size_t extract_number(std::string_view str) {
    size_t start = str.find_first_of("0123456789");
    size_t stop = str.find_last_of("0123456789") + 1;
    size_t result;
    std::from_chars(str.data() + start, str.data() + stop, result);
    return result;
}

Problem parse_input(const std::string &path) {
    Problem result{};
    std::ifstream input{path};
    // read workflows
    for (std::string line; getline(input, line);) {
        if (line.empty()) break;
        std::vector<std::string_view> s = split(line, '{');
        std::string workflow{s.at(0)};
        for (std::string_view instruction : split(s.at(1), ',')) {
            if (instruction.find(':') == instruction.npos) {
                // No condition
                result.workflows[workflow].push_back(
                    {std::nullopt,
                     std::string{instruction.begin(), instruction.end() - 1}});
            } else {
                auto s = split(instruction, ':');
                std::string_view condition = s.at(0);
                std::string next_workflow{s.at(1)};
                char variable = condition.at(0);
                size_t value = extract_number(condition);
                Comparison comp;
                if (condition.at(1) == '>') {
                    comp = Comparison::greater;
                } else {
                    comp = Comparison::less;
                }
                result.workflows[workflow].push_back(
                    {std::make_optional<Condition>({variable, comp, value}),
                     next_workflow});
            }
        }
    }
    // read parts
    for (std::string line; getline(input, line);) {
        std::string_view str{line};
        // remove curly braces
        str.remove_prefix(1);
        str.remove_suffix(1);
        std::vector<std::string_view> s = split(str, ',');
        result.parts.push_back(
            {{extract_number(s.at(0)), extract_number(s.at(1)),
              extract_number(s.at(2)), extract_number(s.at(3))}});
    }
    return result;
}

size_t part1(Problem p) {
    size_t sum{0};
    for (auto part : p.parts) {
        std::string next_state{"in"};
        while (next_state != "A" && next_state != "R") {
            Instructions workflow = p.workflows[next_state];
            for (auto &[cond, next] : workflow) {
                if (!cond.has_value()) {
                    next_state = next;
                    break;
                } else {
                    Condition condition = *cond;
                    size_t var;
                    switch (condition.variable) {
                        case 'x':
                            var = part.at(0);
                            break;
                        case 'm':
                            var = part.at(1);
                            break;
                        case 'a':
                            var = part.at(2);
                            break;
                        case 's':
                            var = part.at(3);
                            break;
                    }
                    if ((condition.comp == Comparison::greater &&
                         var > condition.value) ||
                        (condition.comp == Comparison::less &&
                         var < condition.value)) {
                        next_state = next;
                        break;
                    }
                }
            }
        }
        if (next_state == "A") {
            sum += part[0] + part[1] + part[2] + part[3];
        }
    }
    return sum;
}

size_t part2(Problem p) {
    return p.count_accepting({{{1, 4000}, {1, 4000}, {1, 4000}, {1, 4000}}}, "in");
}

int main() {
    std::string path{"/home/chris/aoc2023/day19/input"};
    const Problem p{parse_input(path)};
    size_t result_1{part1(p)};
    std::cout << "Part 1: " << result_1 << "\n";
    size_t result_2{part2(p)};
    std::cout << "Part 2: " << result_2 << "\n";
}

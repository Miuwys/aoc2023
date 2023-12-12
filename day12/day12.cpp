
#include <algorithm>
#include <charconv>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <stdexcept>

#include "input.h"

using problem = std::vector<std::pair<std::string_view, std::vector<size_t>>>;

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

std::vector<size_t> extract_numbers(std::vector<std::string_view> strs) {
    std::vector<size_t> result{};
    for (auto str : strs) {
        size_t start = str.find_first_of("-1234567890");
        size_t stop = str.find_last_of("1234567890") + 1;
        size_t n;
        std::from_chars(str.data() + start, str.data() + stop, n);
        result.emplace_back(n);
    }
    return result;
}

problem parse_input(const std::vector<std::string_view> &input) {
    problem result{};
    for (auto row : input) {
        auto s = split(row, ' ');
        auto parts = s.at(0);
        auto numbers = s.at(1);
        std::vector<size_t> records = extract_numbers(split(numbers, ','));
        result.push_back({parts, records});
    }
    return result;
}

enum NFA_type { loop, sequence, sequence_end, end };

size_t part1(problem p) {
    size_t sum{0};
    for (auto &[parts, records] : p) {
        std::map<size_t, NFA_type> dfa{};
        // Build DFA for the record
        size_t i{0};
        for (auto record : records) {
            dfa[i++] = loop;
            for (size_t j{1}; j < record; ++j) {
                dfa[i++] = sequence;
            }
            dfa[i++] = sequence_end;
        }
        dfa[i - 1] = end;
        std::map<size_t, size_t> nfa_state{{0, 1}};
        // Do runs
        for (char c : parts) {
            std::map<size_t, size_t> new_states{};
            for (auto &[state, amount] : nfa_state) {
                switch (dfa[state]) {
                    case loop:
                        if (c == '?') {
                            // Goes to both states
                            new_states[state] += amount;
                            new_states[state + 1] += amount;
                        } else if (c == '.') {
                            // Stays in state
                            new_states[state] += amount;
                        } else if (c == '#') {
                            // Goes to next state
                            new_states[state + 1] += amount;
                        }
                        break;
                    case sequence:
                        if (c == '?') {
                            // Needs to be #, Advances state
                            new_states[state + 1] += amount;
                        } else if (c == '.') {
                            // Impossible -> Ignore state
                        } else if (c == '#') {
                            // Goes to next state
                            new_states[state + 1] += amount;
                        }
                        break;
                    case sequence_end:
                        if (c == '?') {
                            // Needs to be ., Advances state
                            new_states[state + 1] += amount;
                        } else if (c == '.') {
                            // Goes to next state
                            new_states[state + 1] += amount;
                        } else if (c == '#') {
                            // Impossible -> Ignore state
                        }
                        break;
                    case end:
                        if (c == '?') {
                            // Needs to be ., stays in end
                            new_states[state] += amount;
                        } else if (c == '.') {
                            // Stays in end
                            new_states[state] += amount;
                        } else if (c == '#') {
                            // Impossible, ignore state
                        }
                        break;
                }
            }
            nfa_state = std::move(new_states);
        }
        sum += nfa_state[dfa.size() - 1];
    }
    return sum;
}

size_t part2(problem p) {
    // Save the new strings so they don't get freed
    std::vector<std::string> strings{};
    for (auto &[parts, records] : p) {
        std::string new_parts{};
        std::vector<size_t> new_records{};
        for (size_t i{0}; i < 5; ++i) {
            new_parts.append(parts);
            if (i != 4) new_parts.push_back('?');
            new_records.insert(new_records.end(), records.begin(),
                               records.end());
        }
        parts = std::string_view{new_parts};
        records = new_records;
        strings.emplace_back(std::move(new_parts));
    }
    return part1(p);
}

int main() {
    const problem p = parse_input(input);
    size_t result_1{part1(p)};
    std::cout << "Part 1: " << result_1 << "\n";
    size_t result_2{part2(p)};
    std::cout << "Part 2: " << result_2 << "\n";
}

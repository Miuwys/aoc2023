
#include <algorithm>
#include <iostream>
#include <map>
#include <stack>
#include <stdexcept>
#include <string>

#include "input.h"

using problem = std::vector<std::vector<char>>;

problem parse_input(const std::vector<std::string_view> &input) {
    problem result{};
    for (auto row : input) {
        std::vector<char> v{};
        for (char c : row) {
            v.emplace_back(c);
        }
        result.emplace_back(std::move(v));
    }
    return result;
}

void print(problem p) {
    for (auto row : p) {
        for (char c : row) {
            std::cout << c;
        }
        std::cout << "\n";
    }
}

problem tilt_north(problem &p) {
    for (size_t col{0}; col < p.at(0).size(); ++col) {
        size_t free{0};
        for (size_t row{0}; row < p.size(); ++row) {
            switch (p[row][col]) {
                case 'O':
                    p[row][col] = '.';
                    p[free++][col] = 'O';
                    break;
                case '#':
                    free = row + 1;
                    break;
                default:
                    break;
            }
        }
    }
    return p;
}

problem tilt_south(problem &p) {
    for (size_t col{0}; col < p.at(0).size(); ++col) {
        size_t free_row{p.size() - 1};
        for (size_t row{p.size() - 1};; --row) {
            switch (p[row][col]) {
                case 'O':
                    p[row][col] = '.';
                    p[free_row--][col] = 'O';
                    break;
                case '#':
                    free_row = row - 1;
                    break;
                default:
                    break;
            }
            // Avoid overflow
            if (row == 0) break;
        }
    }
    return p;
}

problem tilt_west(problem &p) {
    for (size_t row{0}; row < p.size(); ++row) {
        size_t free_col{0};
        for (size_t col{0}; col < p.at(0).size(); ++col) {
            switch (p[row][col]) {
                case 'O':
                    p[row][col] = '.';
                    p[row][free_col++] = 'O';
                    break;
                case '#':
                    free_col = col + 1;
                    break;
                default:
                    break;
            }
        }
    }
    return p;
}

problem tilt_east(problem p) {
    for (size_t row{0}; row < p.size(); ++row) {
        size_t free_col{p.at(0).size() - 1};
        for (size_t col{p.at(0).size() - 1};; --col) {
            switch (p[row][col]) {
                case 'O':
                    p[row][col] = '.';
                    p[row][free_col--] = 'O';
                    break;
                case '#':
                    free_col = col - 1;
                    break;
                default:
                    break;
            }
            // Avoid overflow
            if (col == 0) break;
        }
    }
    return p;
}

size_t get_north_beam_load(const problem &p) {
    size_t sum{0};
    for (size_t row{0}, distance_to_south{p.size()}; row < p.size();
         ++row, --distance_to_south) {
        auto stones = p[row];
        for (char stone : stones) {
            if (stone == 'O') {
                sum += distance_to_south;
            }
        }
    }
    return sum;
}

size_t part1(problem p) {
    return get_north_beam_load(tilt_north(p));
}

size_t part2(problem p) {
    std::map<problem, size_t> seen{};
    for (size_t i{0}; i < 1000000000; ++i) {
        p = tilt_north(p);
        p = tilt_west(p);
        p = tilt_south(p);
        p = tilt_east(p);
        if (seen.find(p) != seen.end()) {
            // Was seen before, there is a cycle
            auto [_, cycle_start] = *seen.find(p);
            size_t cycle_length{i - cycle_start};
            size_t remaining{(999999999 - i) % cycle_length};
            for (size_t i{0}; i < remaining; ++i) {
                p = tilt_north(p);
                p = tilt_west(p);
                p = tilt_south(p);
                p = tilt_east(p);
            }
            return get_north_beam_load(p);
        } else {
            seen.insert({p, i});
        }
    }
    return get_north_beam_load(p);
}

int main() {
    const problem p = parse_input(input);
    size_t result_1{part1(p)};
    std::cout << "Part 1: " << result_1 << "\n";
    size_t result_2{part2(p)};
    std::cout << "Part 2: " << result_2 << "\n";
}

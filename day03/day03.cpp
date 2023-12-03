#include <charconv>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <valarray>
#include <vector>

#include "input.h"


bool is_adjacent(part_number pn, point g) {
    // Check row
    if (std::abs(pn.row - g.row) > 1) {
        return false;
    }
    // Check column
    return pn.start <= g.col + 1 && pn.end >= g.col - 1;
}

size_t part1(const std::vector<part_number> &part_numbers, const std::vector<point> symbols) {
    size_t sum{0};

    for (part_number pn : part_numbers) {
        for (point s : symbols) {
            if (is_adjacent(pn, s)) {
                sum += pn.number;
            }
        }
    }
    return sum;
}

size_t part2(const std::vector<part_number> &part_numbers, const std::vector<point> &gears) {
    size_t result{0};
    for (point g : gears) {
        size_t adjacent{0};
        size_t ratio{1};
        for (part_number pn : part_numbers) {
            if (is_adjacent(pn, g)) {
                ++adjacent;
                ratio *= pn.number;
            }
        }
        if (adjacent == 2) {
            result += ratio;
        }
    }
    return result;
}

int main() {
    const auto part_numbers = get_part_numbers(input);
    const auto symbols = get_symbols(input);
    const auto gears = get_gears(input);
    size_t result_1{part1(part_numbers, symbols)};
    std::cout << "Part 1: " << result_1 << "\n";
    size_t result_2{part2(part_numbers, gears)};
    std::cout << "Part 2: " << result_2 << "\n";
}

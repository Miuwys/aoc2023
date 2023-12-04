#include <algorithm>
#include <charconv>
#include <iostream>
#include <numeric>

#include "input.h"

struct card {
    int id;
    std::vector<int> winning_numbers;
    std::vector<int> own_numbers;
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

int extract_number(std::string_view str) {
    size_t start = str.find_first_of("0123456789");
    size_t stop = str.find_last_of("0123456789") + 1;
    int result;
    std::from_chars(str.data() + start, str.data() + stop, result);
    return result;
}

std::vector<card> parse_input(const std::vector<std::string_view> &input) {
    std::vector<card> result{};
    for (std::string_view line : input) {
        auto start = split(line, ':');
        auto numbers = split(start.at(1), '|');
        int id = extract_number(start.at(0));

        auto winning_numbers = split(numbers.at(0), ' ');
        auto own_numbers = split(numbers.at(1), ' ');

        std::vector<int> winning_numbers_parsed{};
        for (std::string_view winning_number : winning_numbers) {
            winning_numbers_parsed.push_back(extract_number(winning_number));
        }
        std::vector<int> own_numbers_parsed{};
        for (std::string_view own_number : own_numbers) {
            own_numbers_parsed.push_back(extract_number(own_number));
        }

        result.push_back({id, winning_numbers_parsed, own_numbers_parsed});
    }
    return result;
}

bool is_winning(int n, const std::vector<int> &winning_numbers) {
    return std::find(winning_numbers.begin(), winning_numbers.end(), n) !=
           winning_numbers.end();
}

size_t part1(const std::vector<card> &cards) {
    size_t sum{0};

    for (card c : cards) {
        size_t winnings{0};
        for (int n : c.own_numbers) {
            if (is_winning(n, c.winning_numbers)) {
                winnings = winnings ? winnings + winnings : 1;
            }
        }
        sum += winnings;
    }
    return sum;
}

void insert_scratchcard(size_t pos, std::vector<size_t> &scratchcards,
                        size_t increment = 1) {
    for (size_t i = scratchcards.size(); i <= pos; ++i) {
        scratchcards.push_back(0);
    }
    scratchcards.at(pos) += increment;
}

size_t part2(const std::vector<card> &cards) {
    std::vector<size_t> scratchcards_amount{0};

    for (card c : cards) {
        insert_scratchcard(c.id, scratchcards_amount);
        size_t scratchcards{scratchcards_amount.at(c.id)};

        size_t winnings{0};
        for (int n : c.own_numbers) {
            if (is_winning(n, c.winning_numbers)) {
                ++winnings;
            }
        }
        for (size_t i = 1; i < winnings + 1; ++i) {
            insert_scratchcard(c.id + i, scratchcards_amount, scratchcards);
        }
    }
    return std::accumulate(scratchcards_amount.begin(),
                           scratchcards_amount.end(), 0);
}

int main() {
    const std::vector<card> cards = parse_input(input);
    size_t result_1{part1(cards)};
    std::cout << "Part 1: " << result_1 << "\n";
    size_t result_2{part2(cards)};
    std::cout << "Part 2: " << result_2 << "\n";
}

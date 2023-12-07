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

size_t extract_number(std::string_view str) {
    size_t start = str.find_first_of("0123456789");
    size_t stop = str.find_last_of("0123456789") + 1;
    size_t result;
    std::from_chars(str.data() + start, str.data() + stop, result);
    return result;
}

bool contains(std::string_view str, std::string_view search) {
    return str.find(search) != std::string_view::npos;
}

using hand = std::string_view;
using problem = std::vector<std::pair<hand, size_t>>;

problem parse_input(const std::vector<std::string_view> &input) {
    problem result{};
    for (std::string_view line : input) {
        auto numbers = split(line, ' ');
        result.emplace_back(numbers.at(0), extract_number(numbers.at(1)));
    }
    return result;
}

size_t count_card_1(std::string_view hand, char c) {
    return std::count(hand.begin(), hand.end(), c);
}

size_t count_card_2(std::string_view hand, char c) {
    if (c == 'J') {
        return 0;
    }
    return std::count(hand.begin(), hand.end(), c);
}

bool is_five_of_kind(std::vector<size_t> &counts, size_t jokers) {
    return *std::max_element(counts.begin(), counts.end()) + jokers == 5;
}

bool is_four_of_kind(std::vector<size_t> &counts, size_t jokers) {
    return *std::max_element(counts.begin(), counts.end()) + jokers == 4;
}

bool is_three_of_kind(std::vector<size_t> &counts, size_t jokers) {
    return *std::max_element(counts.begin(), counts.end()) + jokers == 3;
}

bool is_one_pair(std::vector<size_t> &counts, size_t jokers) {
    return *std::max_element(counts.begin(), counts.end()) + jokers == 2;
}

bool is_two_pair(std::vector<size_t> &counts) {
    // If there are joker, it never makes sense to build a two pair
    return std::count(counts.begin(), counts.end(), 2) == 4;
}

bool is_full_house(std::vector<size_t> &counts, size_t jokers) {
    if (std::count(counts.begin(), counts.end(), 2) == 2 &&
        std::count(counts.begin(), counts.end(), 3) == 3) {
        return true;
    } else if (jokers == 1) {
        // needs two pairs
        return is_two_pair(counts);
    } else if (jokers == 2) {
        return std::count(counts.begin(), counts.end(), 1) == 1 &&
               std::count(counts.begin(), counts.end(), 2) == 2;
    } else if (jokers >= 3) {
        // Can always build full house
        return true;
    }
    return false;
}

size_t get_type(std::string_view hand) {
    std::vector<size_t> counts{
        count_card_1(hand, hand.at(0)), count_card_1(hand, hand.at(1)),
        count_card_1(hand, hand.at(2)), count_card_1(hand, hand.at(3)),
        count_card_1(hand, hand.at(4))};
    if (is_five_of_kind(counts, 0)) {
        return 6;
    } else if (is_four_of_kind(counts, 0)) {
        return 5;
    } else if (is_full_house(counts, 0)) {
        return 4;
    } else if (is_three_of_kind(counts, 0)) {
        return 3;
    } else if (is_two_pair(counts)) {
        return 2;
    } else if (is_one_pair(counts, 0)) {
        return 1;
    } else {
        return 0;
    }
}

std::map<char, size_t> card_strengths{
    {'2', 2}, {'3', 3},  {'4', 4},  {'5', 5},  {'6', 6},  {'7', 7}, {'8', 8},
    {'9', 9}, {'T', 10}, {'J', 11}, {'Q', 12}, {'K', 13}, {'A', 15}};

bool tiebreaker_1(hand h1, hand h2) {
    if (h1.at(0) != h2.at(0)) {
        return card_strengths.find(h1.at(0))->second <
               card_strengths.find(h2.at(0))->second;
    } else if (h1.at(1) != h2.at(1)) {
        return card_strengths.find(h1.at(1))->second <
               card_strengths.find(h2.at(1))->second;
    } else if (h1.at(2) != h2.at(2)) {
        return card_strengths.find(h1.at(2))->second <
               card_strengths.find(h2.at(2))->second;
    } else if (h1.at(3) != h2.at(3)) {
        return card_strengths.find(h1.at(3))->second <
               card_strengths.find(h2.at(3))->second;
    } else if (h1.at(4) != h2.at(4)) {
        return card_strengths.find(h1.at(4))->second <
               card_strengths.find(h2.at(4))->second;
    }
    return false;
}
std::map<char, size_t> card_strengths_2{
    {'2', 2}, {'3', 3},  {'4', 4}, {'5', 5},  {'6', 6},  {'7', 7}, {'8', 8},
    {'9', 9}, {'T', 10}, {'J', 1}, {'Q', 12}, {'K', 13}, {'A', 15}};

bool tiebreaker_2(hand h1, hand h2) {
    if (h1.at(0) != h2.at(0)) {
        return card_strengths_2.find(h1.at(0))->second <
               card_strengths_2.find(h2.at(0))->second;
    } else if (h1.at(1) != h2.at(1)) {
        return card_strengths_2.find(h1.at(1))->second <
               card_strengths_2.find(h2.at(1))->second;
    } else if (h1.at(2) != h2.at(2)) {
        return card_strengths_2.find(h1.at(2))->second <
               card_strengths_2.find(h2.at(2))->second;
    } else if (h1.at(3) != h2.at(3)) {
        return card_strengths_2.find(h1.at(3))->second <
               card_strengths_2.find(h2.at(3))->second;
    } else if (h1.at(4) != h2.at(4)) {
        return card_strengths_2.find(h1.at(4))->second <
               card_strengths_2.find(h2.at(4))->second;
    }
    return false;
}

size_t part1(problem p) {
    std::sort(p.begin(), p.end(),
              [](std::pair<hand, size_t> p1, std::pair<hand, size_t> p2) {
                  hand hand1 = p1.first;
                  hand hand2 = p2.first;
                  size_t t1 = get_type(hand1);
                  size_t t2 = get_type(hand2);
                  if (t1 != t2) {
                      return t1 < t2;
                  } else {
                      return tiebreaker_1(hand1, hand2);
                  }
              });
    size_t sum{0};
    for (size_t rank{1}; rank <= p.size(); ++rank) {
        sum += rank * p.at(rank - 1).second;
    }
    return sum;
}

size_t get_type_joker(std::string_view hand) {
    std::vector<size_t> counts{
        count_card_2(hand, hand.at(0)), count_card_2(hand, hand.at(1)),
        count_card_2(hand, hand.at(2)), count_card_2(hand, hand.at(3)),
        count_card_2(hand, hand.at(4))};
    size_t jokers = count_card_1(hand, 'J');
    if (is_five_of_kind(counts, jokers)) {
        return 6;
    } else if (is_four_of_kind(counts, jokers)) {
        return 5;
    } else if (is_full_house(counts, jokers)) {
        return 4;
    } else if (is_three_of_kind(counts, jokers)) {
        return 3;
    } else if (is_two_pair(counts)) {
        return 2;
    } else if (is_one_pair(counts, jokers)) {
        return 1;
    } else {
        return 0;
    }
}

size_t part2(problem p) {
    std::sort(p.begin(), p.end(),
              [](std::pair<hand, size_t> p1, std::pair<hand, size_t> p2) {
                  hand hand1 = p1.first;
                  hand hand2 = p2.first;
                  size_t t1 = get_type_joker(hand1);
                  size_t t2 = get_type_joker(hand2);
                  if (t1 != t2) {
                      return t1 < t2;
                  } else {
                      return tiebreaker_2(hand1, hand2);
                  }
              });
    size_t sum{0};
    for (size_t rank{1}; rank <= p.size(); ++rank) {
        sum += rank * p.at(rank - 1).second;
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

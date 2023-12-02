
#include <charconv>
#include <iostream>

#include "input.h"

struct draw {
    size_t r;
    size_t g;
    size_t b;
};

struct game {
    size_t id;
    std::vector<draw> draws;
};

std::vector<std::string_view> split(std::string_view str, char split_char) {
    std::vector<std::string_view> result{};
    for (size_t idx = str.find(split_char); idx != std::string_view::npos;
         idx = str.find(split_char)) {
        result.push_back(str.substr(0, idx));
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

draw parse_box(std::string_view box) {
    size_t amount = extract_number(box);
    if (box.find("red") != std::string_view::npos) {
        return {.r = amount, .g = 0, .b = 0};
    } else if (box.find("green") != std::string_view::npos) {
        return {.r = 0, .g = amount, .b = 0};
    } else if (box.find("blue") != std::string_view::npos) {
        return {.r = 0, .g = 0, .b = amount};
    }
    return {};
}

draw parse_draw(std::string_view draw_str) {
    draw current{.r = 0, .g = 0, .b = 0};
    auto box_split = split(draw_str, ',');
    for (std::string_view box : box_split) {
        auto result = parse_box(box);
        current.r += result.r;
        current.g += result.g;
        current.b += result.b;
    }
    return current;
}

std::vector<game> parse_games(const std::vector<std::string_view> &input) {
    std::vector<game> result{};
    for (std::string_view str : input) {
        auto game_split = split(str, ':');

        std::string_view game = game_split.at(0);
        // Remove Game
        game.remove_prefix(5);
        size_t game_id;
        std::from_chars(game.data(), game.data() + game.length(), game_id);

        std::string_view draws = game_split.at(1);
        auto draws_split = split(draws, ';');

        std::vector<draw> draws_struct;
        for (std::string_view draw : draws_split) {
            draws_struct.push_back(parse_draw(draw));
        }
        result.push_back({
            .id = game_id,
            .draws = draws_struct,
        });
    }
    return result;
}

const size_t MAX_RED{12};
const size_t MAX_GREEN{13};
const size_t MAX_BLUE{14};

size_t part1(const std::vector<game> &input) {
    size_t sum{0};
    for (game g : input) {
        bool impossible{false};
        for (draw d : g.draws) {
            if (d.r > MAX_RED || d.g > MAX_GREEN || d.b > MAX_BLUE) {
                impossible = true;
                break;
            }
        }
        if (not impossible) {
            sum += g.id;
        }
    }
    return sum;
}

size_t part2(const std::vector<game> &input) {
    size_t sum{0};
    for (game g : input) {
        size_t red{0};
        size_t green{0};
        size_t blue{0};
        for (draw d : g.draws) {
            red = std::max(red, d.r);
            green = std::max(green, d.g);
            blue = std::max(blue, d.b);
        }
        sum += red * green * blue;
    }
    return sum;
}

int main() {
    auto games = parse_games(input);
    std::cout << "Part 1: " << part1(games) << "\n";
    std::cout << "Part 2: " << part2(games) << "\n";
}


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

draw parse_box(std::string_view box) {
    size_t amount_size = box.find(' ');
    size_t amount;
    std::from_chars(box.data(), box.data() + amount_size, amount);

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
    for (size_t comma = draw_str.find(','); comma != std::string_view::npos;
         comma = draw_str.find(',')) {
        auto box = draw_str.substr(0, comma);
        auto result = parse_box(box);
        current.r += result.r;
        current.g += result.g;
        current.b += result.b;
        draw_str.remove_prefix(comma + 2);
    }
    auto result = parse_box(draw_str);
    current.r += result.r;
    current.g += result.g;
    current.b += result.b;

    return current;
}

std::vector<game> parse_games(const std::vector<std::string_view> &input) {
    std::vector<game> result{};
    for (std::string_view str : input) {
        // Remove Game
        str.remove_prefix(5);
        size_t game_id;
        size_t game_id_length = str.find(':');
        std::from_chars(str.data(), str.data() + game_id_length, game_id);
        str.remove_prefix(game_id_length + 2);

        std::vector<std::string_view> draws;
        for (size_t semicolon = str.find(';');
             semicolon != std::string_view::npos; semicolon = str.find(';')) {
            draws.push_back(str.substr(0, semicolon));
            str.remove_prefix(semicolon + 2);
        }
        draws.push_back(str);

        std::vector<draw> draws_struct;
        for (std::string_view draw : draws) {
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

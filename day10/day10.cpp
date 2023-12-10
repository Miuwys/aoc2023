
#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include "input.h"

struct pipe {
    size_t x;
    size_t y;
    char type;
};

enum direction { NORTH, EAST, SOUTH, WEST };

using problem = std::map<std::pair<size_t, size_t>, char>;

pipe get_pipe_at(size_t x, size_t y) {
    try {
        return {x, y, input.at(y).at(x)};
    } catch (const std::out_of_range &e) {
        return {x, y, '.'};
    }
}

bool has_connection(pipe p, direction d) {
    // Check where start is connected
    if (p.type == 'S') {
        if (d == NORTH) {
            return has_connection(get_pipe_at(p.x, p.y - 1), SOUTH);
        } else if (d == EAST) {
            return has_connection(get_pipe_at(p.x + 1, p.y), WEST);
        } else if (d == SOUTH) {
            return has_connection(get_pipe_at(p.x, p.y + 1), NORTH);
        } else if (d == WEST) {
            return has_connection(get_pipe_at(p.x - 1, p.y), EAST);
        }
    } else if (p.type == '|') {
        return d == SOUTH || d == NORTH;
    } else if (p.type == '-') {
        return d == EAST || d == WEST;
    } else if (p.type == 'L') {
        return d == NORTH || d == EAST;
    } else if (p.type == 'J') {
        return d == NORTH || d == WEST;
    } else if (p.type == '7') {
        return d == SOUTH || d == WEST;
    } else if (p.type == 'F') {
        return d == SOUTH || d == EAST;
    } else {
        return false;
    }
    throw std::logic_error(std::string{"Unknown type: "} + p.type);
}

problem parse_input(const std::vector<std::string_view> &input) {
    problem result{};
    pipe start;
    pipe next;
    for (auto line = input.begin(); line != input.end(); ++line) {
        if (line->find('S') != std::string_view::npos) {
            start = {line->find('S'),
                     static_cast<size_t>(std::distance(input.begin(), line)),
                     'S'};
            if (has_connection(start, NORTH) && has_connection(start, SOUTH)) {
                start.type = '|';
                next = get_pipe_at(start.x, start.y + 1);
            } else if (has_connection(start, EAST) &&
                       has_connection(start, WEST)) {
                start.type = '-';
                next = get_pipe_at(start.x + 1, start.y);
            } else if (has_connection(start, NORTH) &&
                       has_connection(start, EAST)) {
                next = get_pipe_at(start.x, start.y - 1);
            } else if (has_connection(start, NORTH) &&
                       has_connection(start, WEST)) {
                start.type = 'J';
                next = get_pipe_at(start.x, start.y - 1);
            } else if (has_connection(start, SOUTH) &&
                       has_connection(start, WEST)) {
                start.type = '7';
                next = get_pipe_at(start.x, start.y + 1);
            } else if (has_connection(start, SOUTH) &&
                       has_connection(start, EAST)) {
                start.type = 'F';
                next = get_pipe_at(start.x, start.y + 1);
            } else {
                throw std::logic_error("Unknown start type");
            }
            result.insert({{start.x, start.y}, start.type});
            break;
        }
    }
    pipe previous = start;

    while (next.x != start.x || next.y != start.y) {
        result.insert({{next.x, next.y}, next.type});
        if (has_connection(next, NORTH)) {
            pipe p = get_pipe_at(next.x, next.y - 1);
            if (p.x != previous.x || p.y != previous.y) {
                previous = next;
                next = p;
                continue;
            }
        }
        if (has_connection(next, EAST)) {
            pipe p = get_pipe_at(next.x + 1, next.y);
            if (p.x != previous.x || p.y != previous.y) {
                previous = next;
                next = p;
                continue;
            }
        }
        if (has_connection(next, SOUTH)) {
            pipe p = get_pipe_at(next.x, next.y + 1);
            if (p.x != previous.x || p.y != previous.y) {
                previous = next;
                next = p;
                continue;
            }
        }
        if (has_connection(next, WEST)) {
            pipe p = get_pipe_at(next.x - 1, next.y);
            if (p.x != previous.x || p.y != previous.y) {
                previous = next;
                next = p;
                continue;
            }
        }
        throw std::logic_error("No connected pipe found!");
    }
    return result;
}

size_t part1(problem p) { return p.size() / 2; }

size_t part2(problem p) {
    auto max_x = std::max_element(p.begin(), p.end(), [](auto p1, auto p2) {
                     return p1.first.first < p2.first.first;
                 })->first.first;
    auto max_y = std::max_element(p.begin(), p.end(), [](auto p1, auto p2) {
                     return p1.first.second < p2.first.second;
                 })->first.second;
    for (size_t y{0}; y <= max_y; ++y) {
        bool inside = false;
        direction d = WEST;
        for (size_t x{0}; x <= max_x; ++x) {
            auto current_it = p.find({x, y});
            if (current_it == p.end()) {
                if (inside) {
                    p.insert({{x, y}, 'I'});
                } else {
                    p.insert({{x, y}, 'O'});
                }
            } else {
                if (current_it->second == '|') {
                    inside = !inside;
                } else if (current_it->second == 'L') {
                    // Indicates pipe comes from north
                    d = NORTH;
                } else if (current_it->second == 'J') {
                    if (d != NORTH) {
                        inside = !inside;
                    }
                    // Reset direction
                    d = WEST;
                } else if (current_it->second == '7') {
                    if (d != SOUTH) {
                        inside = !inside;
                    }
                    // Reset direction
                    d = WEST;
                } else if (current_it->second == 'F') {
                    d = SOUTH;
                }
            }
        }
    }
    return std::count_if(p.begin(), p.end(), [](const auto &kv) {
        return kv.second == 'I';
    });
}
    
int main() {
    const problem p = parse_input(input);
    size_t result_1{part1(p)};
    std::cout << "Part 1: " << result_1 << "\n";
    size_t result_2{part2(p)};
    std::cout << "Part 2: " << result_2 << "\n";
}

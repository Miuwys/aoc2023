
#include <algorithm>
#include <charconv>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>
#include <vector>

// Describes the connection of the edge
// north - northern angle
// south - southern angle
// horizontal - horizontal edge
// vertical - vertical edge
enum class Direction { horizontal, vertical, north, south };

using color = std::string;

struct Problem {
    // We only need to save the vertical lines to fully define the problem
    // This data structure is not the most efficient but it gets the job done
    std::map<long long, std::map<long long, Direction>> edges;
    long long x{0};
    long long y{0};
    void insert_instruction(char dir, size_t dist) {
        int dy{0};
        Direction start_d, end_d;
        switch (dir) {
            case 'U':
            case '3':
                start_d = Direction::north;
                end_d = Direction::south;
                dy = -1;
                break;
            case 'D':
            case '1':
                start_d = Direction::south;
                end_d = Direction::north;
                dy = 1;
                break;
            case 'L':
            case '2':
                x -= dist;
                return;
            case 'R':
            case '0':
                x += dist;
                return;
        }
        edges[y][x] = start_d;
        y += dy;
        for (size_t i{1}; i < dist; ++i, y += dy) {
            edges[y][x] = Direction::vertical;
        }
        edges[y][x] = end_d;
    }

    size_t volume() const {
        size_t result{0};
        for (auto y : edges) {
            bool inside = false;
            long long start_x;
            Direction prev_d = Direction::horizontal;
            for (auto [x, dir] : y.second) {
                if (dir == Direction::vertical) {
                    if (inside) {
                        result += (x - start_x) + 1;
                    } else {
                        start_x = x;
                    }
                    inside = !inside;
                } else if (dir == Direction::north) {
                    if (prev_d == Direction::horizontal) {
                        prev_d = Direction::north;
                        if (!inside) {
                            start_x = x;
                        }
                    } else if (prev_d == Direction::north) {
                        prev_d = Direction::horizontal;
                        if (!inside) {
                            result += (x - start_x) + 1;
                        }
                    } else if (prev_d == Direction::south) {
                        inside = !inside;
                        prev_d = Direction::horizontal;
                        if (!inside) {
                            result += (x - start_x) + 1;
                        }
                    }
                } else if (dir == Direction::south) {
                    if (prev_d == Direction::horizontal) {
                        prev_d = Direction::south;
                        if (!inside) {
                            start_x = x;
                        }
                    } else if (prev_d == Direction::south) {
                        prev_d = Direction::horizontal;
                        if (!inside) {
                            result += (x - start_x) + 1;
                        }
                    } else if (prev_d == Direction::north) {
                        inside = !inside;
                        prev_d = Direction::horizontal;
                        if (!inside) {
                            result += (x - start_x) + 1;
                        }
                    }
                }
            }
        }
        return result;
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

Problem parse_input_1(const std::string &path) {
    Problem result{};
    std::ifstream input{path};
    for (std::string line; getline(input, line);) {
        std::vector<std::string_view> s = split(line, ' ');
        char dir = s.at(0).at(0);
        std::size_t dist = extract_number(s.at(1));
        result.insert_instruction(dir, dist);
    }
    return result;
}

Problem parse_input_2(const std::string &path) {
    Problem result{};
    std::ifstream input{path};
    for (std::string line; getline(input, line);) {
        std::vector<std::string_view> s = split(line, ' ');
        std::string color{s.at(2)};
        char dir = color.at(7);
        size_t dist;
        std::from_chars(color.data() + 2, color.data() + 7, dist, 16);
        result.insert_instruction(dir, dist);
    }
    return result;
}

int main() {
    std::string path{"/home/chris/aoc2023/day18/input"};
    const Problem p1{parse_input_1(path)};
    size_t result_1{p1.volume()};
    std::cout << "Part 1: " << result_1 << "\n";
    const Problem p2{parse_input_2(path)};
    size_t result_2{p2.volume()};
    std::cout << "Part 2: " << result_2 << "\n";
}

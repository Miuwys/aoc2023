#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

using problem = std::vector<std::vector<size_t>>;

enum class Direction { north, east, south, west, start };

struct Position {
    int x;
    int y;
    Direction direction;
    size_t steps_in_direction;
};
bool operator==(const Position &lhs, const Position &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.direction == rhs.direction &&
           lhs.steps_in_direction == rhs.steps_in_direction;
}

template <>
struct std::hash<Position> {
    std::size_t operator()(const Position &pos) const {
        using std::hash;
        using std::size_t;
        size_t direction_hash = hash<int>{}(
            static_cast<underlying_type<Direction>::type>(pos.direction));
        return (hash<int>{}(pos.x) ^ hash<int>{}(pos.y) ^
                hash<size_t>{}(pos.steps_in_direction)) ^
               direction_hash;
    }
};

problem parse_input() {
    problem result{};
    std::ifstream input{"/home/chris/aoc2023/day17/input"};
    for (std::string line; getline(input, line);) {
        std::vector<size_t> values{};
        for (char c : line) {
            values.push_back(static_cast<size_t>(c - '0'));
        }
        result.push_back(values);
    }
    return result;
}

size_t h(const problem &p, Position pos) {
    Position end{static_cast<int>(p.at(0).size() - 1),
                 static_cast<int>(p.size() - 1), Direction::east, 0};
    return std::abs(end.x - pos.x) + std::abs(end.y - pos.y);
}

bool is_goal(const problem &p, Position pos) { return h(p, pos) == 0; }

Position go_direction(Position current, Direction d) {
    switch (d) {
        case Direction::north:
            return {
                current.x, current.y - 1, d,
                current.direction == d ? current.steps_in_direction + 1 : 1};
        case Direction::east:
            return {
                current.x + 1, current.y, d,
                current.direction == d ? current.steps_in_direction + 1 : 1};
        case Direction::south:
            return {
                current.x, current.y + 1, d,
                current.direction == d ? current.steps_in_direction + 1 : 1};
        case Direction::west:
            return {
                current.x - 1, current.y, d,
                current.direction == d ? current.steps_in_direction + 1 : 1};
        case Direction::start:
            return {-1, -1, d, 0};
    }
    throw std::logic_error("unknown direction");
}
Direction turn_left(Direction d) {
    switch (d) {
        case Direction::north:
            return Direction::west;
        case Direction::east:
            return Direction::north;
        case Direction::south:
            return Direction::east;
        case Direction::west:
            return Direction::south;
        case Direction::start:
            return Direction::south;
    }
    throw std::logic_error("unknown direction");
}
Direction turn_right(Direction d) {
    switch (d) {
        case Direction::north:
            return Direction::east;
        case Direction::east:
            return Direction::south;
        case Direction::south:
            return Direction::west;
        case Direction::west:
            return Direction::north;
        case Direction::start:
            return Direction::east;
    }
    throw std::logic_error("unknown direction");
}

template <typename NeighboursFunction>
size_t a_star(problem p, NeighboursFunction &&neighbours) {
    Position start{0, 0, Direction::start, 0};
    std::unordered_map<Position, size_t> g_score{{start, 0}};
    std::unordered_map<Position, size_t> f_score{{start, h(p, start)}};
    auto pos_comp = [&f_score](const Position &pos1,
                               const Position &pos2) -> bool {
        return f_score[pos1] > f_score[pos2];
    };
    std::priority_queue<Position, std::vector<Position>, decltype(pos_comp)>
        open_set{pos_comp};
    open_set.push(start);
    while (!open_set.empty()) {
        Position current = open_set.top();
        if (is_goal(p, current)) {
            Position goal{current};
            return f_score[goal];
        }
        open_set.pop();
        for (Position neighbour : neighbours(current)) {
            size_t cost = p.at(neighbour.y).at(neighbour.x);
            size_t total_path_cost = g_score[current] + cost;
            // Insert new cost if its beneficial
            if (g_score.count(neighbour) == 0) {
                g_score[neighbour] = std::numeric_limits<size_t>::max();
            }
            if (total_path_cost < g_score[neighbour]) {
                g_score[neighbour] = total_path_cost;
                f_score[neighbour] = total_path_cost + h(p, neighbour);
                open_set.push(neighbour);
            }
        }
    }
    return std::numeric_limits<size_t>::max();
}

size_t part1(problem p) {
    auto neighboursFn = [&p](Position current) {
        std::vector<Position> possible_neighbours{
            go_direction(current, current.direction),
            go_direction(current, turn_left(current.direction)),
            go_direction(current, turn_right(current.direction))};
        std::vector<Position> neighbours;
        for (Position neighbour : possible_neighbours) {
            if (neighbour.x < 0 || neighbour.x >= static_cast<int>(p[0].size()))
                continue;
            if (neighbour.y < 0 || neighbour.y >= static_cast<int>(p.size()))
                continue;
            if (neighbour.steps_in_direction > 3) continue;
            neighbours.emplace_back(std::move(neighbour));
        }
        return neighbours;
    };
    return a_star(p, neighboursFn);
}

bool is_valid_ultra(const problem &p, Position pos) {
    return pos.x >= 0 && pos.x < static_cast<int>(p[0].size()) && pos.y >= 0 &&
           pos.y < static_cast<int>(p.size()) && pos.steps_in_direction <= 10;
}

size_t part2(problem p) {
    auto neighboursFn = [&p](Position current) {
        std::vector<Position> possible_neighbours{
            go_direction(current, current.direction)};
        if (current.steps_in_direction >= 4 || current.direction == Direction::start) {
            possible_neighbours.emplace_back(go_direction(current, turn_left(current.direction)));
            possible_neighbours.emplace_back(go_direction(current, turn_right(current.direction)));
        }
        std::vector<Position> neighbours;
        for (Position neighbour : possible_neighbours) {
            if (neighbour.x < 0 || neighbour.x >= static_cast<int>(p[0].size()))
                continue;
            if (neighbour.y < 0 || neighbour.y >= static_cast<int>(p.size()))
                continue;
            if (neighbour.steps_in_direction > 10) continue;
            neighbours.emplace_back(std::move(neighbour));
        }
        return neighbours;
    };
    return a_star(p, neighboursFn);
}

int main() {
    const problem p = parse_input();
    size_t result_1{part1(p)};
    std::cout << "Part 1: " << result_1 << "\n";
    size_t result_2{part2(p)};
    std::cout << "Part 2: " << result_2 << "\n";
}

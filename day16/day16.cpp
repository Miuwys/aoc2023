
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

// runs a bit slow, could be optimized by memorization of paths

using problem = std::vector<std::string>;

enum class Turn { left, right, straight, back };
struct lightbeam {
    long long x;
    long long y;
    int d;
};

lightbeam go_forward(lightbeam l) {
    if (l.d == 0)
        return {l.x, l.y - 1, l.d};
    else if (l.d == 2)
        return {l.x, l.y + 1, l.d};
    else if (l.d == 1)
        return {l.x + 1, l.y, l.d};
    else if (l.d == 3)
        return {l.x - 1, l.y, l.d};
    else
        throw std::logic_error("unknown direction");
}

lightbeam make_turn(lightbeam l, Turn t) {
    if (t == Turn::straight)
        return go_forward(l);
    else if (t == Turn::back)
        return go_forward({l.x, l.y, (l.d + 2) % 4});
    else if (t == Turn::left)
        return go_forward({l.x, l.y, (l.d + 3) % 4});
    else if (t == Turn::right)
        return go_forward({l.x, l.y, (l.d + 1) % 4});
    else
        throw std::logic_error("unknown turn");
}

void dfs(problem p, lightbeam light,
         std::map<std::pair<long long, long long>, std::set<int>>& reached) {
    if (light.y < 0 || light.y >= static_cast<long long>(p.size())) return;
    if (light.x < 0 || light.x >= static_cast<long long>(p.at(light.y).size()))
        return;
    if (reached[{light.x, light.y}].count(light.d) >= 1) return;
    reached[{light.x, light.y}].insert(light.d);
    char field = p.at(light.y).at(light.x);
    switch (field) {
        case '.':
            return dfs(p, make_turn(light, Turn::straight), reached);
            break;
        case '/':
            if (light.d == 0 || light.d == 2)
                return dfs(p, make_turn(light, Turn::right), reached);
            else if (light.d == 1 || light.d == 3)
                return dfs(p, make_turn(light, Turn::left), reached);
            else
                throw std::logic_error("unknown direction");
            break;
        case '\\':
            if (light.d == 0 || light.d == 2)
                return dfs(p, make_turn(light, Turn::left), reached);
            else if (light.d == 1 || light.d == 3)
                return dfs(p, make_turn(light, Turn::right), reached);
            else
                throw std::logic_error("unknown direction");
        case '|':
            if (light.d == 0 || light.d == 2)
                return dfs(p, make_turn(light, Turn::straight), reached);
            else if (light.d == 1 || light.d == 3) {
                dfs(p, make_turn(light, Turn::right), reached);
                return dfs(p, make_turn(light, Turn::left), reached);
            } else
                throw std::logic_error("unknown direction");
        case '-':
            if (light.d == 0 || light.d == 2) {
                dfs(p, make_turn(light, Turn::right), reached);
                return dfs(p, make_turn(light, Turn::left), reached);
            } else if (light.d == 1 || light.d == 3)
                return dfs(p, make_turn(light, Turn::straight), reached);
            else
                throw std::logic_error("unknown direction");
    };
}

problem parse_input() {
    problem result{};
    std::ifstream input{"/home/chris/aoc2023/day16/input"};
    for (std::string line; getline(input, line);) {
        result.emplace_back(std::move(line));
    }
    return result;
}

size_t part1(problem p) {
    std::map<std::pair<long long, long long>, std::set<int>> reached_set{};
    dfs(p, lightbeam{0, 0, 1}, reached_set);
    std::set<std::pair<size_t, size_t>> reached_coordinates{};
    for (auto kv : reached_set) {
        reached_coordinates.insert({kv.first.first, kv.first.second});
    }
    return reached_coordinates.size();
}

size_t part2(problem p) {
    std::size_t max{0};
    // Heading right
    for (size_t i{0}; i < p.size(); ++i) {
        std::map<std::pair<long long, long long>, std::set<int>> reached_set{};
        dfs(p, lightbeam{0, static_cast<long long>(i), 1}, reached_set);
        std::set<std::pair<size_t, size_t>> reached_coordinates{};
        for (auto kv : reached_set) {
            reached_coordinates.insert({kv.first.first, kv.first.second});
        }
        max = std::max(max, reached_coordinates.size());
    }
    // Heading left
    for (size_t i{0}; i < p.size(); ++i) {
        std::map<std::pair<long long, long long>, std::set<int>> reached_set{};
        dfs(p, lightbeam{static_cast<long long>(p.at(i).size() - 1), static_cast<long long>(i), 3}, reached_set);
        std::set<std::pair<size_t, size_t>> reached_coordinates{};
        for (auto kv : reached_set) {
            reached_coordinates.insert({kv.first.first, kv.first.second});
        }
        max = std::max(max, reached_coordinates.size());
    }
    // Heading up
    for (size_t i{0}; i < p.at(0).size(); ++i) {
        std::map<std::pair<long long, long long>, std::set<int>> reached_set{};
        dfs(p, lightbeam{static_cast<long long>(i), static_cast<long long>(p.size() - 1), 0}, reached_set);
        std::set<std::pair<size_t, size_t>> reached_coordinates{};
        for (auto kv : reached_set) {
            reached_coordinates.insert({kv.first.first, kv.first.second});
        }
        max = std::max(max, reached_coordinates.size());
    }
    // Heading down
    for (size_t i{0}; i < p.at(0).size(); ++i) {
        std::map<std::pair<long long, long long>, std::set<int>> reached_set{};
        dfs(p, lightbeam{static_cast<long long>(i), 0, 2}, reached_set);
        std::set<std::pair<size_t, size_t>> reached_coordinates{};
        for (auto kv : reached_set) {
            reached_coordinates.insert({kv.first.first, kv.first.second});
        }
        max = std::max(max, reached_coordinates.size());
    }
    return max;
}

int main() {
    const problem p = parse_input();
    size_t result_1{part1(p)};
    std::cout << "Part 1: " << result_1 << "\n";
    size_t result_2{part2(p)};
    std::cout << "Part 2: " << result_2 << "\n";
}

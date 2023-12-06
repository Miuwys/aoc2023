
#include <iostream>
#include <vector>
struct problem {
    std::vector<size_t> times;
    std::vector<size_t> distances;
};

size_t part1(const problem p) {
    size_t product{1};
    for (size_t i{0}; i < p.times.size(); ++i) {
        size_t t{p.times.at(i)};
        size_t d{p.distances.at(i)};
        size_t solutions{0};
        for (size_t x{0}; x <= t; ++x) {
            if (x * (t - x) > d) {
                ++solutions;
            }
        }
        product *= solutions;
    }
    return product;
}

size_t part2(const problem p) {
    size_t t{p.times.at(0)};
    size_t d{p.distances.at(0)};
    size_t solutions{0};
    for (size_t x{0}; x <= t; ++x) {
        if (x * (t - x) > d) {
            ++solutions;
        }
    }
    return solutions;
}

int main() {
    const problem p1 = {
        {56, 97, 78, 75},
        {546, 1927, 1131, 1139},
    };
    const problem p2 = {
        {56977875},
        {546192711311139},
    };
    size_t result_1{part1(p1)};
    std::cout << "Part 1: " << result_1 << "\n";
    size_t result_2{part2(p2)};
    std::cout << "Part 2: " << result_2 << "\n";
}

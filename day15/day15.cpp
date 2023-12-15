
#include <algorithm>
#include <charconv>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <stdexcept>

#include "input.h"

using problem = std::vector<std::string_view>;

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

unsigned int extract_number(std::string_view str) {
    size_t start = str.find_first_of("-1234567890");
    size_t stop = str.find_last_of("1234567890") + 1;
    unsigned int n;
    std::from_chars(str.data() + start, str.data() + stop, n);
    return n;
}

class HASHMAP {
   public:
    HASHMAP() : _hm(256) {}
    static size_t HASH(std::string_view str) {
        size_t current_value{0};
        for (char c : str) {
            current_value += static_cast<size_t>(c);
            current_value *= 17;
            current_value %= 256;
        }
        return current_value;
    }
    void insert(std::string_view key, unsigned int value) {
        auto &box = _hm.at(HASH(key));
        auto old_value = std::find_if(
            box.begin(), box.end(), [&](auto &kv) { return kv.first == key; });
        if (old_value != box.end()) {
            // Replace value
            old_value->second = value;
        } else {
            // Insert at back
            box.push_back({key, value});
        }
    }
    void erase(std::string_view key) {
        auto &box = _hm.at(HASH(key));
        auto old_value = std::find_if(
            box.begin(), box.end(), [&](auto &kv) { return kv.first == key; });
        if (old_value != box.end()) {
            box.erase(old_value);
        }
    }
    size_t focusing_power() {
        size_t sum{0};
        for (size_t i{0}; i < _hm.size(); ++i) {
            size_t box_number{i + 1};
            auto &box = _hm.at(i);
            for (auto box_it{box.begin()}; box_it != box.end(); ++box_it) {
                size_t box_slot = std::distance(box.begin(), box_it) + 1;
                auto value = box_it->second;
                sum += box_number * box_slot * value;
            }
        }
        return sum;
    }

   private:
    std::vector<std::list<std::pair<std::string_view, unsigned int>>> _hm;
};

problem parse_input(const std::vector<std::string_view> &input) {
    problem result{};
    for (auto s : split(input.at(0), ',')) {
        result.push_back(s);
    }
    return result;
}

size_t part1(problem p) {
    size_t sum{0};
    for (auto str : p) {
        sum += HASHMAP::HASH(str);
    }
    return sum;
}

size_t part2(problem p) {
    HASHMAP hashmap{};
    for (auto op : p) {
        if (op.find('=') != op.npos) {
            auto kv = split(op, '=');
            std::string_view key = kv.at(0);
            unsigned int value = extract_number(kv.at(1));
            hashmap.insert(key, value);
        } else {
            auto kv = split(op, '-');
            std::string_view key = kv.at(0);
            hashmap.erase(key);
        }
    }
    return hashmap.focusing_power();
}

int main() {
    const problem p = parse_input(input);
    size_t result_1{part1(p)};
    std::cout << "Part 1: " << result_1 << "\n";
    size_t result_2{part2(p)};
    std::cout << "Part 2: " << result_2 << "\n";
}

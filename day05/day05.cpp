#include <algorithm>
#include <charconv>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>

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

struct map {
    size_t destination_range;
    size_t source_range;
    size_t range_size;
};

struct problem {
    std::vector<size_t> seeds;
    std::vector<std::vector<map>> maps;
};

struct range {
    size_t start;
    size_t size;
};

bool is_in_range(size_t value, map m) {
    return value >= m.source_range && value < m.source_range + m.range_size;
}

size_t get_mapped_value(size_t value, map m) {
    return m.destination_range + value - m.source_range;
}

bool range_is_contained_in_map(range r, map m) {
    return r.start >= m.source_range &&
           r.start + r.size <= m.source_range + m.range_size;
}

bool map_is_contained_in_range(range r, map m) {
    return r.start < m.source_range &&
           r.start + r.size > m.source_range + m.range_size;
}

bool range_is_partially_contained_in_map(range r, map m) {
    return r.start + r.size > m.source_range &&
           r.start < m.source_range + m.range_size - 1;
}

std::vector<range> get_mapped_range(range r, std::vector<map> maps) {
    std::vector<range> unmapped{r};
    std::vector<range> mapped{};
    while (unmapped.size() > 0) {
        range current = unmapped.back();
        unmapped.pop_back();
        for (map m : maps) {
            if (range_is_contained_in_map(current, m)) {
                mapped.push_back({
                    get_mapped_value(current.start, m),
                    current.size,
                });
                current = {0, 0};
                break;
            }
            if (map_is_contained_in_range(current, m)) {
                size_t last_size{current.size};
                mapped.push_back({
                    get_mapped_value(m.source_range, m),
                    m.range_size,
                });
                last_size -= m.range_size;
                unmapped.push_back({
                    current.start,
                    m.source_range - current.start,
                });
                last_size -= (m.source_range - current.start);
                unmapped.push_back({
                    m.source_range + m.range_size,
                    last_size,
                });
                current = {0, 0};
                break;
            }
            if (range_is_partially_contained_in_map(current, m)) {
                size_t mapping_start = std::max(m.source_range, current.start);
                size_t mapping_end = std::min(m.source_range + m.range_size,
                                              current.start + current.size);
                size_t mapped_range_size = mapping_end - mapping_start;
                mapped.push_back({
                    get_mapped_value(mapping_start, m),
                    mapped_range_size,
                });
                if (mapping_start == current.start) {
                    // new range is at the end
                    unmapped.push_back({
                        mapping_end,
                        current.size - mapped_range_size,
                    });
                } else {
                    // new range is at the start
                    unmapped.push_back({
                        current.start,
                        current.size - mapped_range_size,
                    });
                }
                current = {0, 0};
                break;
            }
        }
        if (current.size != 0) {
            mapped.push_back(current);
        }
    }
    return mapped;
}
problem parse_input(const std::vector<std::string_view> &input) {
    std::vector<size_t> seeds{};
    auto seeds_string = input.at(0);
    auto tmp = split(seeds_string, ':');
    auto seeds_strings = split(tmp.at(1), ' ');
    for (auto s : seeds_strings) {
        seeds.push_back(extract_number(s));
    }

    std::vector<std::vector<map>> maps{{}, {}, {}, {}, {}, {}, {}};
    int current_map{-1};
    for (std::string_view line : input) {
        if (contains(line, "to")) {
            ++current_map;
            continue;
        } else if (current_map == -1) {
            // Skip to beginning of maps
            continue;
        } else if (line.length() == 0) {
            continue;
        }

        auto numbers = split(line, ' ');
        map m{
            extract_number(numbers.at(0)),
            extract_number(numbers.at(1)),
            extract_number(numbers.at(2)),
        };
        maps.at(current_map).push_back(m);
    }
    return {
        seeds,
        maps,
    };
}

size_t helper(size_t value, std::vector<map> maps) {
    for (map m : maps) {
        if (is_in_range(value, m)) {
            return get_mapped_value(value, m);
        }
    }
    return value;
}

size_t part1(const problem p) {
    std::vector<size_t> mapped_values = p.seeds;
    for (std::vector<map> current_maps : p.maps) {
        for (size_t i{0}; i < mapped_values.size(); ++i) {
            mapped_values.at(i) = helper(mapped_values.at(i), current_maps);
        }
    }
    return *std::min_element(mapped_values.begin(), mapped_values.end());
}

std::vector<range> process_range(range r, std::vector<std::vector<map>> maps) {
    std::vector<range> workset{r};
    for (std::vector<map> m : maps) {
        std::vector<range> new_workset{};
        for (range w : workset) {
            std::vector<range> new_ranges = get_mapped_range(w, m);
            new_workset.insert(new_workset.end(), new_ranges.begin(),
                               new_ranges.end());
        }
        workset = std::move(new_workset);
    }
    return workset;
}

size_t part2(const problem p) {
    std::vector<range> seed_ranges{};
    for (size_t i{0}; i < p.seeds.size(); i += 2) {
        seed_ranges.push_back({
            p.seeds.at(i),
            p.seeds.at(i + 1),
        });
    }

    std::vector<size_t> min_candidates{};
    for (range r : seed_ranges) {
        std::vector<range> result_ranges = process_range(r, p.maps);
        for (range rr : result_ranges) {
            min_candidates.push_back(rr.start);
        }
    }
    return *std::min_element(min_candidates.begin(), min_candidates.end());
}

int main() {
    const problem p = parse_input(input);
    size_t result_1{part1(p)};
    std::cout << "Part 1: " << result_1 << "\n";
    size_t result_2{part2(p)};
    std::cout << "Part 2: " << result_2 << "\n";
}

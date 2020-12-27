#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <sstream>
#include <list>

using Cups = std::list<int>;
using Locs = std::unordered_map<int, std::list<int>::iterator>;

void move(Cups& cups, Locs& locs, int& current_num) {
    auto current_it = locs[current_num];
    auto original_it = current_it;

    int n_moves = 3;
    std::vector<int> move_nums;
    for (int i = 0; i < n_moves; ++i) {
        current_it = std::next(current_it);
        if (current_it == cups.end()) {
            current_it = cups.begin();
        }
        move_nums.push_back(*current_it);
    }

    int dest_label = -1;
    int num_label = *original_it - 1;
    while (dest_label < 0) {
        if (num_label <= 0) num_label = cups.size();
        dest_label = *locs[num_label];
        for (int i = 0; i < n_moves; ++i) {
            if (dest_label == move_nums[i]) {
                --num_label;
                dest_label = -1;
            }
        }
    }

    auto dest_it = locs[dest_label];

    dest_it = std::next(dest_it);
    if (dest_it == cups.end()) dest_it = cups.begin();
    for (int i = 0; i < n_moves; ++i) {
        cups.insert(dest_it, move_nums[i]);
        cups.erase(locs[move_nums[i]]);
        auto prev = dest_it;
        if (prev == cups.begin()) {
            prev = std::prev(cups.end());
        } else {
            prev = std::prev(prev);
        }
        locs[move_nums[i]] = prev;
    }

    current_it = std::next(original_it);
    if (current_it == cups.end()) current_it = cups.begin();
    current_num = *current_it;
}

int64_t final_ans(const Cups& cups) {
    auto item = cups.begin();
    for (int i = 0; i < cups.size(); ++i) {
        if (*item == 1) {
            break;
        }
        item = std::next(item);
    }

    int64_t res = 0;
    for (int i = 1; i <= cups.size() - 1; ++i) {
        if (item == cups.end()) item = cups.begin();
        item = std::next(item);
        if (item == cups.end()) item = cups.begin();

        res += std::pow(10, cups.size() - i - 1) * *item;
    }
    return res;
}

Locs build_locs(Cups& cups) {
    Locs locs;
    auto item = cups.begin();
    for (int i = 0; i < cups.size(); ++i) {
        int num = *item;
        locs[num] = item;
        item = std::next(item);
    }
    return locs;
}

int64_t part_1(const Cups& cups_in) {
    auto cups = cups_in;
    const int n = cups.size();
    int current = cups.front();
    const int moves = 100;
    auto locs = build_locs(cups);
    for (int i = 0; i < moves; ++i) {
        move(cups, locs, current);
    }
    int64_t res = final_ans(cups);
    return res;
}

int64_t part_2(const Cups& cups_in) {
    auto cups = cups_in;
    for (int i = cups.size() + 1; i <= 1'000'000; ++i) {
        cups.push_back(i);
    }
    const int n = cups.size();
    int current = cups.front();
    const int moves = 10'000'000;
    auto locs = build_locs(cups);
    for (int i = 0; i < moves; ++i) {
        move(cups, locs, current);
    }
    int64_t res = 1;
    auto itr = std::next(locs[1]);
    res *= *itr;
    itr = std::next(itr);
    res *= *itr;
    return res;
}

int main() {
    std::ifstream in("../input/day_23.txt");
    Cups cups;
    std::string line;
    std::getline(in, line);
    for (const char& chr : line) {
        cups.push_back(std::stoi(std::string(1, chr)));
    }

    std::cout << part_1(cups) << std::endl;
    std::cout << part_2(cups) << std::endl;

    return 0;
}

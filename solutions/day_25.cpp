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

int64_t transform(int64_t last_value, int64_t subnum=7) {
    auto value = (last_value * subnum) % 20201227;
    return value;
}

int64_t find_loopsize(int64_t target_pubkey) {
    int64_t loopsize = -1;
    int64_t value = 1;
    int64_t i = 1;
    while (true) {
        value = transform(value);
        if (value == target_pubkey) {
            loopsize = i;
            break;
        }
        ++i;
    }
    return loopsize;
}

int64_t get_enckey(int64_t loopsize_a, int64_t pubkey_b) {
    int64_t value = 1;
    for (int64_t i = 0; i < loopsize_a; ++i) {
        value = transform(value, pubkey_b);
    }
    return value;
}

int64_t part_1(const std::vector<int64_t>& inp) {
    std::vector<int64_t> loop_sizes;
    for (const auto& pk : inp) {
        auto ls = find_loopsize(pk);
        loop_sizes.push_back(ls);
    }
    return get_enckey(loop_sizes[1], inp[0]);
}

int main() {
    std::ifstream in("../input/day_25.txt");
    std::vector<int64_t> inp;
    std::string line;
    while(std::getline(in, line)) {
        inp.push_back(std::stoi(line));
    }

    std::cout << part_1(inp) << std::endl;

    return 0;
}

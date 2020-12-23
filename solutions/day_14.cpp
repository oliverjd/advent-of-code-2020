#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <map>
#include <bitset>
#include <deque>

uint64_t do_mask_1(const std::string& mask, uint64_t num) {
    for (uint64_t i = 0; i < mask.size(); ++i) {
        auto chr = mask[mask.size() - i - 1];
        switch (chr) {
            case '0':
                num &= ~(1ull << i);
                break;
            case '1':
                num |= 1ull << i;
                break;
        }
    }
    return num;
}

uint64_t part_1(const std::vector<std::string>& instructions) {
    std::string mask;
    std::map<uint64_t, uint64_t> mem;

    for (const auto& line : instructions) {
        if (line.substr(0, 4) == "mask") {
            mask = line.substr(7);
        } else {
            std::string tok;
            std::stringstream ss(line);
            std::getline(ss, tok, ']');
            uint64_t i = std::stoll(tok.substr(4));
            std::getline(ss, tok, ']');
            uint64_t n = std::stoll(tok.substr(3));
            mem[i] = do_mask_1(mask, n);
        }
    }

    uint64_t tot = 0;
    for (const auto& [k, v] : mem) {
        tot += v;
    }
    return tot;
}

std::vector<uint64_t> do_mask_2(const std::string& mask, uint64_t num) {
    std::vector<uint64_t> all_addresses;
    std::string nums = std::bitset<64>(num).to_string();

    for (uint64_t i = 0; i < mask.size(); ++i) {
        auto chr = mask[mask.size() - i - 1];
        if (chr == '0') continue;
        nums[nums.size() - 1 - i] = chr;
    }

    std::deque<std::string> queue = {nums};
    while (true) {
        auto el = queue.front();
        bool got_x = false;
        for (int i = 0; i < el.size(); ++i) {
            auto chr = el[i];
            if (chr == 'X') {
                got_x = true;
                el[i] = '1';
                queue.push_back(el);
                el[i] = '0';
                queue.push_back(el);
                break;
            }
        }
        if (!got_x) {
            break;
        } else {
            queue.pop_front();
        }
    }

    for (const auto& s : queue) {
        all_addresses.push_back(std::stoll(s, nullptr, 2));
    }

    return all_addresses;
}

uint64_t part_2(const std::vector<std::string>& instructions) {
    std::string mask;
    std::map<uint64_t, uint64_t> mem;

    for (const auto& line : instructions) {
        if (line.substr(0, 4) == "mask") {
            mask = line.substr(7);
        } else {
            std::string tok;
            std::stringstream ss(line);
            std::getline(ss, tok, ']');
            uint64_t i = std::stoll(tok.substr(4));
            std::getline(ss, tok, ']');
            uint64_t n = std::stoll(tok.substr(3));
            const auto addresses = do_mask_2(mask, i);
            for(const auto& a : addresses) {
                mem[a] = n;
            }
        }
    }

    uint64_t tot = 0;
    for (const auto& [k, v] : mem) {
        tot += v;
    }
    return tot;
}

int main() {
    std::ifstream in("../input/day_14.txt");
    std::string line;

    std::vector<std::string> instructions;
    while (std::getline(in, line)) {
        instructions.push_back(line);
    }

    std::cout << part_1(instructions) << std::endl;
    std::cout << part_2(instructions) << std::endl;

    return 0;
}

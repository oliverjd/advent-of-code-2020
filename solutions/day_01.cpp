#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <algorithm>

int part_1(const std::vector<int>& vec) {
    constexpr int tgt = 2020;

    std::unordered_map<int, int> m;

    for (const auto& x : vec) {
        if (m.find(x) != m.end()) {
            return x * m[x];
        }

        m[tgt - x] = x;
    }

    throw std::runtime_error("Not found");
}

int part_2(const std::vector<int>& vec) {
    constexpr int tgt = 2020;

    std::unordered_map<int, int> m1;

    for (const auto& x : vec) {
        m1[tgt - x] = x;
    }

    for (const auto& [new_tgt, orig] : m1) {
        std::unordered_map<int, int> m2;
        for (const auto& x : vec) {
            if (m2.find(x) != m2.end()) {
                return x * m2[x] * orig;
            }

            m2[new_tgt - x] = x;
        }
    }

    throw std::runtime_error("Not found");
}

int main() {
    std::ifstream in("../input/day_01.txt");
    std::string buf;
    std::vector<int> vec;
    while (std::getline(in, buf)) {
        vec.push_back(std::stoi(buf));
    }

    std::cout << part_1(vec) << std::endl;
    std::cout << part_2(vec) << std::endl;

    return 0;
}

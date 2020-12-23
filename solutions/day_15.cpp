#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <map>

int find_num(const std::vector<int>& inp, int tgt) {
    std::unordered_map<int, int> last_seen;
    for (int i = 0; i < inp.size() - 1; ++i) {
        last_seen[inp[i]] = i;
    }

    auto nums = inp;
    while (nums.size() < tgt) {
        const auto n = nums.back();
        if (last_seen.find(n) == last_seen.end()) {
            nums.push_back(0);
        } else {
            nums.push_back(nums.size() - last_seen[n] - 1);
        }
        last_seen[n] = nums.size() - 2;
    }
    return nums.back();
}

int part_1(const std::vector<int>& inp) {
    return find_num(inp, 2020);
}

int part_2(const std::vector<int>& inp) {
    return find_num(inp, 30000000);
}

int main() {
    std::ifstream in("../input/day_15.txt");
    std::string line;

    std::vector<int> inp;
    while (std::getline(in, line, ',')) {
        inp.push_back(std::stoi(line));
    }

    std::cout << part_1(inp) << std::endl;
    std::cout << part_2(inp) << std::endl;

    return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <sstream>

int part_1(int wait, const std::vector<int>& nums) {
    int i = 0;
    while (true) {
        for (const auto& x : nums) {
            if (x < 0) continue;
            if ((wait + i) % x == 0) {
                return i * x;
            }
        }
        ++i;
    }
}

int64_t part_2(const std::vector<int>& nums) {
    int64_t t = 0;
    int64_t step = 1;
    for (int i = 0; i < nums.size(); ++i) {
        if (nums.at(i) < 0) continue;
        while ((t + i) % nums.at(i) != 0) {
            t += step;
        }
        step *= nums.at(i);
    }
    return t;
}

int main() {
    std::ifstream in("../input/day_13.txt");
    std::string line;

    std::getline(in, line);
    int wait = std::stoi(line);
    std::getline(in, line);
    std::string token;
    std::stringstream ss(line);
    std::vector<int> nums;
    while (std::getline(ss, token, ',')) {
        try {
            nums.push_back(std::stoi(token));
        } catch (std::invalid_argument) {
            nums.push_back(-1);
        }
    }

    std::cout << part_1(wait, nums) << std::endl;
    std::cout << part_2(nums) << std::endl;

    return 0;
}

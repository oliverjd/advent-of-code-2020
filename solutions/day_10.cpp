#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

int part_1(const std::vector<int>& inp) {
    auto sorted = inp;
    std::sort(sorted.begin(), sorted.end());
    const auto n = sorted.size();

    std::map<int, int> numdiffs = {{sorted[0], 1}, {3, 1}};

    for (int i = 1; i < n; ++i) {
        auto diff = sorted[i] - sorted[i - 1];
        if (numdiffs.find(diff) == numdiffs.end()) numdiffs[diff] = 0;
        ++numdiffs[diff];
    }
    return numdiffs[1] * numdiffs[3];
}

int64_t part_2(const std::vector<int>& inp) {
    auto sorted = inp;
    std::sort(sorted.begin(), sorted.end());
    const auto n = sorted.size();

    std::map<int64_t, int64_t> counts = {{0, 1}};

    for (int i = 0; i < n; ++i) {
        auto x = sorted[i];
        counts[x] = counts[x-1] + counts[x-2] + counts[x-3];
    }
    return counts[sorted[sorted.size()-1]];
}

int main() {
    std::ifstream in("../input/day_10.txt");
    std::string line;
    std::vector<int> inp;

    while (std::getline(in, line)) {
        inp.push_back(std::stoll(line));
    }

    std::cout << part_1(inp) << std::endl;
    std::cout << part_2(inp) << std::endl;

    return 0;
}

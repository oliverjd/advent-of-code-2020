#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <deque>

int64_t part_1(const std::vector<int64_t>& inp) {
    const int size = 25;
    std::deque<std::pair<int64_t, std::vector<int64_t>>> queue;

    for (const auto& x : inp) {
        if (queue.size() == size) {
            bool found = false;
            for (const auto& [num, sums] : queue) {
                for (const auto& s : sums) {
                    if (s == x) found = true;
                }
            }
            if (!found) return x;

            queue.pop_front();
        }

        queue.push_back({x, {}});
        for (auto& [num, sums] : queue) {
            sums.push_back(num + x);
        }
    }

    return 0;
}

int64_t part_2(const std::vector<int64_t>& inp) {
    const auto target = part_1(inp);

    // Grid of window sums, <start, end> of size n*n, initialize with zeros
    const auto n = inp.size();
    std::vector<std::vector<int64_t>> sum_grid(n, std::vector<int64_t>(n, 0));

    // Fill grid with window sums, i = start, j = end
    sum_grid[0][0] = inp[0];
    for (int64_t i = 1; i < n; ++i) {
        sum_grid[0][i] = sum_grid[0][i-1] + inp[i];
    }
    for (int64_t i = 1; i < n; ++i) {
        for(int64_t j = i; j < n; ++j) {
            sum_grid[i][j] = sum_grid[i-1][j-1] + inp[j] - inp[i-1];

            if (sum_grid[i][j] == target && j > i) {
                // Found our window i through j, return smallest + largest inside this window
                return *std::min_element(&inp[i], &inp[j]) + *std::max_element(&inp[i], &inp[j]);
            }
        }
    }

    throw std::runtime_error("Not found");
}

int main() {
    std::ifstream in("../input/day_09.txt");
    std::string line;
    std::vector<int64_t> inp;

    while (std::getline(in, line)) {
        inp.push_back(std::stoll(line));
    }

    std::cout << part_1(inp) << std::endl;
    std::cout << part_2(inp) << std::endl;

    return 0;
}

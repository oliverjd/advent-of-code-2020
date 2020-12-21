#include <iostream>
#include <fstream>
#include <vector>

using Grid = std::vector<std::vector<bool>>;

int64_t count_trees(const Grid& grid, int right, int down) {
    const int h = grid.size();
    const int w = grid[0].size();

    int x = 0;
    int y = 0;
    int64_t count = 0;

    while (y < h) {
        if (grid[y][x]) ++count;
        y += down;
        x += right;
        if (x >= w) x = x % w;
    }

    return count;
}

int64_t part_1(const Grid& grid) {
    return count_trees(grid, 3, 1);
}

int64_t part_2(const Grid& grid) {
    return count_trees(grid, 1, 1)
         * count_trees(grid, 3, 1)
         * count_trees(grid, 5, 1)
         * count_trees(grid, 7, 1)
         * count_trees(grid, 1, 2);
}

int main() {
    std::ifstream in("../input/day_03.txt");
    std::string buf;
    Grid grid;
    while (std::getline(in, buf)) {
        std::vector<bool> row;
        for (const auto & c : buf) {
            row.push_back(c == '#');
        }
        grid.push_back(row);
    }

    std::cout << part_1(grid) << std::endl;
    std::cout << part_2(grid) << std::endl;

    return 0;
}

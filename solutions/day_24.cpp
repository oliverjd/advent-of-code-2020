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

using Grid1 = std::vector<bool>;
using Grid2 = std::vector<Grid1>;
using Grid3 = std::vector<Grid2>;
using Coord = std::tuple<int, int, int>;

enum class Dir {
    E,
    SE,
    SW,
    W,
    NW,
    NE
};

Coord get_coord(const std::vector<Dir>& tile) {
        Coord coord = {0, 0, 0};
        for (const auto& dir : tile) {
            switch(dir) {
                case Dir::E:
                    ++std::get<0>(coord);
                    --std::get<1>(coord);
                    break;
                case Dir::SE:
                    --std::get<1>(coord);
                    ++std::get<2>(coord);
                    break;
                case Dir::SW:
                    --std::get<0>(coord);
                    ++std::get<2>(coord);
                    break;
                case Dir::W:
                    --std::get<0>(coord);
                    ++std::get<1>(coord);
                    break;
                case Dir::NW:
                    ++std::get<1>(coord);
                    --std::get<2>(coord);
                    break;
                case Dir::NE:
                    ++std::get<0>(coord);
                    --std::get<2>(coord);
                    break;
            }
        }
    return coord;
}

std::set<Coord> get_flipped(const std::vector<std::vector<Dir>>& inp) {
    std::set<Coord> flipped;

    for (const auto& tile : inp) {
        const auto coord = get_coord(tile);
        if (flipped.find(coord) != flipped.end()) {
            flipped.erase(coord);
        } else {
            flipped.insert(coord);
        }
    }
    return flipped;
}

int part_1(const std::vector<std::vector<Dir>>& inp) {
    auto flipped = get_flipped(inp);
    return flipped.size();
}

int count_neighbours(const Grid3& grid, int i, int j, int k) {
    std::set<Coord> neighbour_coords = {
        {i+1, j-1, k}, {i, j-1, k+1}, {i-1, j, k+1}, {i-1, j+1, k}, {i, j+1, k-1}, {i+1, j, k-1}
    };
    int tot = 0;
    for (const auto& [x, y, z] : neighbour_coords) {
        if (grid[x][y][z]) ++tot;
    }
    return tot;
}

void iterate(Grid3& grid) {
    Grid3 new_grid = grid;
    const int n = grid.size();

    for (int i = 1; i < n-1; ++i) {
        for (int j = 1; j < n-1; ++j) {
            for (int k = 1; k < n-1; ++k) {
                auto neighbours = count_neighbours(grid, i, j, k);
                if (grid[i][j][k] && (neighbours == 0 || neighbours > 2)) {
                    new_grid[i][j][k] = false;
                } else if (!grid[i][j][k] && neighbours == 2) {
                    new_grid[i][j][k] = true;
                }
            }
        }
    }
    grid = new_grid;
}

int count_tiles(Grid3& grid) {
    int tot = 0;
    const int n = grid.size();

    for (int i = 1; i < n-1; ++i) {
        for (int j = 1; j < n-1; ++j) {
            for (int k = 1; k < n-1; ++k) {
                if (grid[i][j][k]) ++tot;
            }
        }
    }

    return tot;
}

int part_2(const std::vector<std::vector<Dir>>& inp) {
    const int n = 150;
    const int o = n/2;
    Grid3 grid(n, Grid2(n, Grid1(n, false)));

    auto flipped = get_flipped(inp);
    for (const auto& [i, j, k]: flipped) {
        grid[i+o][j+o][k+o] = true;
    }

    for (int i = 1; i <= 100; ++i) {
        iterate(grid);
    }

    return count_tiles(grid);
}

int main() {
    std::ifstream in("../input/day_24.txt");
    std::vector<std::vector<Dir>> inp;
    std::string line;
    while(std::getline(in, line)) {
        std::vector<Dir> dirs;
        char prev_char = ' ';
        for (const auto& chr : line) {
            if (prev_char == 's' && chr == 'e') dirs.push_back(Dir::SE);
            if (prev_char == 'n' && chr == 'e') dirs.push_back(Dir::NE);
            if (prev_char != 's' && prev_char != 'n' && chr == 'e') dirs.push_back(Dir::E);
            if (prev_char == 's' && chr == 'w') dirs.push_back(Dir::SW);
            if (prev_char == 'n' && chr == 'w') dirs.push_back(Dir::NW);
            if (prev_char != 's' && prev_char != 'n' && chr == 'w') dirs.push_back(Dir::W);
            prev_char = chr;
        }
        inp.push_back(dirs);
    }

    std::cout << part_1(inp) << std::endl;
    std::cout << part_2(inp) << std::endl;

    return 0;
}

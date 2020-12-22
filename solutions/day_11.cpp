#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

enum class State {
    NON = 0,
    EMP,
    OCC,
};

using Grid = std::vector<std::vector<State>>;

void iterate_1(Grid& grid, bool& changes) {
    changes = false;
    auto new_grid = grid;

    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[i].size(); ++j) {
            int adj_occ = 0;

            if (j > 0 && grid[i][j-1] == State::OCC) ++adj_occ;
            if (i > 0 && grid[i-1][j] == State::OCC) ++adj_occ;
            if (i > 0 && j > 0 && grid[i-1][j-1] == State::OCC) ++adj_occ;
            if (i < grid.size()-1 && grid[i+1][j] == State::OCC) ++adj_occ;
            if (j < grid[i].size()-1 && grid[i][j+1] == State::OCC) ++adj_occ;
            if (i < grid.size()-1 && j < grid[i].size()-1 && grid[i+1][j+1] == State::OCC) ++adj_occ;
            if (i < grid.size()-1 && j > 0 && grid[i+1][j-1] == State::OCC) ++adj_occ;
            if (i > 0 && j < grid[i].size()-1 && grid[i-1][j+1] == State::OCC) ++adj_occ;

            if (grid[i][j] == State::EMP && adj_occ == 0) {
                new_grid[i][j] = State::OCC;
                changes = true;
            } else if (grid[i][j] == State::OCC && adj_occ >= 4) {
                new_grid[i][j] = State::EMP;
                changes = true;
            }
        }
    }

    grid = new_grid;
}

int part_1(const Grid& grid_in) {
    auto grid = grid_in;
    bool changes = true;
    while (changes) {
        iterate_1(grid, changes);
    }

    int count = 0;
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j] == State::OCC) ++count;
        }
    }
    return count;
}

void iterate_2(Grid& grid, bool& changes) {
    changes = false;
    auto new_grid = grid;

    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[i].size(); ++j) {
            int adj_occ = 0;

            std::vector<std::pair<int, int>> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};
            for (const auto& [di, dj] : dirs) {
                int ni = i + di;
                int nj = j + dj;
                while (ni >= 0 && ni < grid.size() && nj >= 0 && nj < grid[ni].size()) {
                    if (grid[ni][nj] == State::OCC) {
                        ++adj_occ;
                        break;
                    }
                    if (grid[ni][nj] == State::EMP) {
                        break;
                    }

                    ni += di;
                    nj += dj;
                }
            }

            if (grid[i][j] == State::EMP && adj_occ == 0) {
                new_grid[i][j] = State::OCC;
                changes = true;
            } else if (grid[i][j] == State::OCC && adj_occ >= 5) {
                new_grid[i][j] = State::EMP;
                changes = true;
            }
        }
    }

    grid = new_grid;
}

int part_2(const Grid& grid_in) {
    auto grid = grid_in;
    bool changes = true;
    while (changes) {
        iterate_2(grid, changes);
    }

    int count = 0;
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j] == State::OCC) ++count;
        }
    }
    return count;
}

int main() {
    std::ifstream in("../input/day_11.txt");
    std::string line;

    Grid grid;

    while (std::getline(in, line)) {
        std::vector<State> row;
        for (const auto& chr : line) {
            State s;
            switch (chr) {
                case '#':
                    s = State::OCC;
                    break;
                case 'L':
                    s = State::EMP;
                    break;
                case '.':
                    s = State::NON;
                    break;
                default:
                    throw std::runtime_error("Not found: ");
            }
            row.push_back(s);
        }
        grid.push_back(row);
    }

    std::cout << part_1(grid) << std::endl;
    std::cout << part_2(grid) << std::endl;

    return 0;
}

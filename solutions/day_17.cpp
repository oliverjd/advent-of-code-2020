#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <set>
#include <sstream>

using Row = std::vector<bool>;
using Grid2 = std::vector<Row>;
using Grid3 = std::vector<Grid2>;
using Grid4 = std::vector<Grid3>;

void iterate3(Grid3& grid) {
    // assumes grid is cubic and large enough that the edges are never touched

    std::vector<std::tuple<int, int, int>> dirs;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; ++k) {
                if (!(i == 0 && j == 0 && k == 0)) dirs.push_back({i, j, k});
            }
        }
    }

    auto new_grid = grid;
    const int n = grid.size();
    for (int i = 1; i < n - 1; ++i) {
        for (int j = 1; j < n - 1; ++j) {
            for (int k = 1; k < n - 1; ++k) {
                int neighbours = 0;
                for (const auto& [d1, d2, d3] : dirs) {
                    if (grid[i+d1][j+d2][k+d3]) ++neighbours;
                }
                if (grid[i][j][k] && !(neighbours == 2 || neighbours == 3)) new_grid[i][j][k] = false;
                if (!grid[i][j][k] && neighbours == 3) new_grid[i][j][k] = true;
            }
        }
    }
    grid = new_grid;
}

int part_1(const Grid2& inp) {
    const int n = 30; // size
    const int o = n / 2; // origin
    Grid3 grid(n, Grid2(n, Row(n, false)));

    for (int i = 0; i < inp.size(); ++i) {
        for (int j = 0; j < inp[i].size(); ++j) {
            grid[o][o+i][o+j] = inp[i][j];
        }
    }

    for (int i = 0; i < 6; ++i) {
        iterate3(grid);
    }

    int tot = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                tot += grid[i][j][k];
            }
        }
    }

    return tot;
}

void iterate4(Grid4& grid) {
    // assumes grid is (hyper)cubic and large enough that the edges are never touched

    std::vector<std::tuple<int, int, int, int>> dirs;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; ++k) {
                for (int l = -1; l <= 1; ++l) {
                    if (!(i == 0 && j == 0 && k == 0 && l == 0)) dirs.push_back({i, j, k, l});
                }
            }
        }
    }

    auto new_grid = grid;
    const int n = grid.size();
    for (int i = 1; i < n - 1; ++i) {
        for (int j = 1; j < n - 1; ++j) {
            for (int k = 1; k < n - 1; ++k) {
                for (int l = 1; l < n - 1; ++l) {
                    int neighbours = 0;
                    for (const auto& [d1, d2, d3, d4] : dirs) {
                        if (grid[i+d1][j+d2][k+d3][l+d4]) ++neighbours;
                    }
                    if (grid[i][j][k][l] && !(neighbours == 2 || neighbours == 3)) new_grid[i][j][k][l] = false;
                    if (!grid[i][j][k][l] && neighbours == 3) new_grid[i][j][k][l] = true;
                }
            }
        }
    }
    grid = new_grid;
}

int part_2(const Grid2& inp) {
    const int n = 30; // size
    const int o = n / 2; // origin
    Grid4 grid(n, Grid3(n, Grid2(n, Row(n, false))));

    for (int i = 0; i < inp.size(); ++i) {
        for (int j = 0; j < inp[i].size(); ++j) {
            grid[o][o][o+i][o+j] = inp[i][j];
        }
    }

    for (int i = 0; i < 6; ++i) {
        iterate4(grid);
    }

    int tot = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                for (int l = 0; l < n; ++l) {
                    tot += grid[i][j][k][l];
                }
            }
        }
    }

    return tot;
}

int main() {
    std::ifstream in("../input/day_17.txt");
    std::string line;

    Grid2 inp;
    while(std::getline(in, line)) {
        Row r;
        for (const auto& chr : line ) {
            switch (chr) {
                case '#':
                    r.push_back(true);
                    break;
                case '.':
                    r.push_back(false);
                    break;
            }
        }
        inp.push_back(r);
    }

    std::cout << part_1(inp) << std::endl;
    std::cout << part_2(inp) << std::endl;

    return 0;
}

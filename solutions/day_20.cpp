#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <deque>
#include <sstream>
#include <regex>

using Tile = std::vector<std::string>;
using Tiles = std::unordered_map<int, Tile>;
using Grid = std::vector<std::vector<Tile>>;
using Image = std::vector<std::string>;

enum class Dir {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
};

int edge_id(const std::string& edge) {
    // return min of the integer of the edge, assumes edges are unique
    int edgeno = 0;
    int edgerevno = 0;
    for (int i = 0; i < edge.size(); ++i) {
        edgeno <<= 1;
        edgerevno <<= 1;
        if (edge[i] == '#') {
            edgeno |= 1;
        } else {
            edgeno |= 0;
        }
        if (edge[edge.size()-1-i] == '#') {
            edgerevno |= 1;
        } else {
            edgerevno |= 0;
        }
    }
    return std::min(edgeno, edgerevno);
}

std::string get_edge(const Tile& tile, Dir dir) {
    std::string edge;
    int n = tile.size();
    switch (dir) {
        case Dir::TOP:
            return tile.front();
        case Dir::BOTTOM:
            return tile.back();
        case Dir::LEFT:
            for(int i = 0; i < n; ++i) {
                edge.push_back(tile[i][0]);
            }
            return edge;
        case Dir::RIGHT:
            for(int i = 0; i < n; ++i) {
                edge.push_back(tile[i][n - 1]);
            }
            return edge;
    }
    throw std::runtime_error("Not found");
}

Tiles get_corners(const Tiles& tiles) {
    Tiles corners;

    std::unordered_map<int, int> edge_ids;

    for (const auto& [tno, tile] : tiles) {
        const auto n = tile.size();
        auto topid = edge_id(get_edge(tile, Dir::TOP));
        auto botid = edge_id(get_edge(tile, Dir::BOTTOM));
        auto lid = edge_id(get_edge(tile, Dir::LEFT));
        auto rid = edge_id(get_edge(tile, Dir::RIGHT));

        if (edge_ids.find(topid) == edge_ids.end()) edge_ids[topid] = 0;
        if (edge_ids.find(botid) == edge_ids.end()) edge_ids[botid] = 0;
        if (edge_ids.find(lid) == edge_ids.end()) edge_ids[lid] = 0;
        if (edge_ids.find(rid) == edge_ids.end()) edge_ids[rid] = 0;
        ++edge_ids[topid];
        ++edge_ids[botid];
        ++edge_ids[lid];
        ++edge_ids[rid];
    }

    for (const auto& [tno, tile] : tiles) {
        const auto n = tile.size();
        auto topid = edge_id(get_edge(tile, Dir::TOP));
        auto botid = edge_id(get_edge(tile, Dir::BOTTOM));
        auto lid = edge_id(get_edge(tile, Dir::LEFT));
        auto rid = edge_id(get_edge(tile, Dir::RIGHT));

        int unique_count = 0;
        if (edge_ids.at(topid) == 1) ++unique_count;
        if (edge_ids.at(botid) == 1) ++unique_count;
        if (edge_ids.at(rid) == 1) ++unique_count;
        if (edge_ids.at(lid) == 1) ++unique_count;
        if (unique_count == 2) corners[tno] = tile;
    }

    return corners;
}

int64_t part_1(const Tiles& tiles) {
    const auto corners = get_corners(tiles);
    int64_t res = 1;
    for (const auto& [k, v] : corners) {
        res *= k;
    }
    return res;
}

Tile rotate_90_cw(const Tile& tile) {
    Tile newtile;
    const int n = tile.size();
    for (int i = 0; i < n; ++i) {
        newtile.push_back("");
        for(int j = 0; j < n; ++j) {
            newtile[i].push_back(tile[n-1-j][i]);
        }
    }
    return newtile;
}

Tile flip_h(const Tile& tile) {
    Tile newtile;
    const int n = tile.size();
    for (int i = 0; i < n; ++i) {
        auto rev = tile.at(i);
        std::reverse(rev.begin(), rev.end());
        newtile.push_back(rev);
    }
    return newtile;
}

Tile flip_v(const Tile& tile) {
    Tile newtile;
    const int n = tile.size();
    for (int i = 0; i < n; ++i) {
        newtile.push_back(tile.at(n-i-1));
    }
    return newtile;
}

Tile find_and_orient_tile(Tiles& remaining_tiles, Dir adjacent_edge_dir, const std::string& adjacent_edge,
        const std::unordered_map<int, std::unordered_set<int>>& edge_id_to_tiles) {
    // return the tile to go in place after rotating/flipping it if necessary
    auto eid = edge_id(adjacent_edge);
    auto found_tile_ids = edge_id_to_tiles.at(eid);
    int found_tile_id = -1;
    for (const auto& tid : found_tile_ids) {
        if (remaining_tiles.find(tid) != remaining_tiles.end()) {
            if (found_tile_id != -1) throw std::runtime_error("not 1 tile found");
            found_tile_id = tid;
        }
    }
    if (found_tile_id == -1) throw std::runtime_error("not found");
    auto tile = remaining_tiles.at(found_tile_id);

    Dir opposite_dir;
    if (adjacent_edge_dir == Dir::TOP) opposite_dir = Dir::BOTTOM;
    if (adjacent_edge_dir == Dir::BOTTOM) opposite_dir = Dir::TOP;
    if (adjacent_edge_dir == Dir::LEFT) opposite_dir = Dir::RIGHT;
    if (adjacent_edge_dir == Dir::RIGHT) opposite_dir = Dir::LEFT;

    while (edge_id(get_edge(tile, opposite_dir)) != eid) {
        tile = rotate_90_cw(tile);
    }
    if (get_edge(tile, opposite_dir) != adjacent_edge) {
        // flip it
        if (opposite_dir == Dir::BOTTOM || opposite_dir == Dir::TOP) {
            tile = flip_h(tile);
        }
        if (opposite_dir == Dir::LEFT || opposite_dir == Dir::RIGHT) {
            tile = flip_v(tile);
        }
    }

    remaining_tiles.erase(found_tile_id);
    return tile;
}

Image combine_grid(const Grid& grid) {
    Image image;
    int gridrownum = 0;
    for (const auto& gridrow : grid) {
        for (int i = 0; i < gridrow[0].size() - 2; ++i) {
            image.push_back("");
        }
        for (const auto& square : gridrow) {
            for (int i = 0; i < square.size() - 2; ++i) {
                auto rowstr = square.at(i+1).substr(1, square.size()-2);
                image[i + gridrownum * (square.size() - 2)] += rowstr;
            }
        }
        ++gridrownum;
    }
    image = rotate_90_cw(image);
    image = flip_h(image);
    return image;
}

int get_roughness(Image image) {
    std::vector<std::string> monster = {
        "                  # ",
        "#    ##    ##    ###",
        " #  #  #  #  #  #   "
    };

    const int m_h = monster.size();
    const int m_w = monster[0].size();
    const int n = image.size();
    int iters = 0;
    int monsters = 0;
    int monster_hashes = 0;
    while (monsters == 0) {
        for (int i = 0; i < n - m_h; ++i) {
            for (int j = 0; j < n - m_w; ++j) {
                bool match = true;
                int this_monster_hashes = 0;
                for (int y = 0; y < m_h; ++y) {
                    for (int x = 0; x < m_w; ++x) {
                        if (monster[y][x] == '#' && image[i+y][j+x] != '#') {
                            match = false;
                            break;
                        }
                        if (image[i+y][j+x] == '#' && monster[y][x] == '#') ++this_monster_hashes;
                    }
                }
                if (match) {
                    ++monsters;
                    monster_hashes += this_monster_hashes;
                }
            }
        }
        ++iters;
        if (iters == 4) image = flip_h(image);
        else if (iters == 8) image = flip_v(image);
        else image = rotate_90_cw(image);
    }
    int total_hashes = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (image[i][j] == '#') {
                ++total_hashes;
            }
        }
    }
    return total_hashes - monster_hashes;
}


int64_t part_2(const Tiles& tiles) {
    // first construct the image i.e. match up all the edges
    // assuming all edge_ids are unique, build grid starting from a corner then working along row-by-row

    Grid grid;

    for (const auto& [tno, tile] : tiles) {
        auto rotated = flip_h(tile);
    }

    std::unordered_map<int, std::unordered_set<int>> edge_id_to_tiles;

    for (const auto& [tno, tile] : tiles) {
        const auto n = tile.size();
        auto topid = edge_id(get_edge(tile, Dir::TOP));
        auto botid = edge_id(get_edge(tile, Dir::BOTTOM));
        auto lid = edge_id(get_edge(tile, Dir::LEFT));
        auto rid = edge_id(get_edge(tile, Dir::RIGHT));

        for (const auto& id : {topid, botid, lid, rid}) {
            if (edge_id_to_tiles.find(id) == edge_id_to_tiles.end()) edge_id_to_tiles[id] = {};
            edge_id_to_tiles[id].insert(tno);
        }
    }

    auto remaining_tiles = tiles;

    auto corners = get_corners(tiles);

    const int n = std::sqrt(tiles.size());

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == 0 && j == 0) {
                // for first one just pick any corner and rotate it so it fits in top left (bottom/right edges match)
                auto corner = corners.begin();
                auto corner_id = corner->first;
                auto corner_tile = corner->second;
                corners.erase(corner->first);
                remaining_tiles.erase(corner_id);
                bool to_flip_v = false;
                bool to_flip_h = false;
                for (const auto& dir : {Dir::TOP, Dir::LEFT, Dir::RIGHT, Dir::BOTTOM}) {
                    auto eid = edge_id(get_edge(corner_tile, dir));
                    if (edge_id_to_tiles[eid].size() == 2) {
                        if (dir == Dir::TOP) {
                            to_flip_v = true;
                        }
                        if (dir == Dir::LEFT) {
                            to_flip_h = true;
                        }
                    }
                }
                if (to_flip_h) corner_tile = flip_h(corner_tile);
                if (to_flip_v) corner_tile = flip_v(corner_tile);
                grid.push_back({corner_tile});
            } else if (j == 0) {
                // if i == 0 match tile from above based on its bottom edge
                auto& tile_above = grid[i-1][j];
                grid.push_back({find_and_orient_tile(remaining_tiles, Dir::BOTTOM, get_edge(tile_above, Dir::BOTTOM),
                                edge_id_to_tiles)});
            } else {
                // otherwise use tile to the left's right edge
                auto& tile_left = grid[i][j-1];
                grid[i].push_back(find_and_orient_tile(remaining_tiles, Dir::RIGHT, get_edge(tile_left, Dir::RIGHT),
                                  edge_id_to_tiles));
            }
        }
    }

    if (remaining_tiles.size() != 0) throw std::runtime_error("not finished");

    // then remove the edge borders/combine into single image
    auto image = combine_grid(grid);

    // finally calculate the answer
    return get_roughness(image);
}

int main() {
    std::ifstream in("../input/day_20.txt");

    Tiles tiles;
    std::string line;

    int thistile;
    Tile thetile;
    while(std::getline(in, line)) {
        if (line.substr(0, 4) == "Tile") {
            thistile = std::stoi(line.substr(5, 4));
        } else if (line.size() == 0) {
            tiles[thistile] = thetile;
            thetile = Tile();
        } else {
            thetile.push_back(line);
        }
    }

    std::cout << part_1(tiles) << std::endl;
    std::cout << part_2(tiles) << std::endl;

    return 0;
}

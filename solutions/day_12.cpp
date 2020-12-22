#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

using Instruction = std::pair<char, int>;

double deg2rad(double deg) {
    return deg * 2 * M_PI / 360.;
}

double rad2deg(double rad) {
    return 360. * rad / (2 * M_PI);
}

int part_1(const std::vector<Instruction>& inp) {
    int x = 0;
    int y = 0;
    int dir_deg = 90;
    for (const auto& [op, num] : inp) {
        switch (op) {
            case 'N':
                y += num;
                break;
            case 'S':
                y -= num;
                break;
            case 'E':
                x += num;
                break;
            case 'W':
                x -= num;
                break;
            case 'F':
                x += num * std::round(std::sin(deg2rad(dir_deg)));
                y += num * std::round(std::cos(deg2rad(dir_deg)));
                break;
            case 'L':
                dir_deg -= num;
                break;
            case 'R':
                dir_deg += num;
                break;
        }
    }
    return std::abs(x) + std::abs(y);
}

int part_2(const std::vector<Instruction>& inp) {
    int sx = 0;
    int sy = 0;
    int wx = 10;
    int wy = 1;
    double wdir = 0;
    double wmag = 0;
    for (const auto& [op, num] : inp) {
        switch (op) {
            case 'N':
                wy += num;
                break;
            case 'S':
                wy -= num;
                break;
            case 'E':
                wx += num;
                break;
            case 'W':
                wx -= num;
                break;
            case 'F':
                sx += wx * num;
                sy += wy * num;
                break;
            case 'L':
                wdir = rad2deg(std::atan(double(wx)/wy)) - num;
                if (wy < 0) wdir += 180;
                wmag = std::sqrt(wx*wx + wy*wy);
                if (std::abs(wmag) > 0.0001) {
                    wx = std::round(std::sin(deg2rad(wdir)) * wmag);
                    wy = std::round(std::cos(deg2rad(wdir)) * wmag);
                }
                break;
            case 'R':
                wdir = rad2deg(std::atan(double(wx)/wy)) + num;
                if (wy < 0) wdir += 180;
                wmag = std::sqrt(wx*wx + wy*wy);
                if (std::abs(wmag) > 0.0001) {
                    wx = std::round(std::sin(deg2rad(wdir)) * wmag);
                    wy = std::round(std::cos(deg2rad(wdir)) * wmag);
                }
                break;
        }
    }
    return std::abs(sx) + std::abs(sy);
}

int main() {
    std::ifstream in("../input/day_12.txt");
    std::string line;

    std::vector<Instruction> inp;

    while (std::getline(in, line)) {
        inp.emplace_back(line[0], std::stoi(line.substr(1)));
    }

    std::cout << part_1(inp) << std::endl;
    std::cout << part_2(inp) << std::endl;

    return 0;
}

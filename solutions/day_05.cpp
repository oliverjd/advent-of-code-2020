#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>

int seat_id(std::string code) {
    std::replace(code.begin(), code.end(), 'F', '0');
    std::replace(code.begin(), code.end(), 'B', '1');
    std::replace(code.begin(), code.end(), 'L', '0');
    std::replace(code.begin(), code.end(), 'R', '1');

    int row = std::stoi(code.substr(0, 7), nullptr, 2);
    int col = std::stoi(code.substr(7), nullptr, 2);

    return row * 8 + col;
}

int part_1(const std::vector<std::string>& vec) {
    std::vector<int> ids;
    for (const auto& x : vec) {
        ids.push_back(seat_id(x));
    }

    return *std::max_element(ids.cbegin(), ids.cend());
}

int part_2(const std::vector<std::string>& vec) {
    std::vector<int> ids;
    for (const auto& x : vec) {
        ids.push_back(seat_id(x));
    }

    std::sort(ids.begin(), ids.end());

    int last = 0;

    for (const auto& x : ids) {
        if (x != last + 1 && last != 0) {
            return last + 1;
        }
        last = x;
    }

    throw std::runtime_error("Not found");
}

int main() {
    std::ifstream in("../input/day_05.txt");
    std::string buf;
    std::vector<std::string> vec;
    while (std::getline(in, buf)) {
        vec.push_back(buf);
    }

    std::cout << part_1(vec) << std::endl;
    std::cout << part_2(vec) << std::endl;

    return 0;
}

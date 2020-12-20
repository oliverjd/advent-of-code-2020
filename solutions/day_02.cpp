#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <algorithm>
#include <tuple>
#include <sstream>
#include <iterator>
#include <string>

using Input = std::tuple<std::string, char, int, int>;

bool is_valid_1(const Input& inp) {
    int count = 0;
    for (const auto& c : std::get<0>(inp)) {
        if (c == std::get<1>(inp)) {
            ++count;
        }
    }
    return (count >= std::get<2>(inp) && count <= std::get<3>(inp));
}

int part_1(const std::vector<Input>& vec) {
    int count = 0;
    for (const auto& x : vec) {
        if (is_valid_1(x)) {
            ++count;
        }
    }
    return count;
}

bool is_valid_2(const Input& inp) {
    int count = 0;
    if (std::get<0>(inp)[std::get<2>(inp) - 1] == std::get<1>(inp)) {
        ++count;
    }
    if (std::get<0>(inp)[std::get<3>(inp) - 1] == std::get<1>(inp)) {
        ++count;
    }
    return count == 1;
}

int part_2(const std::vector<Input>& vec) {
    int count = 0;
    for (const auto& x : vec) {
        if (is_valid_2(x)) {
            ++count;
        }
    }
    return count;
}

Input parse_line(const std::string& line) {
    char delim = ' ';
    std::stringstream ss(line);
    int num1, num2;
    std::string nums, lets, pwd;
    std::getline(ss, nums, delim);
    std::stringstream ssnum(nums);
    std::string num1s, num2s;
    std::getline(ssnum, num1s, '-');
    std::getline(ssnum, num2s, '-');
    num1 = std::stoi(num1s);
    num2 = std::stoi(num2s);
    std::getline(ss, lets, delim);
    char letter = lets[0];
    std::getline(ss, pwd, delim);

    return std::make_tuple(pwd, letter, num1, num2);
}

int main() {
    std::ifstream in("../input/day_02.txt");
    std::string buf;
    std::vector<Input> vec;
    while (std::getline(in, buf)) {
        vec.push_back(parse_line(buf));
    }

    std::cout << part_1(vec) << std::endl;
    std::cout << part_2(vec) << std::endl;

    return 0;
}

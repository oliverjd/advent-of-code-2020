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

int calc_score(const std::deque<int>& deck_) {
    auto deck = deck_;
    int score = 0;
    int n = deck.size();
    for (int i = 1; i <= n; ++i) {
        score += i * deck.back();
        deck.pop_back();
    }
    return score;
}

int part_1(const std::deque<int>& p1_, const std::deque<int>& p2_) {
    auto p1 = p1_;
    auto p2 = p2_;
    while (p1.size() && p2.size()) {
        if (p1.front() > p2.front()) {
            p1.push_back(p1.front());
            p1.push_back(p2.front());
        } else {
            p2.push_back(p2.front());
            p2.push_back(p1.front());
        }
        p1.pop_front();
        p2.pop_front();
    }

    if (p1.size()) return calc_score(p1);
    else return calc_score(p2);
}

std::string hash_state(const std::deque<int>& p1, const std::deque<int>& p2) {
    std::string res = "p1:";
    for (const auto& x : p1) {
        res += std::to_string(x) + ",";
    }
    res += "p2:";
    for (const auto& x : p2) {
        res += std::to_string(x) + ",";
    }
    return res;
}

bool play_recursive(std::deque<int>& p1_, std::deque<int>& p2_, bool inner) {
    std::unordered_set<std::string> seen;
    auto p1 = p1_;
    auto p2 = p2_;

    int round = 0;
    while (p1.size() && p2.size()) {
        ++round;

        auto hash = hash_state(p1, p2);
        if (seen.find(hash) != seen.end()) {
            return true;
        }
        seen.insert(hash);
        int p1c = p1.front();
        int p2c = p2.front();
        p1.pop_front();
        p2.pop_front();

        bool winner;
        if (p1.size() >= p1c && p2.size() >= p2c) {
            auto p1tmp = p1;
            auto p2tmp = p2;
            std::deque<int> p1rec, p2rec;
            for (int i = 0; i < p1c; ++i) {
                p1rec.push_back(p1tmp.front());
                p1tmp.pop_front();
            }
            for (int i = 0; i < p2c; ++i) {
                p2rec.push_back(p2tmp.front());
                p2tmp.pop_front();
            }
            winner = play_recursive(p1rec, p2rec, true);
        } else {
            if (p1c > p2c) {
                winner = true;
            } else {
                winner = false;
            }
        }
        if (winner) {
            p1.push_back(p1c);
            p1.push_back(p2c);
        } else {
            p2.push_back(p2c);
            p2.push_back(p1c);
        }

    }
    if (!inner) {
        p1_ = p1;
        p2_ = p2;
    }
    if (p1.size()) return true;
    else return false;
}

int part_2(const std::deque<int>& p1, const std::deque<int>& p2) {
    auto p1c = p1;
    auto p2c = p2;
    play_recursive(p1c, p2c, false);

    if (p1c.size()) return calc_score(p1c);
    else return calc_score(p2c);
}

int main() {
    std::ifstream in("../input/day_22.txt");
    std::string line;
    std::string word;

    std::deque<int> p1, p2;
    int section = 1;
    while(std::getline(in, line)) {
        if (line.size() == 0) {
            section = 2;
        } else {
            if (section == 1) {
                if (line[0] != 'P') {
                    p1.push_back(std::stoi(line));
                }
            }
            if (section == 2) {
                if (line[0] != 'P') {
                    p2.push_back(std::stoi(line));
                }
            }
        }
    }

    std::cout << part_1(p1, p2) << std::endl;
    std::cout << part_2(p1, p2) << std::endl;

    return 0;
}

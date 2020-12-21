#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <deque>

using Rules = std::unordered_map<std::string, std::vector<std::pair<int, std::string>>>;

int part_1(const Rules& rules) {
    std::string target = "shinygold";

    std::unordered_map<std::string, std::vector<std::string>> rules_rev;
    for (const auto& [k, vs] : rules) {
        for (const auto& [num, v] : vs) {
        if (rules_rev.find(v) == rules_rev.end()) {
            rules_rev[v] = {};
        }
        rules_rev[v].push_back(k);
        }
    }

    std::unordered_set<std::string> seen;
    std::deque<std::string> queue = {target};
    while (queue.size()) {
        auto val = queue.front();
        queue.pop_front();
        for (const auto& v : rules_rev[val]) {
            if (seen.find(v) == seen.end()) {
                seen.insert(v);
                queue.push_back(v);
            }
        }
    }

    return seen.size();
}

int count_under(const std::string& col, const Rules& rules) {
    int tot = 0;
    for (const auto& [num, c] : rules.at(col)) {
        tot += num * (1 + count_under(c, rules));
    }
    return tot;
}

int part_2(const Rules& rules) {
    return count_under("shinygold", rules);
}

int main() {
    std::ifstream in("../input/day_07.txt");
    std::string line;
    std::string word;
    Rules rules;

    while (std::getline(in, line)) {
        std::stringstream liness(line);
        std::string leftcol;
        std::string col;
        int num = 0;
        std::vector<std::pair<int, std::string>> rightrules;
        int i = 0;
        bool skip = false;
        while (std::getline(liness, word, ' ')) {
            if (i < 2) {
                leftcol += word;
            }
            if (i >= 4) {
                int rulei = (i - 4) % 4;
                if (rulei == 0) {
                    if (word == "no") {
                        skip = true;
                    } else {
                        num = std::stoi(word);
                    }
                }
                if (rulei == 1 || rulei == 2) {
                    col += word;
                }
                if (rulei == 3) {
                    if (!skip) {
                        rightrules.push_back({num, col});
                    }
                    col = "";
                }
            }
            ++i;
        }
        rules[leftcol] = rightrules;
    }

    std::cout << part_1(rules) << std::endl;
    std::cout << part_2(rules) << std::endl;

    return 0;
}

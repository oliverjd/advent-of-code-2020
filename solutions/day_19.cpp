#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <set>
#include <deque>
#include <sstream>
#include <regex>

using Rule = std::string;
using Rules = std::unordered_map<int, Rule>;

std::string expand_rules(const Rules& rules_, int start_rule) {
    auto rules = rules_;
    for (auto& [ruleno, r] : rules) {
        r.insert(0, " ( ");
        r.insert(r.size(), " ) ");
    }

    std::string ex = rules.at(start_rule);

    int changed = true;
    while (changed) {
        changed = false;
        std::string newex;
        std::stringstream ss(ex);
        std::string tok;
        while(std::getline(ss, tok, ' ')) {
            try {
                int ruleno = std::stoi(tok); // so it's a number -> replace it with the rule it corresponds to
                newex += rules.at(ruleno);
                changed = true;

            } catch (std::invalid_argument &e) {
                // it's either | or a letter or a space - do nothing except push back to the new string
                newex += " " + tok;
            }
        }
        ex = newex;
    }

    // finally delete all the spaces
    std::string newex;
    for (int i = 0; i < ex.size(); ++i) {
        if (ex[i] != ' ') newex.push_back(ex[i]);
    }
    ex = newex;

    return ex;
}

int part_1(const Rules& rules, const std::vector<std::string>& patterns) {
    std::string ex = expand_rules(rules, 0);
    const std::regex re(ex);

    int matches = 0;
    for (const auto& p : patterns) {
        std::smatch base_match;
        if (std::regex_match(p, base_match, re)) {
            ++matches;
        }
    }
    return matches;
}

int part_2(const Rules& rules, const std::vector<std::string>& patterns) {
    std::string ex = expand_rules(rules, 0);

    std::string ex42 = expand_rules(rules, 42);
    std::string ex31 = expand_rules(rules, 31);

    int matches = 0;
    bool increasing = true;
    int i = 1;
    while (increasing) {
        increasing = false;
        std::string sre = ex42 + "+" + ex42 + "{" + std::to_string(i) + "}" + ex31 + "{" + std::to_string(i) + "}";
        const std::regex re(sre);

        for (const auto& p : patterns) {
            std::smatch base_match;
            if (std::regex_match(p, base_match, re)) {
                increasing = true;
                ++matches;
            }
        }
        ++i;
    }
    return matches;
}

int main() {
    std::ifstream in("../input/day_19.txt");
    std::string line;

    Rules rules;
    std::vector<std::string> patterns;
    int section = 1;
    while(std::getline(in, line)) {
        if (line.size() == 0) {
            section = 2;
        }
        if (section == 1) {
            std::stringstream ss(line);
            std::string token;
            std::getline(ss, token, ':');
            int ruleno = std::stoi(token);
            std::getline(ss, token, ':');
            std::string seq;
            std::stringstream ss2(token);
            std::string num;
            Rule r;
            if (token[1] == '"') {
                r = std::string(1, token[2]);
            } else {
                r = token.substr(1);
            }
            rules[ruleno] = r;
        }
        if (section == 2) patterns.push_back(line);
    }

    std::cout << part_1(rules, patterns) << std::endl;
    std::cout << part_2(rules, patterns) << std::endl;

    return 0;
}

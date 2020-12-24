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

enum class Op {
    ADD,
    MULT
};

int64_t solve(const std::string& qstr, int start, int end, int& skip) {
    // left to right, but when we see a paren just recurse on its contents
    // when we see (, immediately recurse on everything following then return when we see ) (or reach end)

    int64_t tot = 0;
    Op op = Op::ADD;
    for (int i = start; i < end; ++i) {
        try {
            int64_t digit = std::stoll(std::string(1, qstr[i]));
            if (op == Op::ADD) tot += digit;
            if (op == Op::MULT) tot *= digit;
        } catch (std::invalid_argument) {
            switch(qstr[i]) {
                case '(':
                    if (op == Op::ADD) tot += solve(qstr, i+1, end, skip);
                    if (op == Op::MULT) tot *= solve(qstr, i+1, end, skip);
                    i = skip;
                    break;
                case ')':
                    skip = i ;
                    return tot;
                case '*':
                    op = Op::MULT;
                    break;
                case '+':
                    op = Op::ADD;
                    break;
            }
        }
    }
    return tot;
}

int64_t part_1(const std::vector<std::string>& inp) {
    int64_t tot = 0;
    for (const auto& qstr : inp) {
        int skip = 0;
        tot += solve(qstr, 0, qstr.size(), skip);
    }
    return tot;
}

std::string add_parens_for_precedence(const std::string& qstr) {
    auto nstr = qstr;

    // loop over chars, if you find a + put a ( 2 chars before and a ) after the next char unless it's a ( in which case
    // keep going until its corresponding ) and put the next ) after that, but then go back to where we were
    for (int i = 0; i < nstr.size(); ++i) {
        if (nstr[i] == '+') {
            nstr.insert(i-2, 1, '(');
            ++i;
            if (nstr[i+2] != '(') {
                nstr.insert(i+3, 1, ')');
                ++i;
            } else {
                // we have a bracket so find its closing bracket and put another close after it:
                std::deque<char> queue = {'('};
                int j = i+2;
                while (queue.size() > 0) {
                    ++j;
                    auto chr = nstr[j];
                    if (chr == '(') {
                        queue.push_back('(');
                    } else if (chr == ')' && queue.back() == '(') {
                        queue.pop_back();
                    }
                }
                nstr.insert(j+1, 1, ')');
            }
        }
    }
    // delete any pairs of ()
    for (int i = 0; i < nstr.size()-1; ++i) {
        if (nstr[i] == '(' && nstr[i+1] == ')') {
            nstr.erase(i, 2);
        }
    }

    return nstr;
}

int64_t part_2(const std::vector<std::string>& inp) {
    // preprocess by adding parens around every + op, then same as part 1
    int64_t tot = 0;
    for (const auto& qstr : inp) {
        int skip = 0;
        auto nstr = add_parens_for_precedence(qstr);
        auto res = solve(nstr, 0, nstr.size(), skip);
        tot += res;
    }
    return tot;
}

int main() {
    std::ifstream in("../input/day_18.txt");
    std::string line;

    std::vector<std::string> inp;
    while(std::getline(in, line)) {
        inp.push_back(line);
    }

    std::cout << part_1(inp) << std::endl;
    std::cout << part_2(inp) << std::endl;

    return 0;
}

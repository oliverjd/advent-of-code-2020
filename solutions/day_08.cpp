#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <deque>

enum class Op {
    NOP = 0,
    ACC,
    JMP,
};

using Code = std::vector<std::pair<Op, int>>;

void run_code(const Code& code, bool& terminates, int& final_acc) {
    int i = 0;
    int acc = 0;

    std::unordered_set<int> seen_i;
    while (true) {
        if (seen_i.find(i) != seen_i.end()) {
            final_acc = acc;
            terminates = false;
            return;
        }
        seen_i.insert(i);
        if (i == code.size()) {
            final_acc = acc;
            terminates = true;
            return;
        }

        switch (code[i].first) {
            case Op::ACC:
                acc += code[i].second;
                ++i;
                break;
            case Op::JMP:
                i += code[i].second;
                break;
            case Op::NOP:
                ++i;
                break;
        }
    }
}

int part_1(const Code& code) {
    bool terminates = false;
    int acc = 0;
    run_code(code, terminates, acc);
    return acc;
}

int part_2(const Code& code) {
    for (int i = 0; i < code.size(); ++i) {
        bool terminates = false;
        int acc = 0;
        auto code_changed = code;
        if (code_changed[i].first == Op::NOP) {
            code_changed[i].first = Op::JMP;
        } else if (code_changed[i].first == Op::JMP) {
            code_changed[i].first = Op::NOP;
        }
        run_code(code_changed, terminates, acc);
        if (terminates) {
            return acc;
        }
    }
    throw std::runtime_error("Not found");
}

int main() {
    std::ifstream in("../input/day_08.txt");
    std::string line;
    Code code;

    while (std::getline(in, line)) {
        std::string opc = line.substr(0, 3);
        Op op;
        if (opc == "nop") {
            op = Op::NOP;
        } else if (opc == "acc") {
            op = Op::ACC;
        } else if (opc == "jmp") {
            op = Op::JMP;
        }
        int num = std::stoi(line.substr(4));
        code.emplace_back(std::make_pair(op, num));
    }

    std::cout << part_1(code) << std::endl;
    std::cout << part_2(code) << std::endl;

    return 0;
}

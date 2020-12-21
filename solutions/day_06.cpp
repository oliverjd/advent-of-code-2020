#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <unordered_set>

using Group = std::vector<std::string>;
using Groups = std::vector<Group>;

int part_1(const Groups& groups) {
    int sum = 0;
    for (const auto& group : groups) {
        std::unordered_set<char> seen;
        for (const auto& person : group) {
            for (const auto& chr : person) {
                seen.insert(chr);
            }
        }
        sum += seen.size();
    }
    return sum;
}

int part_2(const Groups& groups) {
    int sum = 0;
    for (const auto& group : groups) {
        std::unordered_set<char> seen;
        for (const auto & chr : group[0]) {
            seen.insert(chr);
        }

        for (const auto& person : group) {
            std::unordered_set<char> this_group, to_del;
            for (const auto& chr : person) {
                this_group.insert(chr);
            }

            for (const auto& chr : seen) {
                if (this_group.find(chr) == this_group.end()) {
                    to_del.insert(chr);
                }
            }
            for (const auto & chr : to_del) {
                seen.erase(chr);
            }

        }
        sum += seen.size();
    }
    return sum;
}

int main() {
    std::ifstream in("../input/day_06.txt");
    std::string buf;
    Groups groups;

    Group group;
    while (std::getline(in, buf)) {
        if (buf.size() == 0) {
            groups.push_back(group);
            group = Group();
        } else {
            group.push_back(buf);
        }
    }
    groups.push_back(group);
    group = Group();

    std::cout << part_1(groups) << std::endl;
    std::cout << part_2(groups) << std::endl;

    return 0;
}

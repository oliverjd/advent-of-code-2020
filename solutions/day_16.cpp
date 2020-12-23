#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

using Rules = std::unordered_map<std::string, std::unordered_set<int>>;

int part_1(const std::vector<int>& my_ticket, const std::vector<std::vector<int>>& nearby_tickets, const Rules& rules) {
    int invalid = 0;

    for (const auto& t : nearby_tickets) {
        for (const auto& tnum : t) {
            bool ok = false;
            for (const auto& [rname, rs] : rules) {
                if (rs.find(tnum) != rs.end()) {
                    ok = true;
                }
            }
            if (!ok) invalid += tnum;
        }
    }

    return invalid;
}

int64_t part_2(const std::vector<int>& my_ticket, const std::vector<std::vector<int>>& nearby_tickets, const Rules& rules) {
    std::vector<std::vector<int>> tickets;

    for (const auto& t : nearby_tickets) {
        bool t_ok = true;
        for (const auto& tnum : t) {
            bool ok = false;
            for (const auto& [rname, rs] : rules) {
                if (rs.find(tnum) != rs.end()) {
                    ok = true;
                }
            }
            if (!ok) t_ok = false;
        }
        if (t_ok) tickets.push_back(t);
    }

    // loop over tickets then over numbers. we start with saying any num_i can be for any rule. then whenever we find a
    // value outside that rule range, remove that i from that rule possibilities, then at the end hopefully we have at
    // least 1 rule with only 1 possibility
    std::unordered_map<int, std::unordered_set<std::string>> poss;
    int n_fields = my_ticket.size();
    for (const auto& [rname, rs] : rules) {
        for (int i = 0; i < n_fields; ++i) {
            if (poss.find(i) == poss.end()) poss[i] = {};
            poss[i].insert(rname);
        }
    }

    for (const auto& t : tickets) {
        for (int i = 0; i < n_fields; ++i) {
            std::unordered_set<std::string> to_del;
            for (const auto& fname : poss[i]) {
                if (rules.at(fname).find(t[i]) == rules.at(fname).end()) {
                    to_del.insert(fname);
                }
            }
            for (const auto& x : to_del) {
                poss[i].erase(x);
            }
        }
    }

    // now while we don't have all places, keep going through looking for single ones and if we find them we remove
    // that one from others
    std::unordered_map<std::string, int> found;
    while (true) {
        for (const auto& [i, fields] : poss) {
            if (fields.size() == 1) {
                // we know this one so set it and remove that field from all others
                auto foundfield = *fields.begin();
                found[foundfield] = i;
                for (auto& [j, fields2] : poss) {
                    if (j != i && fields2.find(foundfield) != fields2.end()) {
                        fields2.erase(foundfield);
                    }
                }
            }
        }
        if (found.size() == n_fields) {
            break;
        }
    }

    int64_t ans = 1;
    for (const auto& [k, v] : found) {
        std::string tgt = "departure ";
        if (k.substr(0, tgt.size()) == tgt) {
            ans *= (int64_t)my_ticket[v];
        }
    }

    return ans;
}

int main() {
    std::ifstream in("../input/day_16.txt");
    std::string line;

    int section = 0;
    std::vector<int> my_ticket;
    std::vector<std::vector<int>> nearby_tickets;
    Rules rules;
    while(std::getline(in, line)) {
        if (line.size() == 0) {
            ++section;
            continue;
        }
        switch (section) {
            case 0: // rules
                {
                    std::stringstream ss(line);
                    std::string tok;
                    std::getline(ss, tok, ':');
                    auto name = tok;
                    std::getline(ss, tok, '-');
                    int start1 = std::stoi(tok);
                    std::getline(ss, tok, ' ');
                    int end1 = std::stoi(tok);
                    std::getline(ss, tok, ' ');
                    std::getline(ss, tok, '-');
                    int start2 = std::stoi(tok);
                    std::getline(ss, tok);
                    int end2 = std::stoi(tok);
                    std::unordered_set<int> r;
                    for (int i = start1; i <= end1; ++i) {
                        r.insert(i);
                    }
                    for (int i = start2; i <= end2; ++i) {
                        r.insert(i);
                    }
                    rules[name] = r;
                }
                break;
            case 1: // my ticket
                if (line != "your ticket:") {
                    std::stringstream ss(line);
                    std::string num;
                    while (std::getline(ss, num, ',')) {
                        my_ticket.push_back(std::stoi(num));
                    }
                }
                break;
            case 2: // other tickets
                if (line != "nearby tickets:") {
                    std::vector<int> t;
                    std::stringstream ss(line);
                    std::string num;
                    while (std::getline(ss, num, ',')) {
                        t.push_back(std::stoi(num));
                    }
                    nearby_tickets.push_back(t);
                }
                break;
        }
    }

    std::cout << part_1(my_ticket, nearby_tickets, rules) << std::endl;
    std::cout << part_2(my_ticket, nearby_tickets, rules) << std::endl;

    return 0;
}

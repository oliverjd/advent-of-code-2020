#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <unordered_set>
#include <algorithm>
#include <sstream>

struct Passport {
    std::string byr;
    std::string iyr;
    std::string eyr;
    std::string hgt;
    std::string hcl;
    std::string ecl;
    std::string pid;
    std::string cid;

    bool all_fields_present_ignore_cid() const {
        return byr.size() && iyr.size() && eyr.size() && hgt.size() && hcl.size() && ecl.size() && pid.size();
    }

    bool all_fields_valid_ignore_cid() const {
        return byr_valid() && iyr_valid() && eyr_valid() && hgt_valid() && hcl_valid() && ecl_valid() && pid_valid();
    }

    bool byr_valid() const {
        try {
            const auto byr_n = std::stoi(byr);
            return byr_n >= 1920 && byr_n <= 2002;
        } catch (std::invalid_argument) {
            return false;
        }
    }

    bool iyr_valid() const {
        try {
            const auto iyr_n = std::stoi(iyr);
            return iyr_n >= 2010 && iyr_n <= 2020;
        } catch (std::invalid_argument) {
            return false;
        }
    }

    bool eyr_valid() const {
        try {
            const auto eyr_n = std::stoi(eyr);
            return eyr_n >= 2020 && eyr_n <= 2030;
        } catch (std::invalid_argument) {
            return false;
        }
    }

    bool hgt_valid() const {
        if (hgt.size() == 4) {
            if (hgt.substr(2) == "in") {
                try {
                    int num = std::stoi(hgt.substr(0, 2));
                    return (num >= 59 && num <= 76);
                } catch(std::invalid_argument) {
                    return false;
                }
            }
        } else if (hgt.size() == 5) {
            if (hgt.substr(3) == "cm") {
                try {
                    int num = std::stoi(hgt.substr(0, 3));
                    return (num >= 150 && num <= 193);
                } catch(std::invalid_argument) {
                    return false;
                }
            }
        }
        return false;
    }

    bool hcl_valid() const {
        bool good = hcl.size() == 7;
        if (good) {
            if (hcl[0] != '#') good = false;
            for (int i = 1; i < 7; ++i) {
                if (!((hcl[i] >= 48 && hcl[i] <= 57) || (hcl[i] >= 97 && hcl[i] <= 102))) good = false;
            }
        }
        return good;
    }

    std::unordered_set<std::string> valid_ecls = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};

    bool ecl_valid() const {
        return valid_ecls.find(ecl) != valid_ecls.end();
    }

    bool pid_valid() const {
        bool good = pid.size() == 9;
        for (const auto& c : pid) {
            try {
                std::stoi(std::string(1, c));
            } catch (std::invalid_argument) {
                good = false;
            }
        }
        return good;
    }
};

int part_1(const std::vector<Passport>& vec) {
    int count = 0;
    for (const auto& p : vec) {
        count += p.all_fields_present_ignore_cid();
    }
    return count;
}

int part_2(const std::vector<Passport>& vec) {
    int count = 0;
    for (const auto& p : vec) {
        count += p.all_fields_valid_ignore_cid();
    }
    return count;
}

int main() {
    std::ifstream in("../input/day_04.txt");
    std::string buf;
    std::vector<Passport> vec;

    Passport p;
    while (std::getline(in, buf)) {
        if (buf.size() == 0) {
            vec.push_back(p);
            p = Passport();
        }

        std::stringstream ss(buf);
        std::string token;
        while (std::getline(ss, token, ' ')) {
            std::string prop = token.substr(0, 3);
            std::string val = token.substr(4);
            if (prop == "byr") {
                p.byr = val;
            } else if (prop == "iyr") {
                p.iyr = val;
            } else if (prop == "eyr") {
                p.eyr = val;
            } else if (prop == "hgt") {
                p.hgt = val;
            } else if (prop == "hcl") {
                p.hcl = val;
            } else if (prop == "ecl") {
                p.ecl = val;
            } else if (prop == "pid") {
                p.pid = val;
            } else if (prop == "cid") {
                p.cid = val;
            }
        }
    }

    std::cout << part_1(vec) << std::endl;
    std::cout << part_2(vec) << std::endl;

    return 0;
}

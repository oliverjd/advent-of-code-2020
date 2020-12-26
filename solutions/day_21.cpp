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

using Food = std::pair<std::vector<std::string>, std::vector<std::string>>;
using Foods = std::vector<Food>;

std::unordered_map<std::string, std::string> match_ingredients(const Foods& foods) {
    // allergen can be any of the intersection of ingredients in rows where it appears so go through doing intersection
    std::unordered_map<std::string, std::unordered_set<std::string>> allergens_to_possible_foods;
    for (const auto & [ingredients, allergens] : foods) {
        for (const auto& allergen : allergens) {
            if (allergens_to_possible_foods.find(allergen) == allergens_to_possible_foods.end()) {
                allergens_to_possible_foods[allergen] = {};
                for (const auto& ingr : ingredients) {
                    allergens_to_possible_foods[allergen].insert(ingr);
                }
            } else {
                auto& existing = allergens_to_possible_foods[allergen];
                std::unordered_set<std::string> new_foods = {};
                for (const auto& ingr : ingredients) {
                    new_foods.insert(ingr);
                }
                std::unordered_set<std::string> intersect;
                for (const auto& x : existing) {
                    if (new_foods.find(x) != new_foods.end()) {
                        intersect.insert(x);
                    }
                }
                existing = intersect;
            }
        }
    }

    for (const auto& [allergen, foods] : allergens_to_possible_foods) {
        for (const auto& x : foods) {
        }
    }

    std::unordered_set<std::string> unfound_allergens;
    for (const auto& [k, v] : allergens_to_possible_foods) {
        unfound_allergens.insert(k);
    }
    std::unordered_map<std::string, std::string> allergen_to_food;
    while(unfound_allergens.size() != 0) {
        auto unfound = unfound_allergens;
        for (const auto& allergen : unfound) {
            const auto ingrs = allergens_to_possible_foods[allergen];
            if (ingrs.size() == 1) {
                auto found_ingr = *ingrs.begin();
                allergen_to_food[allergen] = found_ingr;
                unfound_allergens.erase(allergen);
                for (auto& [allergen, ingrs] : allergens_to_possible_foods) {
                    if (ingrs.find(found_ingr) != ingrs.end()) {
                        ingrs.erase(found_ingr);
                    }
                }
            }
        }
    }

    return allergen_to_food;
}

int part_1(const Foods& foods) {
    auto allergen_to_food = match_ingredients(foods);
    std::unordered_map<std::string, std::string> food_to_allergen;
    for (const auto& [k, v] : allergen_to_food) {
        food_to_allergen[v] = k;
    }

    int total = 0;
    for (const auto & [ingredients, allergens] : foods) {
        for (const auto& ingr : ingredients) {
            if (food_to_allergen.find(ingr) == food_to_allergen.end()) ++total;
        }
    }

    return total;
}

std::string part_2(const Foods& foods) {
    auto allergen_to_food = match_ingredients(foods);
    std::unordered_map<std::string, std::string> food_to_allergen;
    for (const auto& [k, v] : allergen_to_food) {
        food_to_allergen[v] = k;
    }

    std::vector<std::string> dangerous, allergens;
    for (const auto& [ingr, allergen] : food_to_allergen) {
        allergens.push_back(allergen);
    }
    std::sort(allergens.begin(), allergens.end());
    for (const auto& x : allergens) {
        dangerous.push_back(allergen_to_food[x]);
    }
    std::string res;
    for (int i = 0; i < dangerous.size(); ++i) {
        res += dangerous[i];
        if (i < dangerous.size() - 1) res += ",";
    }

    return res;
}

int main() {
    std::ifstream in("../input/day_21.txt");
    std::string line;
    std::string word;

    Foods foods;
    std::vector<std::string> patterns;
    while(std::getline(in, line)) {
        Food food;
        std::stringstream ss(line);
        bool contains = false;
        while (std::getline(ss, word, ' ')) {
            if (word[0] == '(') {
                contains = true;
            } else {
                if (contains) {
                    auto theword = word.substr(0, word.size()-1);
                    food.second.push_back(theword);
                } else {
                    food.first.push_back(word);
                }
            }
        }
        foods.push_back(food);
    }

    std::cout << part_1(foods) << std::endl;
    std::cout << part_2(foods) << std::endl;

    return 0;
}

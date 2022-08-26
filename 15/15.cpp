// this could have been done better

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <tuple>
#include <set>
#include "src/parsers.h"

struct Spice
{   
    std::string name;
    int capacity;
    int durability;
    int flavour;
    int texture;
    int calories;
};

// Sprinkles: capacity 2, durability 0, flavor -2, texture 0, calories 3
Spice parse_spice(std::string& input)
{
    std::string_view input_view {input};
    std::vector<std::string> words;
    for (auto i {0}; i < 11; ++i)
    {
        words.push_back(StringParsers::parse_word(input_view));
    }
    auto stoi_pop = [&](int i)
    {
        words[i].pop_back();
        return std::stoi(words[i]);
    };
    return {words[0], stoi_pop(2), stoi_pop(4), stoi_pop(6), stoi_pop(8), std::stoi(words[10])};
}

class Dish
{
private:
    typedef std::map<std::string, int> rec_t;
    rec_t recipe;
public:
    std::vector<Spice> kitchen;

    rec_t add_ingridient(std::set<rec_t>& checked, rec_t previous, int cal_limit, int spoon_limit)
        {
            auto old_recipe = recipe;
            recipe = previous;
            if (calc_calories() >= cal_limit || calc_spoons() >= spoon_limit || calc_score() < 0)
            {
                recipe = old_recipe;
                return previous;
            }
            std::vector<rec_t> temp_recipies {};
            for (auto spice = kitchen.rbegin(); spice != kitchen.rend(); ++spice)
            {
                ++previous.at(spice->name);
                if (!checked.contains(previous))
                {
                    checked.insert(previous);
                    temp_recipies.push_back(add_ingridient(checked, previous, cal_limit, spoon_limit));
                }
                --previous.at(spice->name);
            }

            int max_score = 0;
            rec_t best_rec {};

            for (auto rec : temp_recipies)
            {
                recipe = rec;
                int score = calc_score();
                if (score > max_score && calc_spoons() == spoon_limit && calc_calories() == cal_limit)
                {
                    max_score = score;
                    // std::cout << " score: " << score << std::endl;
                    best_rec = rec;
                }
            }

            if (max_score == 0)
            {
                recipe = old_recipe;
                return previous;
            }
            
            recipe = old_recipe;
            return best_rec;
        };

    void generate_recipe(int cal_limit, int spoon_limit)
    {
        auto old_rec = recipe;
        std::set<rec_t> checked {};
        recipe = add_ingridient(checked, old_rec, cal_limit, spoon_limit);        
    }

    void start_cooking(int i = 0)
    {
        for (auto spice : kitchen)
        {
            recipe.insert({spice.name, i});
        }
    }

    int calc_score()
    {
        int cap {0};
        int dur {0};
        int fla {0};
        int tex {0};
        for (auto spice : kitchen) 
        {
            cap += recipe.at(spice.name) * spice.capacity;
            dur += recipe.at(spice.name) * spice.durability;
            fla += recipe.at(spice.name) * spice.flavour;
            tex += recipe.at(spice.name) * spice.texture;
        }
        auto neg = [](int &what)
        {
            if (what <= 0) what = 0;
        };
        neg(cap);
        neg(dur);
        neg(fla);
        neg(tex);
        return cap * dur * fla * tex;
    }

    int calc_calories()
    {
        int cal {0};
        for (auto spice : kitchen) 
        {
            cal += recipe.at(spice.name) * spice.calories;
        }
        return cal;
    }

    int calc_spoons()
    {
    int spoon {0};
    for (auto spice : kitchen) 
    {
        spoon += recipe.at(spice.name);
    }
    return spoon;
    }

    void add_best()
    {
        auto max_grad {0};
        std::string best_spice {};
        for (auto spice : kitchen) 
        {
            ++recipe.at(spice.name);
            int grad = calc_score();
            --recipe.at(spice.name);

            if (grad > max_grad)
            {
                max_grad = grad;
                best_spice = spice.name;
            }
        }

        std::cout << "adding " << best_spice << std::endl;
        ++recipe.at(best_spice);
    }

    void lower_calories(int target)
    {
        std::map<int, std::vector<std::string>> cal_map {};
        for (Spice spice: kitchen)
        {
            if (cal_map.contains(spice.calories))
            {
                cal_map.at(spice.calories).push_back(spice.name);
            } else {
                cal_map.insert({spice.calories, {spice.name}});
            }
        }
        // searching for a best replacement
        // score loss, spice from, spice to
        while (calc_calories() > target)
        {
            // calorie change, replacements
            std::map<int, std::vector<std::tuple<int, std::string, std::string>>> replace_map {};
            for (auto diet_spice = cal_map.begin(); diet_spice != cal_map.end(); ++diet_spice)
            {                
                for (auto nondiet_spice = cal_map.rbegin() ; nondiet_spice != cal_map.rend(); ++nondiet_spice)
                {
                    auto cal_win = (nondiet_spice->first) - (diet_spice->first);
                    if (cal_win < 0) continue;
                    if (!replace_map.contains(cal_win))
                    {
                        replace_map.insert({cal_win, {}});
                    }
                    for (auto ds : diet_spice->second) {
                        for (auto nds : nondiet_spice->second) {
                            ++recipe.at(ds);
                            --recipe.at(nds);
                            replace_map.at(cal_win).push_back({calc_score(), nds, ds});
                            --recipe.at(ds);
                            ++recipe.at(nds);
                        }
                    }
                }
            }
            int new_score {0};
            int old_score {calc_score()};
            std::string nds, ns;
            for (auto calvin : replace_map)
            {
                if (calvin.first == 0) continue;
                for (auto replacement : calvin.second)
                {
                    std::cout << std::get<1>(replacement) << std::get<2>(replacement) << std::get<0>(replacement) << std::endl;
                    if (std::get<0>(replacement) >= new_score && std::get<0>(replacement) != old_score)
                    {
                        new_score = std::get<0>(replacement);
                        nds = std::get<1>(replacement);
                        ns = std::get<2>(replacement);
                    }
                }
            }

            --recipe.at(nds);
            ++recipe.at(ns);
        }

    }
};



int main()
{
    std::ifstream ifs;
    FileParsers::open_input(ifs);
 
    std::vector<Spice> kitchen;

    while(true)
    {
        std::string line {};
        std::getline(ifs, line);
        if (ifs.eof()) {break;}
        kitchen.push_back(parse_spice(line));
    }
    Dish cookie;
    cookie.kitchen = kitchen;
    cookie.start_cooking(1);
    // ascending with the score gradient worked for the first part, but not the second
    // for (int i = 0; i < 96; ++i) cookie.add_best();
    // bruteforce, here I go
    cookie.generate_recipe(500, 100);

    std::cout << cookie.calc_calories() << std::endl;
    std::cout << cookie.calc_score() << std::endl;
    std::cout << cookie.calc_spoons() << std::endl;
    

    return 0;
}
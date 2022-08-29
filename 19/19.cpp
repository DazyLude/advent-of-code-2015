#include <string>
#include <string_view>
#include <set>
#include <map>
#include <fstream>
#include <vector>
#include "src/parsers.h"

using std::string;
using std::set;
using std::map;

// replace first with second
typedef std::pair<string, string> rep;

void replace_magic(set<string>& target, const string& original, rep recipe)
{
    string::size_type cur {0};
    while (true)
    {
        cur = original.find(recipe.first, cur);
        if (cur == string::npos) break;

        string temp_s {""};
        temp_s += original.substr(0, cur);
        temp_s += recipe.second;
        temp_s += original.substr(cur + recipe.first.size());
        target.insert(temp_s);
        ++cur;
    }
}

void round_of_magick(set<string>& new_round, const set<string>& last_round, const set<rep>& transmutations)
{
    for (auto molecule: last_round)
    {
        for (auto replacement : transmutations)
        {
            replace_magic(new_round, molecule, replacement);
        }
    }
}

void transmutate(const string& start, const string& target, const set<rep>& transmutations, map<string, int>& save)
{
    save = map<string, int>{{start, 0}};
    int round_count {0};
    set<string> this_round {};
    set<string> prev_round {start};
    while(true)
    {
        ++round_count;
        round_of_magick(this_round, prev_round, transmutations);
        prev_round.clear();
        for (auto new_transmute : this_round)
        {
            if (save.contains(new_transmute)) continue;
            save.insert({new_transmute, round_count});
            prev_round.insert(new_transmute);
        }
        if (this_round.contains(target)) return;
        this_round.clear();
    }
}

// same as transmutate, but the other way around
int detransmutate(const string& start, const string& target, const set<rep>& transmutations)
{
    set<rep> r_transmut {};
    for (auto transmut : transmutations)
    {
        r_transmut.insert({transmut.second, transmut.first});
    }

    int round_count {0};
    set<string> this_round {};
    map<string, int> this_side_save {{start, 0}};
    set<string> this_side {start};
    map<string, int> the_other_side_save {{target, 0}};
    set<string> the_other_side {target};

    while(true)
    {
        ++round_count;
        std::cout << round_count << std::endl;
        round_of_magick(this_round, this_side, r_transmut);
        this_side.clear();
        for (auto new_transmute : this_round)
        {
            if (this_side_save.contains(new_transmute)) continue;
            if (the_other_side_save.contains(new_transmute)) return round_count + the_other_side_save[new_transmute];
            // this is an optimization for detransmutate function
            // there is no way to detransmutate an electon, so if there is more than one, we can forget about this string.
            // also no way to remove anything withou turning it into an electron
            if (new_transmute.find("e") != string::npos) {continue;}
            this_side_save.insert({new_transmute, round_count});
            this_side.insert(new_transmute);
        }
        this_round.clear();

        // now from the other side
        round_of_magick(this_round, the_other_side, transmutations);
        the_other_side.clear();
        for (auto new_transmute : this_round)
        {
            if (the_other_side_save.contains(new_transmute)) continue;
            if (this_side_save.contains(new_transmute)) return this_side_save[new_transmute] + round_count;
            the_other_side_save.insert({new_transmute, round_count});
            the_other_side.insert(new_transmute);
        }
        this_round.clear();
    }
}

int main()
{
    std::ifstream ifs;
    FileParsers::open_input(ifs);
    set<string> processed_molecules;

    set<rep> quantum_bazinga;
    string molecule;

    while(true)
    {
        string line;
        std::getline(ifs, line);
        if (line == "") break;

        std::string_view line_v {line};

        std::vector<string> words;
        for (auto i {0}; i < 3; ++i)
        {
            words.push_back(StringParsers::parse_word(line_v));
        }

        quantum_bazinga.insert({words[0], words[2]});
    }

    ifs >> molecule;

    // molecule = {"HOH"};
    // quantum_bazinga = {{"H", "HO"}, {"H", "OH"}, {"O", "HH"}, {"e", "H"}, {"e", "O"}};

    // // pt1
    // for (auto replacement : quantum_bazinga)
    // {
    //     replace_magic(processed_molecules, molecule, replacement);
    // }
    // for (auto mol : processed_molecules)
    // {
    //     std::cout << mol << std::endl;
    // }
    // std::cout << processed_molecules.size() << std::endl;

    // // pt2
    map<string, int> save;
    // this can, but won't work, coz amount of transmutations is horrendous
    // transmutate("e", molecule, quantum_bazinga, save);
    // std::cout << detransmutate(molecule, "e", quantum_bazinga) << std::endl;
    

    return EXIT_SUCCESS;
}
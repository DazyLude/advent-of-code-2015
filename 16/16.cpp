#include "src/parsers.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

enum class Stuff 
{
    children,
    cats,
    samoyeds,
    pomeranians,
    akitas,
    vizslas,
    goldfish,
    trees,
    cars,
    perfumes,
};

std::map<std::string, Stuff> Stuff_m {
    {"children", Stuff::children},
    {"cats", Stuff::cats},
    {"samoyeds", Stuff::samoyeds},
    {"pomeranians", Stuff::pomeranians},
    {"akitas", Stuff::akitas},
    {"vizslas", Stuff::vizslas},
    {"goldfish", Stuff::goldfish},
    {"trees", Stuff::trees},
    {"cars", Stuff::cars},
    {"perfumes", Stuff::perfumes},
};

struct AuntSue
{
    std::string name;
    std::map<Stuff, int> aunts_stuff;

    // one aunt can as well be an another aunt if ones stuff and anothers stuff intersection is nonconficting
    // which can be null
    // aunt that has no stuff can be any aunt
    bool can_be(AuntSue another)
    {
        if (aunts_stuff.empty()) return true;
        for (auto item : aunts_stuff)
        {
            if (another.aunts_stuff.contains(item.first) && item.second != another.aunts_stuff.at(item.first)) return false;
        }
        return true;
    }

    bool can_be_pt2(AuntSue another)
    {
        if (aunts_stuff.empty()) return true;
        for (auto item : aunts_stuff)
        {
            switch(item.first) {
            case Stuff::cats:
            case Stuff::trees:
                if (another.aunts_stuff.contains(item.first) && item.second >= another.aunts_stuff.at(item.first)) return false;
                break;
            case Stuff::pomeranians:
            case Stuff::goldfish:
                if (another.aunts_stuff.contains(item.first) && item.second <= another.aunts_stuff.at(item.first)) return false;
                break;
            default:
                if (another.aunts_stuff.contains(item.first) && item.second != another.aunts_stuff.at(item.first)) return false;
                break;
            }
        }
        return true;
    }

    AuntSue (std::string i_name, std::map<Stuff, int> i_aunts_stuff) : name(i_name), aunts_stuff(i_aunts_stuff)
    { }

    static AuntSue parse_sue(std::string& input)
    {
        using namespace StringParsers;
        // Sue 1: cars: 9, akitas: 3, goldfish: 0
        // 1 or more characters followed by a whitespace followed by  one or more digits, a colon and a whitespace
        // followed by 1 or more of +characters, colon, whitespace, +digits
        // separated by a comma and a whitespace
        std::string_view input_v {input};
        std::vector<std::string> words;
        std::map<Stuff, int> aunts_stuff {};

        auto parse_stuff = [&]()
        {
            trim_start(input_v);
            Stuff stuff = Stuff_m[parse_until(input_v, ':')];
            trim_start(input_v);
            auto count = std::stoi(parse_until(input_v, ','));
            return std::pair<Stuff, int> {stuff, count};
        };

        std::string name = parse_until(input_v, ':');
        while (!input_v.empty())
        {
            aunts_stuff.insert(parse_stuff());
        }
        return AuntSue(name, aunts_stuff);
    }
};

int main()
{
    std::ifstream ifs;
    FileParsers::open_input(ifs);

    AuntSue realsue {"Real One", 
        {
            {Stuff::children, 3},
            {Stuff::cats, 7},
            {Stuff::samoyeds, 2},
            {Stuff::pomeranians, 3},
            {Stuff::akitas, 0},
            {Stuff::vizslas, 0},
            {Stuff::goldfish, 5},
            {Stuff::trees, 3},
            {Stuff::cars, 2},
            {Stuff::perfumes, 1},
        }
    };
    
    while(true)
    {
        std::string line {};
        std::getline(ifs, line);
        if (realsue.can_be_pt2(AuntSue::parse_sue(line)))
        {
            std::cout << AuntSue::parse_sue(line).name << std::endl;
        }
        if (ifs.eof()) {break;}
    }
}
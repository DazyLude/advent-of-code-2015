#include <fstream>
#include <iostream>
#include <map>

using std::string;
using namespace std::literals;

void move(char command, std::pair<int, int>& cur_house) {
    switch(command) {
            case '<':
                --cur_house.first;
                break;
            case '>':
                ++cur_house.first;
                break;
            case 'v':
                --cur_house.second;
                break;
            case '^':
                ++cur_house.second;
                break;
            default:
                std::cerr << "Expected only <, >, ^ or v in input, but got " << command << "\n";
                break;
        }
}

void main() {
    // making an istream using an input file
    auto filename {"input"s};
    std::ifstream ifs {filename};
    if (!ifs) {
        std::cerr << "Couldn't open file \"input\"\n"s;
        return;
    }
    bool is_real_santa {true};
    std::pair<int, int> cur_house {0, 0};
    std::pair<int, int> cur_house_robot {0, 0};
    std::map<std::pair<int, int>, int> city_map;
    // adds a new house to the map, or gives another present to an existing one 
    auto give_present = [&city_map] (std::pair<int, int>& cur_house) {
        if (city_map.find(cur_house) == city_map.end()) 
            city_map.insert({cur_house, 1});
    };
    // reading input one symbol at a time

    while (true) {
        char cur = ifs.get();
        if (ifs.eof()) break;
        if (is_real_santa) {
            move(cur, cur_house);
            give_present(cur_house);
        } else {
            move(cur, cur_house_robot);
            give_present(cur_house_robot);
        }
        // comment the next line for the first solution
        is_real_santa =! is_real_santa;
    }
    std::cout << "Santa(s) visited " << city_map.size() << " houses\n";
}
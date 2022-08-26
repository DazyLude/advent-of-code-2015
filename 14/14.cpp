#include "src/parsers.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <algorithm>
#include <vector>
#include <string_view>

class Horse
{
public:
    const int speed;
    const int stamina;
    const int rest;

    int internal_clock {0};
    int distance {0};
    int points {0};

    void increment_clock()
    {
        if (internal_clock < stamina) {distance += speed;}
        internal_clock++;
        if (internal_clock == stamina + rest) {internal_clock = 0;}
    }
    
    int calc_distance(int time)
    {
        auto n_cycles = time / (stamina + rest);
        auto remaindeer = time % (stamina + rest);
        return (n_cycles * stamina + (remaindeer > stamina ? stamina : remaindeer)) * speed;
    }
};   


// Rudolph can fly 22 km/s for 8 seconds, but then must rest for 165 seconds.

Horse parse_deer(std::string &input)
{   
    std::string_view input_view {input};
    std::vector<std::string> words;
    for (auto i {0}; i < 14; ++i)
    {
        words.push_back(StringParsers::parse_word(input_view));
    }
    return {std::stoi(words[3]), std::stoi(words[6]), std::stoi(words[13])};
}

int main()
{
    std::ifstream ifs;
    FileParsers::open_input(ifs);

    std::vector<Horse> race;

    while(true)
    {
        std::string line {};
        std::getline(ifs, line);
        if (ifs.eof()) {break;}
        race.push_back(parse_deer(line));
    }
    
    for (int i = 0; i < 2503; ++i)
    {
        int max_dist = 0;
        for (auto p = 0; p < race.size(); ++p)
        {
            race[p].increment_clock();
            if (max_dist < race[p].distance) max_dist = race[p].distance;
        }
        for (auto p = 0; p < race.size(); ++p)
        {
            if (max_dist == race[p].distance) race[p].points ++;
        }

    }
    for (auto horse : race)
    {
        std::cout << horse.points << std::endl;
    }
}
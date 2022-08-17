// regex for this is "(\D+) (\d+),(\d+)\D+(\d+),(\d+)" -> [whole match, command, x1, y1, x2, y2]
// but what fun in doing it an easy way? let's do a parser-combinator :^)
// used in command parser
#include <string>
using std::string;
#include <iostream>
#include <cctype>
// used in the lights calculation
#include <array>
#include <vector>
// used in main
#include <fstream>

enum class WhatDo {
    toggle,
    on,
    off,
    err,
};

struct Command {
    std::pair<int, int> topleft;
    std::pair<int, int> bottomright;
    WhatDo action;
};

std::ostream& operator<<(std::ostream& os, const std::pair<int, int>& pair) {
    os << "(" << pair.first << ", " << pair.second << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const WhatDo act) {
    switch(act) {
        case WhatDo::on:
            os << "turn on the ligths";
            break;
        case WhatDo::off:
            os << "turn off the ligths";
            break;
        case WhatDo::toggle:
            os << "toggle the ligths";
            break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Command& comm) {
    os << comm.action << " from " << comm.topleft << " to " << comm.bottomright;
    return os;
}

// returns an action of enum class WhatDo
// Modifies the input string, cutting out parsed input + 1 character
// assumes that the string begins with a command 
// if can't identify a command, returns an err value of an enum
WhatDo parse_action(string& input) {
    switch(input[1]) {
        // catches "toggle"
        case 'o': 
            input = input.substr(7, input.size() - 7);
            return WhatDo::toggle;
        // catches "turn"
        case 'u':
            switch(input[6]) {
                // catches "turn on"
                case 'n':
                    input = input.substr(8, input.size() - 8);
                    return WhatDo::on;
                // catches "turn off"
                case 'f':
                    input = input.substr(9, input.size() - 9);
                    return WhatDo::off;
            }
        default:
            return WhatDo::err;
    }
}

// returns a single integer
// modifies the input string, cutting out parsed input + 1 character
// assumes that input starts with a decimal number
int parse_integer(string& input) {
    string integer;
    for (int i = 0; i < input.size(); ++i) {
        if (std::isdigit(input[i]) != 0) {
            integer.push_back(input[i]);
        } else {
            break;
        }
    }
    input = input.substr(integer.size() + 1, input.size() - integer.size() - 1);
    return std::stoi(integer);
}

// returns a pair of coordinates
// Modifies the input string, cutting out parsed input + 1 character
// assumes that input starts with <int>,<int>...
std::pair<int, int> parse_coordinates(string& input) {
    int first, second;
    first = parse_integer(input);
    second = parse_integer(input);
    return {first, second};
}

// modifies input string, removing anything that is not a digit from the beginning
void trim_left(string& input) {
    int cut {0};
    for (int i = 0; i < input.size(); ++i) {
        if (std::isdigit(input[i]) != 0) {
            break;
        } else {
            ++cut;
        }
    }
    input = input.substr(cut, input.size() - cut);
}

// parses a string of type "command digits+,digits+ sometext digits+,digits+" to a command structure
Command parse_command(string input) {
    Command do_this;
    do_this.action = parse_action(input);
    do_this.topleft = parse_coordinates(input);
    trim_left(input);
    do_this.bottomright = parse_coordinates(input);
    return do_this;
}

// a model of a field with a million lights
// two dimensional array of bools

#include <memory>

struct Field {
    std::vector<std::vector<int>> lights;

    Field() {
        lights = std::vector<std::vector<int>> (1000, std::vector<int> (1000, 0));
    }

    void apply_command(Command do_this) {
        for (int i = do_this.topleft.first; i <= do_this.bottomright.first; ++i) {
            for (int j = do_this.topleft.second; j <= do_this.bottomright.second; ++j) {
                switch(do_this.action) {
                    case WhatDo::off:
                        // lights.at(i).at(j) = 0;
                        if (lights.at(i).at(j) > 0) --lights.at(i).at(j);
                        break;
                    case WhatDo::on:
                        // lights.at(i).at(j) = 1;
                        ++lights.at(i).at(j);
                        break;
                    case WhatDo::toggle:
                        // lights.at(i).at(j) = (lights.at(i).at(j) == 0) ? 1 : 0;
                        lights.at(i).at(j) += 2;
                        break;
                }
            }
        }
    }
    

    int count_lights() {
        int count {0};
        for (int i = 0; i < 1000; ++i) {
            for (int j = 0; j < 1000; ++j) {
                count += lights.at(i).at(j);
            }
        }
        return count;
    }
};


int main() {
    auto filename {"input"};
    std::ifstream ifs {filename};
    if (!ifs) {
        std::cerr << "Couldn't open file \"input\"\n";
        return 1;
    }

    std::vector<Command> command_list;

    while(true) {
        std::string word;
        std::getline(ifs, word);
        if (word.size() == 0) break;
        word.push_back(' ');
        command_list.push_back(parse_command(word));
        // std::cout << command_list.back();
        if (ifs.eof()) break;
    }
    
    Field field;
    // field.apply_command(parse_command("turn on 000,000 through 999,999 "));

    for (auto command : command_list) {
        field.apply_command(command);
    }
    std::cout << "There are " << field.count_lights() << " lights";

    return 0;
}

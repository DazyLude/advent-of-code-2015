#include <string>
#include <iostream>
#include <fstream>

#include <sstream>

// escaped characters are: \\, \", \x__
// formats a string like string constructor does with string literals
// returns formatted string, the original is unchanged
// we can just count characters to get an answer
#include <optional>
std::optional<int> characters_escape_route(const std::string& input) {
    if (input.front() != '"' || input.back() != '"') {
        std::cerr << "wrong string literal: " << input << std::endl;
        return {};
    }
    auto count {0}; 
    for (auto ch = input.begin(); ch != input.end(); ++ch) {
        if(*ch == '\\') {
            if(*++ch == 'x') {
                ch += 2;                   
            }
        }
        ++count;
    }
    return count - 2;
}

std::optional<int> characters_trap_route(const std::string& input) {
    if (input.front() != '"' || input.back() != '"') {
        std::cerr << "wrong string literal: " << input << std::endl;
        return {};
    }
    auto count {0}; 
    for (auto ch = input.begin(); ch != input.end(); ++ch) {
        if(*ch == '\\' || *ch == '\"') {
            ++count;
        }
        ++count;
    }
    return count + 2;
}

int main() {
    auto filename {"input"};
    std::ifstream ifs {filename};
    if (!ifs) {
        std::cerr << "Couldn't open file \"input\"\n";
        return 1;
    }
    int count {0};
    while(!ifs.eof()) {
        std::string santas_string;
        ifs >> santas_string;
        if (santas_string.empty()) break;
        // part1
        // count += santas_string.size();
        // count -= characters_escape_route(santas_string).value_or(0);
        // part2
        count += characters_trap_route(santas_string).value_or(0);
        count -= santas_string.size();
    }
    std::cout << count << std::endl;
}
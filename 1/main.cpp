#include <fstream>
#include <iostream>

using std::string;
using namespace std::literals;

void main() {
    auto filename {"input"s};
    std::ifstream ifs {filename};
    if (!ifs) 
        std::cerr << "Couldn't open file \"input\"\n"s;
    else {
        auto floor = 0;
        auto instruction_pos = 0;
        while (true) {
            char cur = ifs.get();
            if (ifs.eof()) break;
            switch(cur) {
                case '(': 
                    ++floor;
                    break;
                case ')':
                    --floor;
                    break;
                default:
                    std::cerr << "Expected only \"(\" or \")\" in input, but got " << cur << "\n";
            }
            ++instruction_pos;
            if (floor == -1) std::cout << "instruction to enter the basement is at: " << instruction_pos << "\n";
        } 
        std::cout << "floor is: " << floor << "\n";
    }
}

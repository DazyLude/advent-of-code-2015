// long-ear green bastards are up to something once again

#include <fstream>
#include <iostream>
#include <regex>

using std::string;
using namespace std::literals;

int calculate_wrapping(int, int, int);
int calculate_ribbon(int, int, int);

void main() {
    auto filename {"input"s};
    std::ifstream ifs {filename};
    if (!ifs) {
        std::cerr << "Couldn't open file \"input\"\n"s;
        return;
    }
    
    int l, w, h;
    auto total_wrapping {0};
    auto total_ribbon {0};
    std::regex lxwxh {R"((\d+)x(\d+)x(\d+))"};
    std::smatch matches;
    string line;

    while(true) {
        std::getline(ifs, line);
        if (ifs.eof()) break;
        std::regex_search(line, matches, lxwxh);
        l = std::stoi(matches[1]);
        w = std::stoi(matches[2]);
        h = std::stoi(matches[3]);
        total_wrapping += calculate_wrapping(l, w, h);
        total_ribbon += calculate_ribbon(l, w, h);
    }
    std::cout << "total wrapping surface is: " << total_wrapping << "\n";
    std::cout << "total ribbon length is: " << total_ribbon << "\n";
}

int calculate_wrapping(int l, int w, int h) {
    auto top {l * w};
    auto front {h * w};
    auto side {l * h};
    return (top + front + side) * 2 + std::min({top, front, side});
}

int calculate_ribbon(int l, int w, int h) {
    auto volume = l * w * h;
    return (l + w + h - std::max({l, w, h})) * 2 + volume;
}
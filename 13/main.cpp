#include "src/graph.h"
#include "src/parsers.h"


#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <set>

// parses lines that look as follows:
// Alice would lose 57 happiness units by sitting next to Bob.
// adding an undirected edge to a graph with cost of -57
// between nodes with id's 'Alice' and 'Bob'
void add_relation(std::string& input, Graphomania::Graph<std::string, int>& graph)
{
    input.pop_back();
    std::string_view input_v {input};
    std::vector<std::string> words {};
    for (auto i {0}; i < 11; ++i) {
        words.push_back(StringParsers::parse_word(input_v));
    }

    int cost = std::stoi(words[3]); 
    graph.add_directed_edge(words[0], words[10], (words[2][0] == 'g' ? 1 : -1) * cost);
}

int main()
{
    std::ifstream ifs;
    FileParsers::open_input(ifs);
    Graphomania::Graph<std::string, int> friendship {};

    while(true)
    {
        std::string line {};
        std::getline(ifs, line);
        if (ifs.eof()) {break;}
        add_relation(line, friendship);
    }
    
    std::cout << RoundTableProblem::calc_seating(friendship);

}
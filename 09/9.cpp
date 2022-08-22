// this is traveling merchant problem
// bruteforce is O(N!), but we can (possibly) do better by using branch and bound
// also trying out c++20 standard with module system and stuff


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

import GraphAndTree;

int main() {
    Graph graph;
    //std::istringstream test_input{ "London to Dublin = 464\nLondon to Belfast = 518\nDublin to Belfast = 141\n" };
    auto filename{ "input" };
    std::ifstream ifs{ filename };
    if (!ifs) {
        std::cerr << "Couldn't open file \"input\"\n";
        return 1;
    }
    while(true)
    {
        std::string temp{};
        std::getline(ifs, temp);
        if (temp.size() == 0) break;

        std::cout << temp << std::endl;
        graph.parse_5_words(temp);
    }
    for (auto node : graph.get_nodes()) {
        std::cout << node << std::endl;
    }

    int bound = graph.get_lower_bound();
    std::cout << bound;
    std::cout << TSP::branch_and_prune_start(graph, bound);
    return 0;
}
export module GraphAndTree:Graph;

import <string>;
import <vector>;
import <map>;
import <set>;

import Parser;

using std::string;
using std::vector;
using std::map;
using std::set;

export class Graph {
    // map<node_id map<route_to, cost>>
    map<string, map<string, int>> nodes;

    void add_empty_node(string id) {
        nodes.insert({ id, {} });
    }

    void add_connection(string id_1, string id_2, int cost) {
        nodes.at(id_1).insert({ id_2, cost });
        nodes.at(id_2).insert({ id_1, cost });
    }
public:
    set<string> get_nodes() {
        set<string> result;
        for (auto node : nodes) {
            result.insert(node.first);
        }
        return result;
    }

    set<string> get_routes(string id) {
        set<string> result;
        for (auto node : nodes.at(id)) {
            result.insert(node.first);
        }
        return result;
    }

    string get_cheapest_destination(string from, set<string> restriction = {}) {
        for (auto route : nodes.at(from)) {
            if (!restriction.contains(route.first)) return route.first;
        }
        return "";
    }

    int get_lower_bound() {
        int bound{};
        for (auto node : nodes) {
            string cheapest = get_cheapest_destination(node.first);
            bound += node.second.at(cheapest);
            bound += node.second.at(get_cheapest_destination(node.first, { cheapest }));
        }
        return bound;
    }

    int get_cost(string from, string to) {
        return nodes.at(from).at(to);
    }

    // parses inputs x to y = cost
    void parse_5_words(string& input) {
        string commands[5];
        for (auto i = 0; i < 5; ++i) {
            commands[i] = parse_word(input);
        }
        if (!nodes.contains(commands[0])) add_empty_node(commands[0]);
        if (!nodes.contains(commands[2])) add_empty_node(commands[2]);
        add_connection(commands[0], commands[2], std::stoi(commands[4]));
    }
}; // class Graph
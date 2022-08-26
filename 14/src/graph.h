#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <concepts>

namespace Graphomania
{
    using std::string;
    using std::vector;
    using std::map;
    using std::set;

    template <class id_t, class cost_t>
    class Graph
    {
        map<id_t, map<id_t, cost_t>> nodes;
        cost_t cost_modifier {cost_t(0)};

        void unsafe_add_edge(id_t id_1, id_t id_2, cost_t cost);
        void unsafe_add_directed_edge(id_t from, id_t to, cost_t cost);
        
    public:
        set<id_t> get_nodes();
        set<id_t> get_nodes_but(set<id_t>);
        set<id_t> get_routes(id_t id);
        cost_t get_cost(id_t from, id_t to);
        cost_t get_cost_mod();

        void add_empty_node(id_t id);
        void add_edge(id_t id1, id_t id2, cost_t cost);
        void add_directed_edge(id_t id1, id_t id2, cost_t cost);

        id_t get_cheapest_destination(id_t from, set<id_t> restriction);
        id_t get_expensive(id_t from, set<id_t> restriction);

        cost_t get_lower_bound();
        
    }; // class Graph
} // namespace Graphomania

// definition of templated Graph methods
namespace Graphomania 
{
    // private methods
    template <class id_t, class cost_t>
    void Graph<id_t, cost_t>::unsafe_add_edge(id_t id_1, id_t id_2, cost_t cost) {
        nodes.at(id_1).insert({ id_2, cost });
        nodes.at(id_2).insert({ id_1, cost });
        if (cost < -cost_modifier) {cost_modifier = -cost;}
    } 

    template <class id_t, class cost_t>
    void Graph<id_t, cost_t>::unsafe_add_directed_edge(id_t id_from, id_t id_to, cost_t cost) {
        nodes.at(id_from).insert({ id_to, cost });
        if (cost < cost_modifier) {cost = cost_modifier;}
    }

    // public methods 
    // simple getters
    template <class id_t, class cost_t>
    set<id_t> Graph<id_t, cost_t>::get_nodes() {
        set<id_t> result;
        for (auto node : nodes) {
            result.insert(node.first);
        }
        return result;
    }

    template <class id_t, class cost_t>
    set<id_t> Graph<id_t, cost_t>::get_nodes_but(std::set<id_t> exc) {
        set<id_t> result;
        for (auto node : nodes) {
            if (exc.contains(node.first)) continue;
            result.insert(node.first);
        }
        return result;
    }

    template <class id_t, class cost_t>
    set<id_t> Graph<id_t, cost_t>::get_routes(id_t id) {
        set<id_t> result;
        for (auto node : nodes.at(id)) {
            result.insert(node.first);
        }
        return result;
    }

    template <class id_t, class cost_t>
    cost_t Graph<id_t, cost_t>::get_cost(id_t from, id_t to) {
        return nodes.at(from).at(to);
    }

    template <class id_t, class cost_t>
    cost_t Graph<id_t, cost_t>::get_cost_mod() {
        return cost_modifier;
    }

    // setters    
    template <class id_t, class cost_t>
    void Graph<id_t, cost_t>::add_edge(id_t id1, id_t id2, cost_t cost) {
        if (!nodes.contains(id1)) add_empty_node(id1);
        if (!nodes.contains(id2)) add_empty_node(id2);
        unsafe_add_edge(id1, id2, cost);
    }

    template <class id_t, class cost_t>
    void Graph<id_t, cost_t>::add_directed_edge(id_t id1, id_t id2, cost_t cost) {
        if (!nodes.contains(id1)) add_empty_node(id1);
        if (!nodes.contains(id2)) add_empty_node(id2);
        unsafe_add_directed_edge(id1, id2, cost);
    }

    template <class id_t, class cost_t>
    void Graph<id_t, cost_t>::add_empty_node(id_t id) {
        nodes.insert({ id, {} });
    }

    // specific getters
    template <class id_t, class cost_t>
    id_t Graph<id_t, cost_t>::get_cheapest_destination(id_t from, set<id_t> restriction) {
        for (auto route : nodes.at(from)) {
            if (!restriction.contains(route.first)) return route.first;
        }
        return id_t();
    }

    template <class id_t, class cost_t>
    cost_t Graph<id_t, cost_t>::get_lower_bound() {
        int bound{};
        for (auto node : nodes) {
            string cheapest = get_cheapest_destination(node.first, {});
            bound += node.second.at(cheapest);
            bound += node.second.at(get_cheapest_destination(node.first, { cheapest }));
        }
        return bound;
    }

} // namespace Graphomania

namespace TSP {
    
    struct Solution {
		std::vector<std::string> route;
		int cost;

		Solution add_node(std::string id, int cost);
	};
    
    std::ostream& operator<<(std::ostream& os, const Solution sol);

    Solution branch_and_bound_start(Graphomania::Graph<std::string, int>& gr, int& bound);
    Solution branch_and_bound_continue(Solution route_before, Graphomania::Graph<std::string, int>& gr, int& bound);
}

namespace RoundTableProblem {
    using std::string;

    int calc_friendship(std::vector<string> order, Graphomania::Graph<string, int>& gr);


    int calc_seating(Graphomania::Graph<string, int>& gr);
}
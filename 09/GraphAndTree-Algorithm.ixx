export module GraphAndTree:Algorithm;

import :Graph;

import <vector>;
import <string>;
import <set>;
import <memory>;
import <iostream>;

// Traveling salesman problem
// There is a graph with cities and roads
// find cheapes solutution
// Bruteforce:
// check all possible routes, select cheapest
export namespace TSP {
	using std::vector;
	using std::string;
	using std::set;

	struct Solution {
		vector<string> route;
		int cost;

		Solution add_node(string id, int cost) {
			Solution updated (*this);
			updated.route.push_back(id);
			updated.cost += cost;
			return updated;
		}
	};

	std::ostream& operator<<(std::ostream& os, const Solution sol) {
		for (auto node : sol.route) {
			os << node << " -> ";
		}
		os << "total: " << sol.cost << std::endl;
		return os;
	}

	Solution bruteforce_continue(Solution route_before, Graph& world) {
		set<string> restricted;
		for (auto entry : route_before.route) {
			restricted.insert(entry);
		}
		string& last_node = route_before.route.back();
		string next_destination = world.get_cheapest_destination(last_node, restricted);
		vector<Solution> exploration_results;
		// fun part
		while (true) {
			next_destination = world.get_cheapest_destination(last_node, restricted);
			if (next_destination == "") break;
			exploration_results.push_back(
				bruteforce_continue(
					route_before.add_node(
						next_destination,
						world.get_cost(last_node, next_destination)),
					world)
			);
			restricted.insert(next_destination);
		}
		// if next destination is unavailable, that means that means that we've exhausted this route
		if (exploration_results.empty()) {
			return route_before;
		}
		// comparing recieved solutions
		Solution longest = exploration_results[0];
		for (auto i = 1; i < exploration_results.size(); ++i) {
			if (longest.cost < exploration_results[i].cost) {
				longest = exploration_results[i];
			}
		}
		return longest;
	}

	Solution bruteforce_start(Graph& world) {
		vector<Solution> exploration_results;
		// same as bruteforce_continue, but choosing starting node out of all available
		for (auto starting_node : world.get_nodes()) {
			exploration_results.push_back(
				bruteforce_continue(Solution{ {starting_node}, 0 }, world)
			);
		}
		// comparing recieved solutions
		Solution longest = exploration_results[0];
		for (auto i = 1; i < exploration_results.size(); ++i) {
			if (longest.cost < exploration_results[i].cost) {
				longest = exploration_results[i];
			}
		}
		return longest;
	}

	Solution branch_and_prune_continue(Solution route_before, Graph& world, int& bound);

	Solution branch_and_prune_start(Graph& world, int& bound) {
		vector<Solution> exploration_results;
		// same as bruteforce_continue, but choosing starting node out of all available
		for (auto starting_node : world.get_nodes()) {
			exploration_results.push_back(
				branch_and_prune_continue(Solution{ {starting_node}, 0 }, world, bound)
			);
		}
		// comparing recieved solutions
		Solution best = exploration_results[0];
		for (auto i = 1; i < exploration_results.size(); ++i) {
			if (best.cost > exploration_results[i].cost) {
				best = exploration_results[i];
			}
		}
		return best;
	}
	
	Solution branch_and_prune_continue(Solution route_before, Graph& world, int& bound) {
		// prune this route if it exceeds i_bound
		if (route_before.cost >= bound) {
			std::cout << "pruned " << route_before << std::endl;
			return route_before;
		}		
		set<string> restricted;
		for (auto entry : route_before.route) {
			restricted.insert(entry);
		}
		string& last_node = route_before.route.back();
		string next_destination = world.get_cheapest_destination(last_node, restricted);
		vector<Solution> exploration_results;
		// fun part
		while (true) {
			next_destination = world.get_cheapest_destination(last_node, restricted);
			if (next_destination == "") break;
			exploration_results.push_back(
				branch_and_prune_continue(
					route_before.add_node(
						next_destination,
						world.get_cost(last_node, next_destination)),
					world,
					bound)
			);
			restricted.insert(next_destination);
		}
		// if next destination is unavailable, that means that means that we've exhausted this route
		// in case of branch and prune, we also can update bound.
		if (exploration_results.empty()) {
			if (route_before.cost < bound) bound = route_before.cost;
			return route_before;
		}
		// comparing recieved solutions
		Solution best = exploration_results[0];
		for (auto i = 1; i < exploration_results.size(); ++i) {
			if (best.cost > exploration_results[i].cost) {
				best = exploration_results[i];
			}
		}
		return best;
	}
}

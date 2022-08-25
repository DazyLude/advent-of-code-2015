#include "graph.h"
#include <algorithm>
#include <map>
#include <compare>

namespace TSP {
	using std::vector;
	using std::string;
	using std::set;
    using Graphomania::Graph;

	Solution Solution::add_node(std::string id, int cost) {
		Solution updated (*this);
		updated.route.push_back(id);
		updated.cost += cost;
		return updated;
	}

	std::ostream& operator<<(std::ostream& os, const Solution sol) {
		for (auto node : sol.route) {
			os << node << " -> ";
		}
		os << "total: " << sol.cost << std::endl;
		return os;
	}

	Solution branch_and_bound_start(Graph<string, int>& gr, int& bound)
    {
		vector<Solution> exploration_results;
		// same as continue, but choosing starting node out of all available
		for (auto starting_node : gr.get_nodes()) {
			exploration_results.push_back(
				branch_and_bound_continue(Solution{ {starting_node}, 0 }, gr, bound)
			);
		}
		// comparing recieved solutions
		Solution best = exploration_results[0];
		for (auto i : exploration_results) {
			if (i.cost < best.cost) best = i;
		}
		best.cost = best.cost - best.route.size() * gr.get_cost_mod();

		return best;
	}
	
	Solution branch_and_bound_continue(Solution route_before, Graph<string, int>& gr, int& bound)
    {
		// prune this route if it exceeds bound
		if (bound <= route_before.cost) {
			// std::cout << "pruned " << route_before << std::endl;
			return route_before;
		}		
		set<string> restricted;
		for (auto entry : route_before.route) {
			restricted.insert(entry);
		}
		string& last_node = route_before.route.back();
		string next_destination = gr.get_cheapest_destination(last_node, restricted);
		vector<Solution> exploration_results;
		// fun part
		while (true) {
			next_destination = gr.get_cheapest_destination(last_node, restricted);
			if (next_destination == "") break;
			exploration_results.push_back(
				branch_and_bound_continue(
					route_before.add_node(
						next_destination,
						gr.get_cost(last_node, next_destination)),
					gr,
					bound)
			);
			restricted.insert(next_destination);
		}
		// if next destination is unavailable, that means that means that we've exhausted this route
		// in case of branch and prune, we also can update bound.
		if (exploration_results.empty()) {
			if (bound > route_before.cost)
			{
				bound = route_before.cost;
			    std::cout << "new bound: " << bound << std::endl;
			}
			return route_before;
		}
		// comparing recieved solutions
		Solution best = exploration_results[0];
		for (auto i : exploration_results) {
			if (i.cost < best.cost) best = i;
		}
		return best;
	}
} // TSP

namespace RoundTableProblem {
	using std::vector;
	using std::string;
	using std::set;
    using Graphomania::Graph;

	typedef std::set<std::pair<int, vector<string>>, std::greater<std::pair<int, vector<string>>>> tbl_set;

	int rel(Graph<string, int>& gr, string person1, string person2)
	{
		return gr.get_cost(person1, person2) + gr.get_cost(person2, person1);
	}

	int third_wheel(Graph<string, int>& gr, string person1, string person2, string person3)
	{
		return rel(gr, person1, person3) + rel(gr, person2, person3) - rel(gr, person1, person2);
	}

	int max_impact_combos(int n, int depth)
	{
		if (depth == 0 || n == 2) return 1;
		int prev = max_impact_combos(n - 1, depth - 1);
		return prev + (n - 1)*(n - 2);
	}

	int factor(int n)
	{
		if (n == 1) return 1;
		return n * factor(n - 1);
	}

	int seating_combos(int n)
	{
		return factor(n - 1) / 2;
	}

	int calc_friendship(std::vector<string> seat, Graph<string, int>& gr)
	{
		int count {rel(gr, seat[0], seat.back())};
		for (auto i {0}; i < seat.size() - 1; ++i)
		{
			count += rel(gr, seat[i], seat[i+1]);
		}
		return count;
	}

	
	

	int max_impact(Graph<string, int>& gr, string person, set<string> except)
	{
		auto check {gr.get_nodes()};
		check.erase(person);
		for (auto person : except) 
		{
			check.erase(person);
		}
		int max {0};
		for (auto person1 = check.begin(); person1 != --check.end(); ++person1) {
			for (auto person2 = person1; ++person2 != check.end();) {
				int rd = third_wheel(gr, *person1, *person2, person);
				if (rd > max) max = rd;
			}
		}
		return max;
	}	

	std::vector<string> calc_next_round(Graph<string, int>& gr, std::vector<string> seat)
	{
		return seat;
	}

	int calc_seating(Graph<string, int>& gr)
	{
		auto people = gr.get_nodes();
		auto n_peep = people.size();
		std::vector<std::pair<int, string>> impact_rating;
		for (string person : people)
		{
			impact_rating.push_back({max_impact(gr, person, {}), person});
		}
		std::sort(impact_rating.begin(), impact_rating.end());
		for (auto rating : impact_rating)
		{
			std::cout << rating.second << ": " << rating.first << std::endl;
		}
		vector<string> starting_table;
		
		for (auto i {0}; i < 3; ++i)
		{
			starting_table.push_back(impact_rating.back().second);
			impact_rating.pop_back();
		}
		vector<tbl_set> table_sets;
		table_sets.push_back({{calc_friendship(starting_table, gr), starting_table}});

		auto take_a_seat = [&table_sets, &gr](vector<string> table, string who, int pos)
		{
			vector<string> temp {};
			for (int i = 0; i < table.size(); ++i)
			{
				if (i == pos) temp.push_back(who);
				temp.push_back(table[i]);
			}
			table_sets.back().insert({calc_friendship(temp, gr), temp});
		};

		auto rest_max_impact = [&impact_rating]()
		{
			auto count {0};
			for (auto rating : impact_rating)
			{
				count += rating.first;
			}
			return count;
		};

		// generate a table set with a new person
		// remove added person from the leaderboard
		// remove tables from a set that are too far behind
		for (int r {0}; r < n_peep - 3; ++r)
		{
			tbl_set last_round = table_sets.back();
			
			table_sets.push_back({});

			for (auto old_set = last_round.begin(); old_set != last_round.end(); ++old_set)
			{
				for (auto i {0}; i < 3 + r; i++)
				{
					take_a_seat(old_set->second, impact_rating.back().second, i);
				}
			}

			impact_rating.pop_back();

			// int highest = table_sets.back().begin()->first;
			// while((highest - table_sets.back().rbegin()->first) > rest_max_impact())
			// {
			// 	table_sets.back().erase(*table_sets.back().rbegin());
			// }
		}
		

		

		return table_sets.back().begin()->first;
	}

	
} // RoundTableProblem

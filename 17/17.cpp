#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <numeric>

// key: index, bool: y/n
typedef std::vector<bool> container_choice;
// testing
// std::vector<int> available {20, 15, 10, 5, 5};
std::vector<int> available {11, 30, 47, 31, 32, 36, 3, 1, 5, 3, 32, 36, 15, 11, 46, 26, 28, 1, 19, 3};
std::vector<int> left {available};

int sum_of_cc(container_choice cc)
{
    int count {}; 
    for (auto i {0}; i < cc.size(); ++i)
    {
        if (cc[i]) count += available[i];
    }
    return count;
}

int cc_count(container_choice cc)
{
    int count {}; 
    for (auto i {0}; i < cc.size(); ++i)
    {
        if (cc[i]) ++count;
    }
    return count;
}


int choose_next(container_choice cc, int target)
{   
    // exiting the recursion
    int current_sum = sum_of_cc(cc);
    // if we've reached target - this choice of containers is correct
    if (current_sum == target) return 1;
    // if we've made a choice for every container, but target is not reached - we didn't make it
    if (cc.size() == available.size()) return 0;
    // if we overshoot, the route is dead
    if (current_sum > target) return 0;
    // if we won't be able to get enough volume with the current choice of containers, this route is *dead*
    if (current_sum + left[cc.size() - 1] < target) return 0; 

    int count_rest {0};
    auto temp = cc; 
    temp.push_back(false);
    count_rest += choose_next(temp, target);
    temp.back() = true;
    count_rest += choose_next(temp, target);

    return count_rest;
}

// same as before, but has a calculated container bound.
int choose_next_pt2(container_choice cc, int target, int container_bound)
{   
    // exiting the recursion
    int current_sum = sum_of_cc(cc);
    // if we've reached target - this choice of containers is correct
    if (current_sum == target) return 1;
    // if we've chosen more containers than we can, we should stop
    if (cc_count(cc) >= container_bound) return 0;
    // if we overshoot, the route is dead
    if (current_sum > target) return 0;
    // if we won't be able to get enough volume with the current choice of containers, this route is dead
    if (current_sum + left[cc.size() - 1] < target) return 0; 

    int count_rest {0};
    auto temp = cc; 
    temp.push_back(false);
    count_rest += choose_next_pt2(temp, target, container_bound);
    temp.back() = true;
    count_rest += choose_next_pt2(temp, target, container_bound);

    return count_rest;
}

// same as choose next, but returns minimal amount of containers
int find_min_size(container_choice cc, int target)
{
    // exiting the recursion
    int current_sum = sum_of_cc(cc);
    // if we've reached target - this choice of containers is correct, and we return the size
    if (current_sum == target) return cc_count(cc);
    // if we've made a choice for every container, but target is not reached - we didn't make it
    if (cc.size() == available.size()) return 0;
    // if we overshoot, the route is dead
    if (current_sum > target) return 0;
    // if we won't be able to get enough volume with the current choice of containers, this route is *dead*
    if (current_sum + left[cc.size() - 1] < target) return 0; 

    int min_size {0};
    auto temp = cc; 
    int temp_i {0};

    temp.push_back(false);
    temp_i = find_min_size(temp, target);
    if (temp_i != 0) min_size = temp_i;

    temp.back() = true;
    temp_i = find_min_size(temp, target);
    if (temp_i != 0) min_size = (min_size == 0 ? temp_i : std::min({temp_i, min_size}));

    return min_size;
}

int main()
{
    int total = std::accumulate(available.begin(), available.end(), 0);

    std::sort(available.rbegin(), available.rend());

    std::partial_sum(available.begin(), available.end(), left.begin());
    std::transform(left.begin(), left.end(), left.begin(), [total](int n){return total - n;});

    int min_size_tr, min_size_fa, min_size;
    int target {150};

    min_size_tr = find_min_size({true}, target);
    min_size_fa = find_min_size({false}, target);
    if (min_size_fa * min_size_tr == 0)
    {
        min_size = min_size_tr == 0 ? min_size_fa : min_size_tr;
    }
    else min_size = std::min({min_size_fa, min_size_tr});

    int count {0};
    count += choose_next_pt2({true}, target, min_size);
    count += choose_next_pt2({false}, target, min_size);

    // count += choose_next({false}, 150);
    // count += choose_next({true}, 150);

    std::cout << min_size << " " << count;
}
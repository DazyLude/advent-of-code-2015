#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <string>

using std::vector;
using std::set;

std::ostream& operator<<(std::ostream& os, const set<int>& vec)
{
    os << "( ";
    for (auto elem : vec)
    {
        os << elem << " "; 
    }
    os << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const vector<int>& vec)
{
    os << "( ";
    for (auto elem : vec)
    {
        os << elem << " "; 
    }
    os << ")";
    return os;
}



class Combo
{
    vector<int> packages;
    vector<int> package_ind;

    long long int wombo_qe(set<int> wombo)
    {
        long long int quantum_entanglement {1};
        for (int n : wombo)
        {
            quantum_entanglement *= packages[n];
        }
        return quantum_entanglement;
    }

    float f_wombo_qe(set<int> wombo)
    {
        float quantum_entanglement {1.};
        for (int n : wombo)
        {
            quantum_entanglement *= packages[n];
        }
        return quantum_entanglement;
    }

    int wombo_sum(set<int> wombo)
    {
        int sum {};
        for (int n : wombo)
        {
            sum += packages[n];
        }
        return sum;
    }


    inline static auto set_cmp = [](set<int> st, set<int> nd)
    {
        if (st.size() != nd.size()) return st.size() < nd.size();
        return (st < nd);
    };

    set<set<int>, decltype(set_cmp)> wombos;

    bool check_complementarness(set<int>& st, set<int>& nd, set<int>& rd)
    {
        // pseudo-completeness, if there are repeats can be true for incomplete
        // but can't be incomplete if there are no repeats
        if (st.size() + nd.size() + rd.size() != packages.size()) return false;
        // check for no repeats
        for (auto st_obj : st)
        {
            if (nd.count(st_obj) != 0) return false;
            if (rd.count(st_obj) != 0) return false;
        }
        for (auto nd_obj : nd)
        {
            if (rd.count(nd_obj) != 0) return false;
        }
        return true;
    }

    bool find_complement(set<int>& wombo)
    {
        for(auto nd_wombo: wombos)
        {
            // since wombos are sorted similarly, we can exclude same ones by checking for the first element;
            // this also excludes some other wombo pairs not complementary because of overlap;
            if (*nd_wombo.begin() == *wombo.begin()) continue;
            for (auto rd_wombo: wombos)
            {
                if (*nd_wombo.begin() == *rd_wombo.begin() || *wombo.begin() == *rd_wombo.begin()) continue;
                if (check_complementarness(wombo, nd_wombo, rd_wombo)) return true;
            }
        }
        return false;
    }

    set<int> generate_compl(set<int> subset, int sum)
    {
        vector<int> package_ind_subset {};
        for (auto i : package_ind)
        {
            if (subset.count(i) == 0) package_ind_subset.push_back(i);
        }

        set<std::pair<set<int>, int>> possible_prev{{{}, 0}};
        set<std::pair<set<int>, int>> possible_new{};
        while (!possible_prev.empty()) //until we can try to add something new
        {
            for (int i : package_ind_subset) // for every package
            {
                for (auto poss : possible_prev) // for every possible subset
                {
                    if (poss.first.count(i) != 0) continue;
                    int poss_sum = poss.second + packages[i];
                    if (poss_sum > sum) continue;
                    poss.first.insert(i);
                    if (poss_sum == sum)
                    {
                        return poss.first;
                    }
                    poss.second = poss_sum;
                    possible_new.insert(poss);
                }
            }
            possible_prev = possible_new;
            possible_new.clear();
        }
        return {};
    }

    set<int> generate_compl(set<int> subset1, set<int> subset2, int sum)
    {
        set<int> subset {};
        for (auto s: subset1)
        {
            subset.insert(s);
        }
        for (auto s: subset2)
        {
            subset.insert(s);
        }
        return generate_compl(subset, sum);
    }

    set<int> generate_compl(set<int> subset1, set<int> subset2)
    {
        set<int> package_ind_subset {};
        for (auto i : package_ind)
        {
            if ((subset1.count(i) | subset2.count(i)) == 0) package_ind_subset.insert(i);
        }
        return package_ind_subset;
    }

    set<int> generate_compl(set<int> subset1, set<int> subset2, set<int> subset3)
    {
        set<int> subset {};
        for (auto s: subset1)
        {
            subset.insert(s);
        }
        for (auto s: subset2)
        {
            subset.insert(s);
        }
        return generate_compl(subset, subset3);
    }

public:
    Combo(vector<int>& ivec, int sum) : packages(ivec)
    {
        set<std::pair<set<int>, int>> possible_prev{{{}, 0}};
        set<std::pair<set<int>, int>> possible_new{};
        while (!possible_prev.empty()) //until we can try to add something new
        {
            for (int i = 0; i < packages.size(); ++i) // for every package
            {
                for (auto poss : possible_prev) // for every possible subset
                {
                    if (poss.first.count(i) != 0) continue;
                    int poss_sum = poss.second + packages[i];
                    if (poss_sum > sum) continue;
                    poss.first.insert(i);
                    if (poss_sum == sum)
                    {
                        wombos.insert(poss.first);
                        continue;
                    }
                    poss.second = poss_sum;
                    possible_new.insert(poss);
                }
            }
            possible_prev = possible_new;
            possible_new.clear();
        }
    }
    
    Combo(vector<int>& ivec) : packages(ivec)
    {
        for (int i = 0; i < packages.size(); ++i) package_ind.push_back(i);
    };

    // same as initializer that generates wombos of all sizes (aka bruteforce)
    // but generates only the best (or one of the best) combination :^)
    void find_leg_room(int sum)
    {
        set<std::pair<set<int>, int>> possible_prev{{{}, 0}};
        set<std::pair<set<int>, int>> possible_new{};

        auto fqe_cmp = [](std::pair<set<int>, float> st, std::pair<set<int>, float> nd)
        {
            return st.second < nd.second;
        };
        set<std::pair<set<int>, float>, decltype(fqe_cmp)> possible_wombos;

        while (!possible_prev.empty()) //until we can try to add something new
        {
            for (int i = 0; i < packages.size(); ++i) // for every package
            {
                for (auto poss : possible_prev) // for every possible subset
                {
                    if (poss.first.count(i) != 0) continue;
                    int poss_sum = poss.second + packages[i];
                    if (poss_sum > sum) continue;
                    poss.first.insert(i);
                    if (poss_sum == sum)
                    {
                        possible_wombos.insert({poss.first, f_wombo_qe(poss.first)});
                        std::cout << "found possible wombo: " << poss.first << std::endl;
                        continue;
                    }
                    poss.second = poss_sum;
                    possible_new.insert(poss);
                }
            }
            if (!possible_wombos.empty())
            {
                for (auto wombo_candidate : possible_wombos)
                {
                    set<int> poss_compl1 = generate_compl(wombo_candidate.first, sum);
                    if (poss_compl1.empty()) continue;
                    set<int> poss_compl2 = generate_compl(wombo_candidate.first, poss_compl1, sum);
                    if (poss_compl1.empty()) continue;
                    set<int> poss_compl3 = generate_compl(wombo_candidate.first, poss_compl1, poss_compl2);
                    wombos.insert(wombo_candidate.first);
                    wombos.insert(poss_compl1);
                    wombos.insert(poss_compl2);
                    wombos.insert(poss_compl3);
                    return;
                }
            }
            possible_prev = possible_new;
            possible_new.clear();
        }
    }

    void simple_print()
    {
        std::cout << std::boolalpha;
        std::cout << "wombos with sum of " << wombo_sum(*wombos.begin()) << ":\n";
        for (auto wombo : wombos)
        {
            std::cout << wombo << " (QE: " << wombo_qe(wombo) << ")" << std::endl;
        }
    }

    void print()
    {
        std::cout << std::boolalpha;
        std::cout << "wombos with sum of " << wombo_sum(*wombos.begin()) << ":\n";
        for (auto wombo : wombos)
        {
            std::cout << wombo << " (QE: " << wombo_qe(wombo) << "); has complement: ";
            std::cout << find_complement(wombo) << std::endl;
        }
    }
};



int main()
{

    vector<int> test_packages {11, 10, 9, 8, 7, 5, 4, 3, 2, 1};
    std::sort(test_packages.begin(), test_packages.end());
    auto test_sum = std::accumulate(test_packages.begin(), test_packages.end(), 0) / 4;

    
    std::ifstream ifs {};
    ifs.open({"input"});
    if (!ifs) {
        std::cerr << "Couldn't open file \"input\"\n";
    }
    

    vector<int> packages {};
    while(true)
    {
        std::string line;
        std::getline(ifs, line);
        if (line == "") break;
        int number = std::stoi(line);
        packages.push_back(number);
    }
    auto sum = std::accumulate(packages.begin(), packages.end(), 0) / 4;
    std::cout << "read packages: " << packages << std::endl;
    std::cout << "single compartment volume: " << sum << std::endl;

    // first, find a part 1 with min amount of packages, and other combinations with that amount;
    // for an amount of k with total n packages there is n(n - 1)...(n - k + 1)/(k * (k-1)...) combinations 
    // then try to divide the rest of packages in half, and if succeed, then it's ok, else repeat previous with k + 1
    
    Combo com(packages);

    com.find_leg_room(sum);
    com.print();

    return 0;
}
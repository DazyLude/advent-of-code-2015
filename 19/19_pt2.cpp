// this solution finds a way to transmutate starting molecule into the target relatively fast,
// but confirming that it is a *most* optimal solution takes time. A lot of time.

#include <string>
#include <string_view>
#include <set>
#include <map>
#include <fstream>
#include <vector>
#include <list>
#include "src/parsers.h"

using std::vector;
using std::set;
using std::map;

// same as pt1, but molecules are replaced with id's

// compound type
typedef vector<int> compound;
std::ostream& operator<<(std::ostream& os, compound c)
{
    for (auto i : c)
    {
        os << i << " ";
    }
    return os;
}

// transmut type
typedef std::pair<compound, compound> transmut;
bool cmp_tr(transmut a, transmut b)
{   
    if (a.second.size() == b.second.size()) return (a < b);
    return a.second.size() > b.second.size();
};
std::ostream& operator<<(std::ostream& os, transmut t)
{
    os << t.first << " -> " << t.second;
    return os;
}

// string to compound parser
compound stoc(std::string& s, map<std::string, int>& dictionary)
{
    compound r_result; 
    auto j = s.size();
    while (true)
    {
        for(auto i {1}; i <= j; ++i)
        {
            if (dictionary.contains(s.substr(j - i, i)))
            {
                r_result.push_back(dictionary.at(s.substr(j - i, i)));
                j -= i;
                break;
            }
        }
        if (j == 0) break;
    }
    compound result;
    for (auto thing = r_result.rbegin(); thing != r_result.rend(); ++thing)
    {
        result.push_back(*thing);
    } 
    return result;
};

compound detransmutate(compound& old, compound::iterator begin, compound::iterator end, compound ins)
{
    compound n {};
    for (auto o = old.begin(); o != old.end(); ++o)
    {
        if (o == begin && begin != end)
        {
            ++begin;
        }
        else if (begin == end)
        {
            --begin;
            for (auto i : ins)
            {
                n.push_back(i);
            }
        }
        else
        {
            n.push_back(*o);
        }
    }

    return n;
}
int  desynthethise(compound& c, int n_t, set<transmut, decltype(&cmp_tr)>& t, compound& target, int& bound, set<compound>& used_products)
{
    if (c == target) 
    {
        std::cout << "success in: " << n_t << std::endl;
        return n_t;
    }

    if (n_t >= bound)
    {
        return 0;
    }

    if (used_products.contains(c))
    {
        return 0;
    }
    used_products.insert(c);

    // since electron can't be decomposed, if the compund has it, we can prune it
    for (auto elem : c)
    {
        // works for targets with length of 1
        if (elem == target[0])
        {
            return 0;
        }
    }

    auto t_bound {bound}; 
    
    for (auto tr = t.begin(); tr != t.end(); ++tr)
    {
        for (auto elem = c.begin(); elem != c.end(); ++elem)
        {
            if (*tr->second.begin() == *elem)
            {
                auto start = elem;
                bool connect {true};
                for(auto tr_it = tr->second.begin(); tr_it != tr->second.end() && elem != c.end(); ++elem, ++tr_it)
                {
                    if (*tr_it != *elem)
                    {
                        connect = false;
                        break;
                    }
                }
                --elem;
                if (connect) 
                {
                    auto product = detransmutate(c, start, elem, tr->first);
                    auto n_bound = desynthethise(product, n_t + 1, t, target, t_bound, used_products);
                    if (n_bound != 0 && n_bound < t_bound) t_bound = n_bound;
                }
                elem = start;
            }
        }
    }
    return t_bound;
}

int main()
{
    std::ifstream ifs;
    FileParsers::open_input(ifs);

    set<std::pair<std::string, std::string>> s_quantum_bazinga;
    std::string s_molecule;
    std::string s_start {"e"};

    while(true)
    {
        std::string line;
        std::getline(ifs, line);
        if (line == "") break;

        std::string_view line_v {line};

        std::vector<std::string> words;
        for (auto i {0}; i < 3; ++i)
        {
            words.push_back(StringParsers::parse_word(line_v));
        }

        s_quantum_bazinga.insert({words[0], words[2]});
    }

    ifs >> s_molecule;

    // s_molecule = {"HOHOHO"};
    // s_quantum_bazinga = {{"H", "HO"}, {"H", "OH"}, {"O", "HH"}, {"e", "H"}, {"e", "O"}};

    map<std::string, int> SCFSMB; // an acronym
    int id {0};
    std::list<std::string> mess;
    
    // making a mess *intensive cough*
    for (auto mnm : s_quantum_bazinga)
    {
        mess.push_back(mnm.first);
        mess.push_back(mnm.second);
    }

    // returns a shortest element from mess, removing all of those from it
    auto shortest_m = [&mess]
    {
        auto shortest = mess.begin()->size();

        for (auto messiah : mess)
        {
            if (messiah.size() < shortest) shortest = messiah.size();
        }

        for (auto messiah : mess)
        {
            if (messiah.size() == shortest)
            {
                auto save = messiah;
                mess.remove(messiah);
                return save;
            }
        }
        return std::string("s");
    };

    // clearing mess from elements
    while(!mess.empty())
    {
        std::string new_one = shortest_m();
        SCFSMB.insert({new_one, id++});
        set<std::string> will_remove;
        for (auto messier : mess)
        {
            if (will_remove.contains(messier)) continue;
            if (messier.find(new_one) != std::string::npos)
            {
                auto cur = messier.find(new_one);
                mess.push_back(messier.substr(0, cur));
                mess.push_back(messier.substr(cur + new_one.size()));
                will_remove.insert(messier);
            }
        }
        for (auto rm : will_remove) mess.remove(rm);
        will_remove.clear();
        mess.remove("");
    }

    auto max_element_size {SCFSMB.begin()->first.size()};
    for (auto kv : SCFSMB)
    {
        if (max_element_size < kv.first.size()) max_element_size = kv.first.size();
        std::cout << kv.first << ": " << kv.second << std::endl;
    }

    set<compound> processed_molecules;
    set<transmut, decltype(&cmp_tr)> quantum_bazinga(cmp_tr);
    compound molecule;
    
    for (auto bazinga : s_quantum_bazinga)
    {
        quantum_bazinga.insert({stoc(bazinga.first, SCFSMB), stoc(bazinga.second, SCFSMB)});
    }

    molecule = stoc(s_molecule, SCFSMB);
    auto start = stoc(s_start, SCFSMB);

    std::cout << start << " to " << molecule << std::endl;

    for (auto q : quantum_bazinga)
    {
        std::cout << q << std::endl;
    }
    // // pt1
    // for (auto replacement : quantum_bazinga)
    // {
    //     replace_magic(processed_molecules, molecule, replacement);
    // }
    // for (auto mol : processed_molecules)
    // {
    //     std::cout << mol << std::endl;
    // }
    // std::cout << processed_molecules.size() << std::endl;

    // // pt2
    int i_bound = molecule.size() + 1;
    set<compound> used {};
    std::cout << desynthethise(molecule, 0, quantum_bazinga, start, i_bound, used);

    return EXIT_SUCCESS;
}
#include <string>
#include <iostream>
#include <list>
#include <memory>
#include <map>
#include <set>
#include <vector>
#include <string>


class Character;

enum class StatusTypes
{
    Test = 0,
    Poison,
    Shield,
    Recharge,
    HardMode,
};

class StatusEffect
{
protected:
    int duration {0};
    static const int initial_length {3};
public:
    static const StatusTypes name {StatusTypes::Test};
    
    StatusEffect() {duration = initial_length;}
    
    StatusEffect& operator=(const StatusEffect& other)
    {
        this->duration = other.duration;
    }

    const int get_duration() {return duration;}
    void tick() {--duration;}

    virtual StatusEffect* clone()
    {
        return new StatusEffect(*this);
    }

    // effects of a given status
    // when first applied
    virtual void apply_effect(Character* target)
    {
        std::cout << "applied test effect to " << target << std::endl;
    };
    // when duration is over
    virtual void finished_effect(Character* target)
    {
        std::cout << "test effect has ended" << std::endl;
    };
    // when ticking
    virtual void effect(Character* target) 
    {
        std::cout << "test effect called for a character located at " << target << std::endl;
        tick();
        if (duration == 0) finished_effect(target);
    }

    virtual void output_info(std::ostream& os) const
    {
        os << "Test Effect";
    }
    friend std::ostream& operator<< (std::ostream& os, const StatusEffect& eff);
};

std::ostream& operator<< (std::ostream& os, StatusEffect const& eff)
{
    eff.output_info(os);
    return os;
}

enum class Spells
{
    MagicMiss = 0,
    Drain = 1,
    Poison = 2,
    Shield = 3,
    Recharge = 4,
    Nothing = 5,
};

std::ostream& operator<<(std::ostream& os, Spells spell)
{
    switch (spell)
    {
    case Spells::MagicMiss:
        os << "Magic Missile";
        break;
    case Spells::Recharge:
        os << "Recharge";
        break;
    case Spells::Drain:
        os << "Drain";
        break;
    case Spells::Poison:
        os << "Poison";
        break;
    case Spells::Shield:
        os << "Shield";
        break;
    case Spells::Nothing:
        os << "Nothing";
    }
    return os;
}

std::map<Spells, int> spell_cost
{
    {Spells::Nothing, 0},
    {Spells::MagicMiss, 53},
    {Spells::Drain, 73},
    {Spells::Poison, 173},
    {Spells::Shield, 113},
    {Spells::Recharge, 229},
};

class Character
{
    std::list<std::unique_ptr<StatusEffect>> status {};
public:
    std::set<StatusTypes> current_status {};
    int mana;
    int hp;
    int damage;
    int armor;


    Character(int d, int a, int h, int m) : damage(d), armor(a), hp(h), mana(m) {}
    Character() : Character(0, 0, 100, 500) {}
    ~Character() {};
    Character(const Character& other)
    {
        damage = other.damage;
        armor = other.armor;
        hp = other.hp;
        mana = other.mana;
        current_status = other.current_status;
        
        for (auto status_ptr = other.status.begin(); status_ptr != other.status.end(); ++status_ptr)
        {
            status.push_back(std::unique_ptr<StatusEffect>(status_ptr->get()->clone()));
        }
    }
    Character& operator=(const Character& other)
    {
        damage = other.damage;
        armor = other.armor;
        hp = other.hp;
        mana = other.mana;
        current_status = other.current_status;

        for (auto status_ptr = other.status.begin(); status_ptr != other.status.end(); ++status_ptr)
        {
            status.push_back(std::unique_ptr<StatusEffect>(status_ptr->get()->clone()));
        }
        return *this;
    }
    Character(Character&& other) = delete;
    Character& operator=(Character&& other) = delete;
    
    bool add_status(StatusTypes st);

    void tick_status()
    {
        if (status.empty()) return;
        for (auto st = status.begin(); st != status.end(); ++st)
        {
            // std::cout << *(st->get()) << std::endl;
            st->get()->effect(this);
        }
        status.remove_if([](std::unique_ptr<StatusEffect>& st_ptr){return st_ptr.get()->get_duration() == 0;});
    }

    bool cast_spell(Spells sp, Character& target)
    {
        if (mana < spell_cost[sp]) return false;
        mana -= spell_cost[sp];
        switch(sp)
        {
        case Spells::MagicMiss:
            target.hp -= 4;
            break;
        case Spells::Drain:
            target.hp -= 2;
            hp += 2;
            break;
        case Spells::Poison:
            return target.add_status(StatusTypes::Poison);
        case Spells::Shield:
            return add_status(StatusTypes::Shield);
        case Spells::Recharge:
            return add_status(StatusTypes::Recharge);
        }
        return true;
    }
};

class HardMode : public StatusEffect
{
    static const int initial_length {0};
    bool should_damage {true};
public:
    static const  StatusTypes name = StatusTypes::HardMode;
    
    HardMode() {duration = initial_length;};
    HardMode(HardMode& other) {};
    HardMode* clone() {return new HardMode(*this);}
    void output_info(std::ostream& os) const
    {
        os << "HardMode; Remaining duration: infinite";
    }
    void apply_effect(Character* target) {}
    void finished_effect(Character* target) {}
    void effect(Character* target)
    {
        if (should_damage)
            target->hp -= 1;
        should_damage = !should_damage;
    }
};

class Poison : public StatusEffect
{
    static const int initial_length {6};
public:
    static const  StatusTypes name = StatusTypes::Poison;
    
    Poison() {duration = initial_length;};
    Poison(Poison& other)
    {
        this->duration = other.duration;
    }
    Poison* clone() {return new Poison(*this);}
    void output_info(std::ostream& os) const
    {
        os << "Poison; Remaining duration: " << duration;
    }
    void apply_effect(Character* target) {}
    void finished_effect(Character* target)
    {
        target->current_status.erase(name);
    }
    void effect(Character* target)
    {
        target->hp -= 3;
        tick();
        if (duration == 0) finished_effect(target);
    }
};

class Shield : public StatusEffect
{
    static const int initial_length {6};
public:
    static const StatusTypes name = StatusTypes::Shield;

    Shield() {duration = initial_length;};
    Shield(Shield& other)
    {
        this->duration = other.duration;
    }
    Shield* clone() {return new Shield(*this);}
    void output_info(std::ostream& os) const
    {
        os << "Shield; Remaining duration: " << duration;
    }
    void apply_effect(Character* target)
    {
        target->armor += 7;
    }
    void finished_effect(Character* target)
    {
        target->armor -= 7;
        target->current_status.erase(name);
    }
    void effect(Character* target)
    {
        tick();
        if (duration == 0) finished_effect(target);
    }
};

class Recharge : public StatusEffect
{
    static const int initial_length {5};
public:
    static const StatusTypes name = StatusTypes::Recharge;

    Recharge() {duration = initial_length;};
    Recharge(Recharge& other)
    {
        this->duration = other.duration;
    }
    Recharge* clone() {return new Recharge(*this);}
    void output_info(std::ostream& os) const
    {
        os << "Recharge; Remaining duration: " << duration;
    }
    void apply_effect(Character* target) {}
    void finished_effect(Character* target)
    {
        target->current_status.erase(name);
    }
    void effect(Character* target)
    {
        target->mana += 101;
        tick();
        if (duration == 0) finished_effect(target);
    }
};

bool Character::add_status(StatusTypes st)
{
    if (current_status.count(st) != 0) return false;
    current_status.insert(st);

    switch(st)
    {
    case StatusTypes::Test:
        status.push_back(std::unique_ptr<StatusEffect>(new StatusEffect()));
        break;
    case StatusTypes::HardMode:
        status.push_back(std::unique_ptr<StatusEffect>(new HardMode()));
        break;
    case StatusTypes::Poison:
        status.push_back(std::unique_ptr<StatusEffect>(new Poison()));
        break;
    case StatusTypes::Shield:
        status.push_back(std::unique_ptr<StatusEffect>(new Shield()));
        break;
    case StatusTypes::Recharge:
        status.push_back(std::unique_ptr<StatusEffect>(new Recharge()));
        break;
    }
    
    status.back().get()->apply_effect(this);
    return true;
};

enum class RoundResults
{
    hero_killed,
    invalid_spell,
    boss_killed,
    boss_killed_before_cast,
    stalemate,
};

RoundResults new_round(Character& hero, Character& boss, Spells sp)
{
    // tick effects
    boss.tick_status();
    if (boss.hp <= 0) return RoundResults::boss_killed_before_cast;
    hero.tick_status();

    // cast spell
    if (!hero.cast_spell(sp, boss)) return RoundResults::invalid_spell;
    if (boss.hp <= 0) return RoundResults::boss_killed;
    
    // tick effects
    boss.tick_status();
    if (boss.hp <= 0) return RoundResults::boss_killed;
    hero.tick_status();

    // take damage
    int effective_damage {boss.damage - hero.armor <= 1 ? 1 : boss.damage - hero.armor};
    hero.hp -= effective_damage;
    if (hero.hp <= 0) return RoundResults::hero_killed;
    return RoundResults::stalemate;
}

class SpellSequenceTree
{
    struct Node
    {
        Node* parent_node {nullptr};
        std::list<Node*> children_nodes {nullptr};
        Character hero;
        Character boss;
        int spent_mana;
        Spells last_spell;
        RoundResults this_round_result;

        // creates a leaf node
        Node(Node* dad, Character old_hero, Character old_boss, int old_mana, Spells new_spell)
        {
            parent_node = dad;
            dad->children_nodes.push_back(this);
            
            this_round_result = new_round(old_hero, old_boss, new_spell);
            hero = old_hero;
            boss = old_boss;
            
            spent_mana = old_mana += spell_cost[new_spell];
            last_spell = new_spell;
        }
        // creates a root node
        Node(Character new_hero, Character new_boss)
        {
            parent_node = nullptr;
            hero = new_hero;
            boss = new_boss;
            spent_mana = 0;
            last_spell = Spells::Nothing;
            this_round_result = RoundResults::stalemate;
        }

        // destructor deletes all of this node's children
        ~Node()
        {
            for (auto child : children_nodes) delete child;
        }

        void remove_childe(Node* whomst) // from genshin
        {
            delete whomst;
            children_nodes.remove(whomst);
        }

        Node() = default;
        Node(const Node& other) = default;
        Node& operator=(const Node& other) = default;
        Node(Node&& other) = delete;
        Node& operator=(Node&& other) = delete;
    };
    Node* base_node;
    Node* interesting_node;
public:
    SpellSequenceTree(Character h, Character b)
    {
        base_node = new Node(h, b);
        interesting_node = base_node;
    }
    ~SpellSequenceTree()
    {
        delete base_node;
    }

    int get_least_mana()
    {
        int bound {0};
        std::vector<Node*> last_nodes {base_node};
        while(!last_nodes.empty())
        {
            std::vector<Node*> new_last_nodes {};
            for (auto current_node = last_nodes.begin(); current_node != last_nodes.end(); ++current_node)
            {
                if (bound != 0 && (*current_node)->spent_mana > bound) continue;
                for (int i = 0; i < 5; ++i)
                {
                    auto new_node = new Node(*current_node, (*current_node)->hero, (*current_node)->boss, (*current_node)->spent_mana, Spells(i));
                    switch(new_node->this_round_result)
                    {
                    case RoundResults::invalid_spell:
                    case RoundResults::hero_killed:
                        (*current_node)->remove_childe(new_node);
                        break;
                    case RoundResults::stalemate:
                        new_last_nodes.push_back(new_node);
                        break;
                    case RoundResults::boss_killed_before_cast:
                        std::cout << "boss killed before cast";
                        new_node->last_spell = Spells::Nothing;
                    case RoundResults::boss_killed:
                        if (bound == 0 || new_node->spent_mana <= bound)
                        {
                            bound = new_node->spent_mana;                
                            interesting_node = new_node;
                        }
                        break;
                    default:
                        std::cout << "what\n" ;
                    }
                }
            }
            last_nodes = new_last_nodes;
        }
        Node* current_node = base_node;

        return bound;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Node& node);
    const void print_interesting_node()
    {
        std::cout << *interesting_node;
    }
};

std::ostream& operator<<(std::ostream& os, const SpellSequenceTree::Node& node)
{
    if (node.parent_node != nullptr)
    {
        os << *(node.parent_node) << " ";
    }
    os << node.last_spell;
    return os;
}

int main()
{
    
    // Character boss {8, 0, 14, 0};
    // Character hero {0, 0, 10, 250};
    Character boss {9, 0, 58, 0};
    Character hero {0, 0, 50, 500};
    hero.add_status(StatusTypes::HardMode);

    // std::cout << boss.add_status(StatusTypes::Poison);
    // std::cout << boss.add_status(StatusTypes::Poison);
    // std::cout << boss.add_status(StatusTypes::Shield);
    // std::cout << boss.add_status(StatusTypes::Recharge) << std::endl;
    
    // for (int i {0}; i < 7; ++i) 
    // {
    //     boss.tick_status();
    //     std::cout << "hp: " << boss.hp << "; armor: " << boss.armor << "; mana: " << boss.mana << std::endl;
    //     // now copy character
    //     auto c_boss = boss;
    //     std::cout << "copy\n";
    //     std::cout << "copy's hp: " << c_boss.hp << "; copy's armor: " << c_boss.armor << "; copy's mana: " << c_boss.mana << std::endl;
    //     c_boss.tick_status();
    //     std::cout << "tick copy\n";
    //     std::cout << "copy's hp: " << c_boss.hp << "; copy's armor: " << c_boss.armor << "; copy's mana: " << c_boss.mana << std::endl;
    // }
    SpellSequenceTree battle(hero, boss);
    std::cout << battle.get_least_mana() << std::endl;
    battle.print_interesting_node();
    std::cout <<  std::endl;

    return EXIT_SUCCESS;
}
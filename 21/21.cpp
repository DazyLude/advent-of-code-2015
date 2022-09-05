#include <vector>
// #include <set>
#include <map>
#include <string>
#include <iostream>
#include <tuple>

class Stats
{
protected:
    int hp;
    int cost;
    int damage;
    int armor;
public:
    Stats(int c, int d, int a) : cost(c), damage(d), armor(a), hp(100) {}
    Stats() : Stats(0, 0, 0) {}
    int get_cost() {return cost;}
    int get_damage() {return damage;}
    int get_armor() {return armor;}
};

enum class ItemSlot
{
    Hands,
    Body,
    Finger,
};

class Item : public Stats
{
public:
    ItemSlot slot;
    std::string name;
    Item(ItemSlot sl, std::string n, Stats st) : Stats(st), slot(sl), name(n) {}
    static bool less(Item one, Item another)
    {
        if (one.slot == another.slot) return one.name < another.name;
        return one.slot < another.slot;
    }
    bool operator==(Item another)
    {
        return name == another.name;
    }
};

std::vector<Item> Weapons
{
    {ItemSlot::Hands, {"Wooden Stick"}, {0, 0, 0}},
    {ItemSlot::Hands, {"Dagger"}, {8, 4, 0}},
    {ItemSlot::Hands, {"Shortsword"}, {10, 5, 0}},
    {ItemSlot::Hands, {"Warhammer"}, {25, 6, 0}},
    {ItemSlot::Hands, {"Longsword"}, {40, 7, 0}},
    {ItemSlot::Hands, {"Greataxe"}, {74, 8, 0}},
};
std::vector<Item> Armors
{
    {ItemSlot::Body, {"Bare Skin"}, {0, 0, 0}},
    {ItemSlot::Body, {"Leather"}, {13, 0, 1}},
    {ItemSlot::Body, {"Chainmail"}, {31, 0, 2}},
    {ItemSlot::Body, {"Splintmail"}, {53, 0, 3}},
    {ItemSlot::Body, {"Bandedmail"}, {75, 0, 4}},
    {ItemSlot::Body, {"Platemail"}, {102, 0, 4}},
};
std::vector<Item> Rings
{   
    {ItemSlot::Finger, {"Empty Promises"}, {0, 0, 0}},
    {ItemSlot::Finger, {"Hollow Dreams"}, {0, 0, 0}},
    {ItemSlot::Finger, {"Damage+1"}, {25, 1, 0}},
    {ItemSlot::Finger, {"Damage+2"}, {50, 2, 0}},
    {ItemSlot::Finger, {"Damage+3"}, {100, 3, 0}},
    {ItemSlot::Finger, {"Armor+1"}, {20, 0, 1}},
    {ItemSlot::Finger, {"Armor+2"}, {40, 0, 2}},
    {ItemSlot::Finger, {"Armor+3"}, {80, 0, 3}},
};


class Character : public Stats
{
    std::vector<Item> inventory {};
    std::map<ItemSlot, int> free_slots {{ItemSlot::Finger, 2}, {ItemSlot::Hands, 1}, {ItemSlot::Body, 1}};
public:
    Character(int c, int d, int a) : Stats(c, d, a) {}
    Character() : Character(0, 0, 0) {}
    void equip(Stats e)
    {
        cost += e.get_cost();
        damage += e.get_damage();
        armor += e.get_armor();
        
    }
    bool try_equip(Item it)
    {
        if (!inventory.empty()) for (auto equipped : inventory)
        {
            if (equipped == it) return false;
            if (free_slots.at(it.slot) <= 0) return false;
        }
        equip(it);
        
        inventory.push_back(it);
        --free_slots[it.slot];

        return true;
    }
    void equip(int w, int a, int r1, int r2)
    {
        try_equip(Weapons.at(w));
        try_equip(Armors.at(a));
        try_equip(Rings.at(r1));
        try_equip(Rings.at(r2));
    }
    bool fight(Character another)
    {
        auto eff_dmg_to = (damage - another.armor) > 0 ? (damage - another.armor) : 1;
        auto eff_dmg_from = (another.damage - armor) > 0 ? (another.damage - armor) : 1;
        while (true)
        {
            another.hp -= eff_dmg_to;
            if (another.hp <= 0) return true;
            hp -= eff_dmg_from;
            if (hp <= 0) return false;
        }
    }
    bool predict_fight(Character another)
    {
        auto eff_dmg_to = (damage - another.armor) > 0 ? (damage - another.armor) : 1;
        auto eff_dmg_from = (another.damage - armor) > 0 ? (another.damage - armor) : 1;\
        // ttk player <= ttk boss
        return (another.hp - 1) / eff_dmg_to + 1 <= (hp - 1) / eff_dmg_from + 1;
    }

    friend std::ostream& operator<<(std::ostream& os, const Character& character);
};

std::ostream& operator<<(std::ostream& os, const Character& character)
{
    os << "this character has ";
    for (auto item : character.inventory)
    {
        switch (item.slot)
        {
        case ItemSlot::Hands:
            os << item.name << " in his hands";
            break;
        case ItemSlot::Body:
            os << item.name << " as his armor";
            break;
        case ItemSlot::Finger:
            os << "a ring of " << item.name;
            break;
        }
        os << ", ";
    }
    os << "and all this drip costs just " << character.cost <<  std::endl;
    return os;
}

int main()
{
    Character boss {0, 8, 2};

    int bound = 100;

    for (int w {1}; w < Weapons.size(); ++w)
    {
        for (int a {0}; a < Armors.size(); ++a)
        {
            for (int r1 {0}; r1 < Rings.size() - 1; ++r1)
            {
                for (int r2 {r1 + 1}; r2 < Rings.size(); ++r2)
                {
                    Character player {};
                    player.equip(w, a, r1, r2);
                    if (player.get_cost() <= bound) continue;
                    if (!player.fight(boss))
                    {
                        bound = player.get_cost();
                        std::cout << "New loser!\n" << player;
                    }
                }
            }
        }
    }
    return EXIT_SUCCESS;
}
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const char vowels[5] = {'a', 'i', 'u', 'e', 'o'};
const std::string naughty_combos[4] = {"ab", "cd", "pq", "xy"};

bool is_vowel (char letter) {
    for (char vowel : vowels) {
        if (vowel == letter) return true;
    }
    return false;
}

bool has_naughty (std::string& combo) {
    for (std::string naughty : naughty_combos) {
        if (naughty == combo){
            return true;
        }                
    }
    return false;
}

void count_rules1 (int& count, std::string& word) {
    // assumes word.size() >= 2
    bool is_naughty = false;
    int vowel_count = 0;
    if (is_vowel(word[0])) ++vowel_count;
    bool parity_check = false;
    for (size_t i = 1; i < word.size(); ++i) {
        if (has_naughty(word.substr(i-1, 2))) {
            is_naughty = true;
            break;
        }
        if (is_vowel(word[i])) ++vowel_count;
        if (word[i] == word[i-1]) parity_check = true;
    }
    if (!is_naughty && parity_check && vowel_count >= 3) ++count;
}

// recursion baayyybeee
bool pair_check (std::vector<std::string>& pairs) {
    if (pairs.size() > 2) {
        for (int i = 0; i < pairs.size() - 2; ++i) {
            if (pairs.back() == pairs[i]) return true;
        }
        pairs.pop_back();
        return pair_check(pairs);
    }
    return false;
} 


void count_rules2 (int& count, const std::string& word) {
    // assumes word.size() >= 3
    std::vector<std::string> pairs;
    pairs.push_back(word.substr(0,2));
    bool x_x_check = false;
    for (auto i {1}; i < word.size(); ++i) {
        pairs.push_back(word.substr(i,2));
        if (word[i-1] == word[i+1]) x_x_check = true;
    }
    if (x_x_check && pair_check(pairs)) ++count;
}

void main() {
    auto filename {"input"};
    std::ifstream ifs {filename};
    if (!ifs) {
        std::cerr << "Couldn't open file \"input\"\n";
        return;
    }
    auto count {0};
    // count_rules2(count, "qjhvhtzxzqqjkmpb");
    while(true) {
        std::string word;
        std::getline(ifs, word);
        count_rules2(count, word);
        if (ifs.eof()) break;
    }
    std::cout << "there is " << count << " nice strings";

    ifs.close();
}
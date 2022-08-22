#include <string>
#include <iostream>
#include <set>

const size_t PWD_SIZE = 8;

void increment_letter(std::string& pwd, size_t& index)
{
    if (pwd[index] != 'z')
    {
        ++pwd[index];
    }
    else
    {
        pwd[index] = 'a';
        --index >= 0 ? increment_letter(pwd, index) : throw std::overflow_error("couldn't increment pwd");
    }
}

void increment_pwd(std::string& pwd)
{
    size_t size = PWD_SIZE - 1;
    increment_letter(pwd, size);
}

void ilo_skip(std::string& pwd, size_t pos)
{
    ++pwd[pos];
    for (size_t i = pos + 1; i < PWD_SIZE; ++i)
    {
        pwd[i] = 'a';
    }
}

bool straight_check(std::string& pwd)
{
    std::string last_3 {};
    for (size_t i = 0; i < PWD_SIZE - 2; ++i)
    {
        last_3 = pwd.substr(i, 3);
        if ((last_3[0] + 1 == last_3[1]) && (last_3[1] + 1 == last_3[2])) {
            return true;
        } 
    }
    return false;
}

bool pair_check(std::string& pwd)
{
    std::set<char> pairs {};
    for (size_t i = 0; i < PWD_SIZE - 1; ++i)
    {
        if (pwd[i] == pwd[i + 1]) {
            pairs.insert(pwd[i]);
            ++i;
        }
    }
    return pairs.size() >= 2;
}

bool ilo_check(std::string& pwd)
{
    for (size_t i = 0; i < PWD_SIZE; ++i)
    {
        if (pwd[i] == 'i' || pwd[i] == 'l' || pwd[i] == 'o')
        {
            ilo_skip(pwd, i);
            return false;
        }
    }
    return true;
}


int main()
{
    // old password
    std::string pwd {"inputpwd"};
    increment_pwd(pwd);
    
    auto all_check = [&](){return !ilo_check(pwd) || !pair_check(pwd) || !straight_check(pwd);};
    
    while (all_check()) {
        increment_pwd(pwd);
    }

    std::cout << pwd;
}
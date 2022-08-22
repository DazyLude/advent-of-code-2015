#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

// advances is until non-digit character or eof is encountered
// encountered digits are returned as a string
string parse_number(std::istream& is)
{
    if (is.eof()) return "0";
    std::string number {};
    while (isdigit(is.peek()) != 0 && !is.eof())
    {
        number.push_back(is.get());
    }
    return number;
}

// advances is until digit character or eof is encountered
// if preceding character is '-', returns false, else - true
bool skip_till_digit_(std::istream& is) {
    auto last_char {is.get()};
    while (isdigit(is.peek()) == 0 && !is.eof())
    {
        last_char = is.get();
    }
    return last_char != '-';
}

// advances is until a digit, {, }, [, ], "red", or eof is encountered
// if not digit, returns encountered character or 'r' in case of "red"
// if a digit, returns + if preceding character is not a '-', else '-'
// in case of reached eof returns ' '
// assumes that istream has at least 1 character
char skip_till_special(std::istream& is) {
    // return '+' if the first character is a digit
    if (isdigit(is.peek()) != 0) {return '+';}

    std::string red {"\"red\""};
    int red_counter {0};
    char last_char;
    do {
        if (is.eof()) return ' ';
        last_char = is.get();
        switch (last_char) 
        {
            case '[':
            case ']':
            case '{':
            case '}':
                return last_char;
        }
        if (last_char == red[red_counter]) 
        {
            if (++red_counter == red.size()) return 'r';
        } else {
            red_counter = 0;
        }
    } while (isdigit(is.peek()) == 0);
    
    return last_char == '-' ? '-' : '+';
}


// advances is until the current object ends
// returns sum of numbers (and objects) inside this object
// returns 0 if object has a property of red and is not an array
int parse_json_object(std::istream& is, bool is_array) {
    int self_count {0};
    bool red_alert {false};
    while(true)
    {
        switch(skip_till_special(is))
        {
            case '{': 
                self_count += parse_json_object(is, false);
                break;
            case '[': 
                self_count += parse_json_object(is, true);
                break;
            case '+':
                self_count += std::stoi(parse_number(is));
                break;
            case '-':
                self_count -= std::stoi(parse_number(is));
                break;
            case 'r':
                red_alert = is_array ? false : true;
                break;
            default:
                return red_alert ? 0 : self_count;
        }
    }
}

#include <sstream>

int main() {
    // std::istringstream test1 {R"([1,2,3])"};
    // std::cout << parse_json_object(test1, false) << std::endl;
    // // ouputs 6
    // std::istringstream test2 {R"([1,{"c":"red","b":2},3])"};
    // std::cout << parse_json_object(test2, false) << std::endl;
    // // ouputs 4
    // std::istringstream test3 {R"({"d":"red","e":[1,2,3,4],"f":5})"};
    // std::cout << parse_json_object(test3, false) << std::endl;
    // // ouputs 0
    // std::istringstream test4 {R"([1,"red",5])"};
    // std::cout << parse_json_object(test4, false) << std::endl;
    // // ouputs 6

    auto filename {"input"};
    std::ifstream ifs {filename};
    if (!ifs) {
        std::cerr << "Couldn't open file \"input\"\n";
        return 1;
    }
    auto count {0};
    while(!ifs.eof()) {
        count = parse_json_object(ifs, false); 
    }
    std::cout << count << std::endl;
}
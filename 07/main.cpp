#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cctype>

using std::string;

enum class Operators {
    connect,
    not,
    and,
    or,
    lshift,
    rshift,
};

std::ostream& operator<<(std::ostream& os, const Operators op) {
    switch(op) {
        case Operators::connect:
            os << "->";
            break;
        case Operators::not:
            os << "not";
            break;
        case Operators::and:
            os << "&";
            break;
        case Operators::or:
            os << "|";
            break;
        case Operators::rshift:
            os << ">>";
            break;
        case Operators::lshift:
            os << "<<";
            break;
    }
    return os;
}

struct Gate {
    Operators type;
    string input[2];
    string output;

    Gate(Operators _type, string _input) {
        if (_type != Operators::not && _type != Operators::connect) {
            std::cerr << "Initializing gate with an incorrect operation: expected 2 inputs, got 1\n";
            std::cerr << _input << _type << std::endl;
        }
        type = _type;
        input[0] = _input;
        input[1] = "";
    }
    Gate(Operators _type, string _input1, string _input2) {
        if (_type == Operators::not || _type == Operators::connect) {
            std::cerr << "Initializing gate with an incorrect operation: expected 1 input, got 2\n";
            std::cerr << _input1 << _type << _input2 << std::endl;
        }
        type = _type;
        input[0] = _input1;
        input[1] = _input2;
    }
};

// assumes input not empty
// returns a first string from the input surrounded by blanks
// spaces in front and at the end are trimmed
// modifies the input string, removing the parsed part
string parse_word(string& input) {
    string word;
    for(int i = 0; i < input.size(); ++i) {
        if (iswspace(input[i]) != 0) {
            if (!word.empty()) {
                input = input.substr(i, input.size() - i);
                return word;
            }
        } else {
            word.push_back(input[i]);
        }
    }
    input = "";
    return word;
}

Operators parse_operation(string& input) {
    switch (input[0]) {
        case 'A':
            return Operators::and;
        case 'O':
            return Operators::or;
        case 'L':
            return Operators::lshift;
        case 'R':
            return Operators::rshift;
        default:
            std::cerr << "tried assigning an unknown operation: " << input << std::endl;
            return Operators::connect;
    }
}

class Circuit {
    std::map<string, Gate> gates;
public:
    std::map<string, unsigned __int16> known_wires;
    void add_gate(string input) {
        // input has the following structure:
        // INPUT -> OUTPUT
        // where
        // INPUT is a name
        // OUTPUT is a command of one of the following types:
        // name
        // NOT name
        // name1 OPERATOR name2
        // sometimes, instead of names you get numbers
        std::vector<string> words;
        while (!input.empty()) {
            words.push_back(parse_word(input));
        }
        switch (words.size()) {
            case 3:
                gates.insert({words[2], Gate(Operators::connect, words[0])});
                break;
            case 4:
                gates.insert({words[3], Gate(Operators::not, words[1])});
                break;
            case 5:
                gates.insert({words[4], Gate(parse_operation(words[1]), words[0], words[2])});
                break;
            default:
                std::cerr << "Tried adding an incorrect gate: ";
                for (auto word : words) std::cerr << word << " " << std::endl;
        }
    } // add_gate

    unsigned __int16 calculate_output(string output_name) {
        if (gates.find(output_name) == gates.end()) {
            std::cerr << "trying to calculate undefined output " << output_name << std::endl; 
            return 0;
        }
        unsigned __int16 input[2];

        auto gate = gates.at(output_name);

        auto check_input = [this, &gate, &input](int i) {
            if (this->known_wires.find(gate.input[i]) != this->known_wires.end()) {
                std::cout << "wire found" << std::endl;
                input[i] = this->known_wires.at(gate.input[i]);
            } else {
                std::cout << "wire not found" << std::endl;
                if (isdigit(gate.input[i][0]) != 0) {
                    input[i] = std::stoi(gate.input[i]);
                } else {
                    input[i] = this->calculate_output(gate.input[i]);
                }
                std::cout << "inserting wire " << gate.input[i]  << " with value " << input[i] << std::endl;
                this->known_wires.insert({gate.input[i], input[i]});
            }
        };

        check_input(0);

        switch(gate.type) {
            case Operators::connect:
                return input[0];
            case Operators::not:
                return ~input[0];
        }
        
        check_input(1);

        switch(gate.type) {
            case Operators::and:
                return input[0] & input[1];
            case Operators::or:
                return input[0] | input[1];
            case Operators::lshift:
                return input[0] << input[1];
            case Operators::rshift:
                return input[0] >> input[1];
        }
        return 0;
    }
};



int main() {
    auto filename {"input"};
    std::ifstream ifs {filename};
    if (!ifs) {
        std::cerr << "Couldn't open file \"input\"\n";
        return 1;
    }

    Circuit circuit;

    while(true) {
        std::string command;
        std::getline(ifs, command);
        if (command.empty()) break;

        circuit.add_gate(command);
        if (ifs.eof()) break;
    }
    // comment the next line for part 1
    circuit.known_wires.insert({"b", 3176});

    std::cout << circuit.calculate_output("a") << std::endl;
}
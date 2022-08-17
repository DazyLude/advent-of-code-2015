#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using std::string;

enum class Operators {
    connect,
    not,
    and,
    or,
    lshift,
    rshift,
};

class Gate {
    Operators type;
    string input1;
    string input2;
    string output;
public:
    Gate(Operators _type, string _input, string _output) {
        if (_type != Operators::not || _type != Operators::connect) {
            std::cerr << "Initializing gate with an incorrect operation: expected 2 inputs, got 1";
            std::cerr << std::endl;
        }
        type = _type;
        input1 = _input;
        input2 = "";
        output = _output;
    }
    Gate(Operators _type, string _input1, string _input2, string _output) {
        if (_type == Operators::not || _type == Operators::connect) {
            std::cerr << "Initializing gate with an incorrect operation: expected 1 input, got 2";
            std::cerr << std::endl;
        }
        type = _type;
        input1 = _input1;
        input2 = _input2;
        output = _output;
    }
};

class Circuit {
    std::vector<Gate> gates;

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
        gates.push_back(Gate(Operators::connect, "a", "b"));
    }
}
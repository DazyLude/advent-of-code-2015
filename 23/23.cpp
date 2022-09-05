#include "src/parsers.h"
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class BrandNewComputer
{
    enum class Commands
    {
        hlf,
        tpl,
        inc,
        jmp,
        jie,
        jio,
    };

    struct Command
    {
        Commands command;
        char target;
        int value;

        Command(Commands c, char t, int v) : command(c), target(t), value(v) {};
        static Command parse_command(std::string_view input)
        {
            std::string words[3];
            words[0] = StringParsers::parse_word(input);
            words[1] = StringParsers::parse_word(input);
            switch(words[0][0])
            {
            case 'h':
                return Command(Commands::hlf, words[1][0], 0);
            case 't':
                return Command(Commands::tpl, words[1][0], 0);
            case 'i':
                return Command(Commands::inc, words[1][0], 0);
            }
            switch(words[0][2])
            {
            case 'p':
                return Command(Commands::jmp, 'n', std::stoi(words[1]));
            case 'e':
                words[2] = StringParsers::parse_word(input);
                return Command(Commands::jie, words[1][0], std::stoi(words[2]));
            case 'o':
                words[2] = StringParsers::parse_word(input);
                return Command(Commands::jio, words[1][0], std::stoi(words[2]));
            }
            return Command(Commands::jmp, 'n', 666);
        }
    };

    std::vector<Command> command_v {};    
    std::map<char, unsigned int> registers {{'a', 1}, {'b', 0}};

    void half(char reg)
    {
        registers[reg] /= 2;
    }

    void triple(char reg)
    {
        registers[reg] *= 3; 
    }

    void increment(char reg)
    {
        ++registers[reg];
    }
public:
    BrandNewComputer(std::vector<std::string>& s_command_v)
    {
        for (auto input : s_command_v)
        {
            command_v.push_back(Command::parse_command(input));
        }
    }

    void start_execution()
    {
        for (int i = 0; i < command_v.size(); ++i)
        {
            std::cout << *this;
            switch(command_v[i].command)
            {
            case Commands::hlf:
                half(command_v[i].target);
                break;
            case Commands::tpl:
                triple(command_v[i].target);
                break;
            case Commands::inc:
                increment(command_v[i].target);
                break;
            case Commands::jmp:
                i += command_v[i].value - 1;
                break;
            case Commands::jie:
                if (registers[command_v[i].target] % 2 == 0) i += command_v[i].value - 1;
                break;
            case Commands::jio:
                if (registers[command_v[i].target] == 1) i += command_v[i].value - 1;
                break;
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const BrandNewComputer& BNC);
};

std::ostream& operator<<(std::ostream& os, const BrandNewComputer& BNC)
{
    for (auto kv : BNC.registers)
    {
        os << kv.first << ": " << kv.second << std::endl;
    }
    return os;
}

int main()
{
    // std::vector<std::string> s_input {{"inc a"}, {"jio a +2"}, {"tpl a"}, {"inc a"}};

    std::ifstream ifs;
    FileParsers::open_input(ifs);

    std::vector<std::string> s_input {};

    while(true)
    {
        std::string line;
        std::getline(ifs, line);
        if (line == "") break;
        s_input.push_back(line);
    }

    BrandNewComputer BNC(s_input);
    BNC.start_execution();
    std::cout << BNC;
    return 0;
}
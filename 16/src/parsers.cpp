#include <string>
#include <string_view>
#include <iostream>
#include <fstream>

namespace FileParsers
{
    using std::ifstream;
    void open_input(ifstream& target) {
        target.open({"input"});
        if (!target) {
            std::cerr << "Couldn't open file \"input\"\n";
        }
    }
}

namespace StringParsers
{
    using std::string_view;
    using std::string;
    /// <summary>
    /// parses a string part surrounded by whitespaces, starting from beginning
    /// changes input, removing parsed part and preceeding whitespaces
    /// if no whitespaces are encountered, returns input, leaving it empty
    /// </summary>
    /// <param name="input">string to parse</param>
    /// <returns>parsed part</returns>
    string parse_word(string_view& input) {
        string word;
        for (int i = 0; i < input.size(); ++i) {
            if (iswspace(input[i]) != 0) 
            {
                if (!word.empty()) 
                {
                    input.remove_prefix(i);
                    goto exit;
                }
            }
            else 
            {
                word.push_back(input[i]);
            }
        }
        input = "";
    exit:
        return word;
    }
    /// <summary>
    /// parses a string from a beginning of the input until a specified character is encountered
    /// or until the end of a string
    /// changes input, removing parsed part and a specified character
    /// </summary>
    /// <param name="input">string to parse</param><param name="ch">specified character</param>
    /// <returns>parsed part</returns>
    string parse_until(string_view& input, char ch)
    {   
        string word;
        for (int i = 0; i < input.size(); ++i) {
            if (input[i] == ch) 
            {
                input.remove_prefix(i + 1);
                goto exit;
            }
            else 
            {
                word.push_back(input[i]);
            }
        }
        input = "";
    exit:
        return word;
    }

    void trim_start(string_view& input)
    {
        for (auto ch = input.begin(); ch != input.end(); ++ch) {
            if (iswspace(*ch) != 0) 
            {
                input.remove_prefix(1);
            }
            else 
            {
                return;
            }
        }
    }

    /// <summary>
    /// parses symbols, equivalent to the first symbol of a string, until any other symbol is encountered
    /// moves the beginning of the input on the next character after parsed part
    /// </summary>
    /// <param name="input">string to parse</param>
    /// <returns>parsed part</returns>
    string parse_until_changes(string_view& input) {
        auto first_symbol = input.at(0);
        string word { first_symbol };
        for (int i = 1; i < input.size(); ++i) {
            if (input[i] != first_symbol) {
                input.remove_prefix(i);
                goto exit;
            }
            word.push_back(first_symbol);
        }
        input = "";
    exit:
        return word;
    }
} // namespace  StringParsers



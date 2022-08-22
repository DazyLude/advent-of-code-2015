#include <string>

using std::string;

/// <summary>
/// parses a string part surrounded by whitespaces, starting from beginning
/// changes input, removing parsed part and preceeding whitespaces
/// if no whitespaces are encountered, returns input, leaving it empty
/// </summary>
/// <param name="input">string to parse</param>
/// <returns>parsed part</returns>
string parse_word(string& input) {
    string word;
    for (int i = 0; i < input.size(); ++i) {
        if (iswspace(input[i]) != 0) {
            if (!word.empty()) {
                input = input.substr(i, input.size() - i);
                goto exit;
            }
        }
        else {
            word.push_back(input[i]);
        }
    }
    input = "";
exit:
    return word;
};

/// <summary>
/// parses symbols, equivalent to the first symbol of a string, until any other symbol is encountered
/// empties input, input without the remainder goes to buf
/// </summary>
/// <param name="input">string to parse</param>
/// <returns>parsed part</returns>
string parse_until_changes(string& input) {
    if (input.empty()) { return input; }
    auto first_symbol = input.at(0);
    string word { first_symbol };
    for (int i = 1; i < input.size(); ++i) {
        if (input[i] != first_symbol) {
            input = input.substr(i, input.size() - i);
            goto exit;
        }
        word.push_back(first_symbol);
    }
    input = "";
exit:
    return word;
}
export module Parser;

import <string>;

using std::string;

export string parse_word(string& input) {
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
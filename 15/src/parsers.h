#pragma once
#include <string>
#include <string_view>
#include <iostream>

namespace FileParsers
{
    void open_input(std::ifstream& ifs);
}

namespace StringParsers
{
    std::string parse_word(std::string_view& input);
    std::string parse_until_changes(std::string_view& input);
}
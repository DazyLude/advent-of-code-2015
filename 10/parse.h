#pragma once
#include <string>
#include <string_view>

std::string parse_word(std::string_view& input);
std::string parse_until_changes(std::string_view& input);
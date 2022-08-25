#include <string>
#include <iostream>
#include "parse.h"

std::string xx_to_2x(std::string& input) {
    return std::to_string(input.size()) + input.at(0);
}

std::string& look_and_say(std::string& input, std::string& output, int count, int max_count) {
    if (count >= max_count) { return input; }
    std::cout << "now at " << count << " out of " << max_count << std::endl;
    std::string_view input_view{ input };
    while (!input_view.empty()) {
        std::string temp = parse_until_changes(input_view);
        output += xx_to_2x(temp);
    }
    input.erase();
    return look_and_say(output, input, ++count, max_count);
}

int main() {
    std::string input = "3113322113";
    std::string output {};
    auto result = look_and_say(input, output, 0, 50);
    std::cout << result.size();
    return 0;
}
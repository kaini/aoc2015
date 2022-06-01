#include <iostream>

int main() {
    std::string string;
    std::cin >> string;

    for (int i = 0; i < 50; ++i) {
        int current_count = 0;
        char current_char = 0;
        std::string new_string = "";
        for (char c : string) {
            if (current_char != c) {
                if (current_char) {
                    new_string += std::to_string(current_count) + current_char;
                }
                current_char = c;
                current_count = 0;
            }
            current_count += 1;
        }
        if (current_char) {
            new_string += std::to_string(current_count) + current_char;
        }
        std::swap(string, new_string);
        if (i == 39) {
            std::cout << string.length() << std::endl;
        }
    }
    std::cout << string.length() << std::endl;

    return 0;
}
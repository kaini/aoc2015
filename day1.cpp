#include <iostream>
#include <string>
#include <cassert>

int main() {
    std::string input;
    std::cin >> input;

    int level = 0;
    size_t first_basement = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        if (c == '(') level += 1;
        else if (c == ')') level -= 1;
        else assert(false);

        if (first_basement == 0 && level == -1) {
            first_basement = i + 1;
        }
    }
    std::cout << level << '\n' << first_basement << std::endl;

    return 0;
}

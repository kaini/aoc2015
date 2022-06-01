#include <iostream>
#include <set>
#include <tuple>

using point = std::tuple<int, int>;

static void move(point& at, char command) {
    if (command == '<') std::get<0>(at) -= 1;
    else if (command == '>') std::get<0>(at) += 1;
    else if (command == '^') std::get<1>(at) -= 1;
    else if (command == 'v') std::get<1>(at) += 1;
    else assert(false);
}

int main() {
    std::string commands;
    std::cin >> commands;

    point at = {0,0};
    std::set<point> visited;
    visited.insert(at);
    for (char command : commands) {
        move(at, command);
        visited.insert(at);
    }
    std::cout << visited.size() << std::endl;

    at = {0,0};
    point at2 = {0,0};
    bool even = true;
    visited.clear();
    for (char command : commands) {
        if (even) {
            even = false;
            move(at, command);
            visited.insert(at);
        } else {
            even = true;
            move(at2, command);
            visited.insert(at2);
        }
    }
    std::cout << visited.size() << std::endl;

    return 0;
}
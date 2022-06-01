#include <iostream>
#include <set>
#include <tuple>

using point = std::tuple<int, int>;

std::set<point> run(std::set<point> on, bool corners_on) {
    std::set<point> next;
    for (int step = 0; step < 100; ++step) {
        for (int x = 0; x < 100; ++x) {
            for (int y = 0; y < 100; ++y) {
                int neighbours = 0;
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        if ((dx != 0 || dy != 0) && on.find({x + dx, y + dy}) != on.end()) {
                            neighbours += 1;
                        }
                    }
                }
                if (on.find({x, y}) != on.end()) {
                    if (neighbours == 2 || neighbours == 3) {
                        next.emplace(x, y);
                    }
                } else {
                    if (neighbours == 3) {
                        next.emplace(x, y);
                    }
                }
            }
        }
        if (corners_on) {
            next.emplace(0, 0);
            next.emplace(0, 99);
            next.emplace(99, 0);
            next.emplace(99, 99);
        }
        std::swap(on, next);
        next.clear();
    }
    return on;
}

int main() {
    std::set<point> on;
    for (int y = 0; y < 100; ++y) {
        for (int x = 0; x < 100; ++x) {
            char c;
            std::cin.get(c);
            if (c == '#') {
                on.emplace(x, y);
            } else if (c == '.') {
                // do nothing
            } else {
                assert(false);
            }
        }
        std::string newline;
        std::getline(std::cin, newline);  // eat the newline
    }

    std::cout << run(on, false).size() << std::endl;
    std::cout << run(on, true).size() << std::endl;

    return 0;
}
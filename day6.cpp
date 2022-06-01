#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <numeric>

enum class action {
    off, on, toggle,
};

struct point : std::tuple<int, int> {
    point() : std::tuple<int, int>(0, 0) {}
    point(int x, int y) : std::tuple<int, int>(x, y) {}
    int& x() { return std::get<0>(*this); }
    int& y() { return std::get<1>(*this); }
    const int& x() const { return std::get<0>(*this); }
    const int& y() const { return std::get<1>(*this); }
};

struct instruction {
    action action;
    point from;
    point to;
};

int main() {
    std::vector<instruction> instrs;
    for (;;) {
        instruction instr;

        std::string command_start;
        std::cin >> command_start;
        if (std::cin.eof()) {
            break;
        }
        std::string on_off;
        if (command_start == "turn") {
            std::cin >> on_off;
            instr.action = on_off == "on" ? action::on : action::off;
        } else if (command_start == "toggle") {
            instr.action = action::toggle;
        } else {
            assert(false);
        }

        char comma;
        std::string through;
        std::cin >> instr.from.x() >> comma >> instr.from.y() >> through >> instr.to.x() >> comma >> instr.to.y();

        instrs.push_back(instr);
    }

    std::set<point> on;
    for (const auto& instr : instrs) {
        for (int x = instr.from.x(); x <= instr.to.x(); ++x) {
            for (int y = instr.from.y(); y <= instr.to.y(); ++y) {
                switch (instr.action) {
                    case action::on:
                        on.insert({x,y});
                        break;
                    case action::off:
                        on.erase({x,y});
                        break;
                    case action::toggle:
                        auto iter = on.find({x,y});
                        if (iter != on.end()) {
                            on.erase(iter);
                        } else {
                            on.insert(iter, {x,y});
                        }
                        break;
                }
            }
        }
    }
    std::cout << on.size() << std::endl;

    std::map<point, int> brightness;
    for (const auto& instr : instrs) {
        for (int x = instr.from.x(); x <= instr.to.x(); ++x) {
            for (int y = instr.from.y(); y <= instr.to.y(); ++y) {
                switch (instr.action) {
                    case action::on:
                        brightness[{x,y}] += 1;
                        break;
                    case action::off:
                        if ((brightness[{x,y}] -= 1) < 0) {
                            brightness[{x,y}] = 0;
                        }
                        break;
                    case action::toggle:
                        brightness[{x,y}] += 2;
                        break;
                }
            }
        }
    }
    std::cout << std::accumulate(brightness.begin(), brightness.end(), 0, [](int result, auto entry) {
        return result + entry.second;
    }) << std::endl;

    return 0;
}
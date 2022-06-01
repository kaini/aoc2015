#include <iostream>
#include <vector>

struct present {
    int x, y, z;
};

int main() {
    std::vector<present> input;
    for (;;) {
        present p;
        char x;
        std::cin >> p.x >> x >> p.y >> x >> p.z;
        if (std::cin.eof()) break;
        input.push_back(p);
    }

    int area = 0;
    for (const present& present : input) {
        int a = present.x * present.y;
        int b = present.x * present.z;
        int c = present.y * present.z;
        area += 2 * a + 2 * b + 2 * c + std::min({a, b, c});
    }
    std::cout << area << std::endl;

    int ribbon = 0;
    for (const present& present : input) {
        ribbon += present.x * present.y * present.z + 2 * (present.x + present.y + present.z - std::max({present.x, present.y, present.z}));
    }
    std::cout << ribbon << std::endl;

    return 0;
}
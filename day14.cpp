#include <iostream>
#include <vector>

static const constexpr int seconds = 2503;

struct reindeer {
    std::string name;
    int speed;
    int fly_time;
    int rest_time;
    int points = 0;
    int at = 0;
};

int main() {
    std::vector<reindeer> reindeers;
    for (;;) {
        // Comet can fly 14 km/s for 10 seconds, but then must rest for 127 seconds.
        std::string dummy;
        reindeer reindeer;
        std::cin >> reindeer.name >> dummy >> dummy >> reindeer.speed >> dummy >> dummy >> reindeer.fly_time
                >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> reindeer.rest_time >> dummy;
        if (std::cin.eof()) {
            break;
        }
        reindeers.push_back(reindeer);
    }

    int max_dist = std::numeric_limits<int>::min();
    for (const auto& r : reindeers) {
        int cycle = seconds / (r.fly_time + r.rest_time);
        int rest = seconds % (r.fly_time + r.rest_time);
        int dist = (cycle * r.fly_time + std::min(rest, r.fly_time)) * r.speed;
        max_dist = std::max(max_dist, dist);
    }
    std::cout << max_dist << std::endl;

    int max_points = std::numeric_limits<int>::min();
    int max_at = std::numeric_limits<int>::min();
    for (int i = 0; i < seconds; ++i) {
        for (auto& r : reindeers) {
            if (i % (r.fly_time + r.rest_time) < r.fly_time) {
                r.at += r.speed;
                max_at = std::max(max_at, r.at);
            }
        }
        for (auto& r : reindeers) {
            if (r.at == max_at) {
                r.points += 1;
                max_points = std::max(max_points, r.points);
            }
        }
    }
    std::cout << max_points << std::endl;

    return 0;
}

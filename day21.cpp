#include <iostream>
#include <vector>

struct fighter {
    int hp;
    int dmg;
    int def;
    int cost;
};

struct item {
    int cost;
    int dmg;
    int def;
};

static const std::vector<item> weapons{
    {8, 4, 0},
    {10, 5, 0},
    {25, 6, 0},
    {40, 7, 0},
    {74, 8, 0},
};

static const std::vector<item> armors{
    {0, 0, 0},
    {13, 0, 1},
    {31, 0, 2},
    {53, 0, 3},
    {75, 0, 4},
    {102, 0, 5},
};

static const std::vector<item> rings{
    {0, 0, 0},
    {0, 0, 0},
    {25, 1, 0},
    {50, 2, 0},
    {100, 3, 0},
    {20, 0, 1},
    {40, 0, 2},
    {80, 0, 3},
};

static fighter items_to_player() {
    return {100, 0, 0, 0};
}

template <typename... Items>
static fighter items_to_player(const item& item, const Items&... items) {
    auto player = items_to_player(items...);
    player.dmg += item.dmg;
    player.def += item.def;
    player.cost += item.cost;
    return player;
}

// Returns true if a wins, false otherwise.
static bool fight(fighter a, fighter b) {
    for (;;) {
        b.hp -= std::max(1, a.dmg - b.def);
        if (b.hp <= 0) return true;
        a.hp -= std::max(1, b.dmg - a.def);
        if (a.hp <= 0) return false;
    }
}

int main() {
    std::string dummy;
    fighter boss;
    std::cin >> dummy >> dummy >> boss.hp;
    std::cin >> dummy >> boss.dmg;
    std::cin >> dummy >> boss.def;
    boss.cost = 0;

    int min_gold = std::numeric_limits<int>::max();
    int max_gold = std::numeric_limits<int>::min();
    for (const item& weapon : weapons) {
        for (const item& armor : armors) {
            for (size_t ring1 = 0; ring1 < rings.size(); ++ring1) {
                for (size_t ring2 = ring1 + 1; ring2 < rings.size(); ++ring2) {
                    fighter player = items_to_player(weapon, armor, rings[ring1], rings[ring2]);
                    bool player_wins = fight(player, boss);
                    if (player.cost < min_gold && player_wins) {
                        min_gold = player.cost;
                    }
                    if (player.cost > max_gold && !player_wins) {
                        max_gold = player.cost;
                    }
                }
            }
        }
    }
    std::cout << min_gold << std::endl;
    std::cout << max_gold << std::endl;

    return 0;
}
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <algorithm>

struct state {
    int boss_hp = 0;
    int boss_damage = 0;

    int player_hp = 50;
    int player_mana = 500;
    int shield_active = 0;
    int poison_active = 0;
    int recharge_active = 0;
};

static bool frontier_cmp(const std::tuple<state, int>& a, const std::tuple<state, int>& b) {
    return std::get<int>(a) > std::get<int>(b);
}

static bool operator==(const state& a, const state& b) {
    return a.boss_hp == b.boss_hp &&
            a.boss_damage == b.boss_damage &&
            a.player_hp == b.player_hp &&
            a.player_mana == b.player_mana &&
            a.shield_active == b.shield_active &&
            a.poison_active == b.poison_active &&
            a.recharge_active == b.recharge_active;
}

namespace std {
template<> struct hash<state> {
    size_t operator()(const state& s) const {
        return std::hash<int>()(s.boss_hp) |
                std::hash<int>()(s.boss_damage) |
                std::hash<int>()(s.player_hp) |
                std::hash<int>()(s.player_mana) |
                std::hash<int>()(s.shield_active) |
                std::hash<int>()(s.poison_active) |
                std::hash<int>()(s.recharge_active);
    }
};
}

static int djikstra(state initial_state, int penalty) {
    initial_state.player_hp -= penalty;

    std::vector<std::tuple<state, int>> frontier;
    frontier.push_back({initial_state, 0});
    std::unordered_map<state, int> cost;
    cost[initial_state] = 0;
    while (!frontier.empty()) {
        std::pop_heap(frontier.begin(), frontier.end(), frontier_cmp);
        auto popped = frontier.back();
        frontier.pop_back();
        state current_state = std::get<state>(popped);
        int current_cost = std::get<int>(popped);

        if (current_state.boss_hp <= 0) {
            return current_cost;
        }

        std::vector<std::tuple<state, int>> next_states;
        // Magic Missile
        if (current_state.player_mana >= 53) {
            state next = current_state;
            next.player_mana -= 53;
            next.boss_hp -= 4;
            next_states.push_back({next, 53});
        }
        // Drain
        if (current_state.player_mana >= 73) {
            state next = current_state;
            next.player_mana -= 73;
            next.player_hp += 2;
            next.boss_hp -= 2;
            next_states.push_back({next, 73});
        }
        // Shield
        if (current_state.player_mana >= 113 && !current_state.shield_active) {
            state next = current_state;
            next.player_mana -= 113;
            next.shield_active = 6;
            next_states.push_back({next, 113});
        }
        // Poison
        if (current_state.player_mana >= 173 && !current_state.poison_active) {
            state next = current_state;
            next.player_mana -= 173;
            next.poison_active = 6;
            next_states.push_back({next, 173});
        }
        // Recharge
        if (current_state.player_mana >= 229 && !current_state.recharge_active) {
            state next = current_state;
            next.player_mana -= 229;
            next.recharge_active = 5;
            next_states.push_back({next, 229});
        }

        for (auto& next_tuple : next_states) {
            state& next = std::get<state>(next_tuple);

            // Handle the bosses turn
            int player_defense = 0;
            if (next.shield_active > 0) {
                next.shield_active -= 1;
                player_defense = 7;
            }
            if (next.poison_active > 0) {
                next.poison_active -= 1;
                next.boss_hp -= 3;
            }
            if (next.recharge_active > 0) {
                next.recharge_active -= 1;
                next.player_mana += 101;
            }
            if (next.boss_hp > 0) {
                next.player_hp -= std::max(1, next.boss_damage - player_defense);
            }

            // Handle start of player's turn
            next.player_hp -= penalty;
            if (next.shield_active > 0) {
                next.shield_active -= 1;
            }
            if (next.poison_active > 0 && next.player_hp > 0) {
                next.poison_active -= 1;
                next.boss_hp -= 3;
            }
            if (next.recharge_active > 0) {
                next.recharge_active -= 1;
                next.player_mana += 101;
            }
        }

        for (const auto& next_tuple : next_states) {
            const state& next = std::get<state>(next_tuple);
            if (next.player_hp <= 0) {
                continue;
            }
            const int used_mana = std::get<int>(next_tuple);
            int next_cost = current_cost + used_mana;
            auto cost_iter = cost.find(next);
            if (cost_iter == cost.end() || next_cost < cost_iter->second) {
                cost[next] = next_cost;
                frontier.push_back({next, next_cost});
                std::push_heap(frontier.begin(), frontier.end(), frontier_cmp);
            }
        }
    }

    return -1;
}

int main() {
    state initial_state;
    std::string dummy;
    std::cin >> dummy >> dummy >> initial_state.boss_hp;
    std::cin >> dummy >> initial_state.boss_damage;

    std::cout << djikstra(initial_state, 0) << std::endl;
    std::cout << djikstra(initial_state, 1) << std::endl;

    return 0;
}

#include <iostream>
#include <vector>

struct ingredient {
    std::string name;
    int capacity;
    int durability;
    int flavor;
    int texture;
    int calories;
};

int combine(
    const std::vector<ingredient>& ingredients,
    std::vector<int>& counts,
    int remaining,
    int goal_calories = -1
) {
    if (ingredients.size() == counts.size()) {
        int capacity = 0, durability = 0, flavor = 0, texture = 0, calories = 0;
        for (size_t i = 0; i < ingredients.size(); ++i) {
            const auto& in = ingredients[i];
            int c = counts[i];
            capacity += c * in.capacity;
            durability += c * in.durability;
            flavor += c * in.flavor;
            texture += c * in.texture;
            calories += c * in.calories;
        }
        if (goal_calories != -1 && calories != goal_calories) {
            return 0;
        }
        return std::max(0, capacity) * std::max(0, durability) * std::max(0, flavor) * std::max(0, texture);
    }

    if (ingredients.size() - 1 == counts.size()) {
        counts.push_back(remaining);
        int result = combine(ingredients, counts, 0, goal_calories);
        counts.erase(counts.end() - 1);
        return result;
    }

    counts.push_back(0);
    int result = 0;
    for (int i = 0; i <= remaining; ++i) {
        *(counts.end() - 1) = i;
        result = std::max(result, combine(ingredients, counts, remaining - i, goal_calories));
    }
    counts.erase(counts.end() - 1);
    return result;
}

int main() {
    std::vector<ingredient> ingredients;
    for (;;) {
        // Frosting: capacity 4, durability -2, flavor 0, texture 0, calories 5
        std::string dummy;
        ingredient i;
        std::cin >> i.name >> dummy
                >> i.capacity >> dummy >> dummy
                >> i.durability >> dummy >> dummy 
                >> i.flavor >> dummy >> dummy
                >> i.texture >> dummy >> dummy
                >> i.calories;
        if (std::cin.eof()) {
            break;
        }
        i.name.erase(i.name.end() - 1);
        ingredients.push_back(i);
    }

    std::vector<int> counts;
    std::cout << combine(ingredients, counts, 100) << std::endl;
    std::cout << combine(ingredients, counts, 100, 500) << std::endl;

    return 0;
}

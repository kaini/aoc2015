#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

static const constexpr int goal = 150;

std::unordered_map<int, int> count(const std::vector<int>& sizes, int remaining, int containers, size_t at) {
    if (remaining < 0) {
        return {};
    } else if (remaining == 0) {
        return {{containers, 1}};
    } else if (at == sizes.size()) {
        return {};
    } else {
        auto a = count(sizes, remaining, containers, at + 1);  // don't use the container
        const auto b = count(sizes, remaining - sizes[at], containers + 1, at + 1);  // use the container
        for (const auto& entry : b) {
            a[entry.first] += entry.second;
        }
        return a;
    }
}

int main() {
    std::vector<int> sizes;
    for (int n; std::cin >> n; sizes.push_back(n))
        ;

    std::sort(sizes.begin(), sizes.end(), std::greater<>());
    auto result = count(sizes, goal, 0, 0);
    int total_count = 0;
    int min_containers = std::numeric_limits<int>::max();
    for (const auto& entry : result) {
        total_count += entry.second;
        min_containers = std::min(min_containers, entry.first);
    }
    std::cout << total_count << std::endl;
    std::cout << result.at(min_containers) << std::endl;

    return 0;
}
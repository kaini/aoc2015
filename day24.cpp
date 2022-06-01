#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

static void branch_and_cut(
    const std::vector<int>& weights,
    int goal_weight,
    int containers,
    std::vector<int>& solution,
    int& best_count,
    long double& best_qe
) {
    int container_weights[containers];
    for (int& cw : container_weights) {
        cw = 0;
    }
    int container_one_count = 0;
    long double container_one_qe = 1;
    for (size_t i = 0; i < solution.size(); ++i) {
        container_weights[solution[i]] += weights[i];
        if (solution[i] == 0) {
            container_one_count += 1;
            container_one_qe *= weights[i];
        }
    }
    // Cut all branches where the weight is too high for a container
    for (int c = 0; c < containers; ++c) {
        if (container_weights[c] > goal_weight) {
            return;
        }
    }
    // Cut all branches where the count is too high
    if (container_one_count > best_count) {
        return;
    }
    // Cut all branches where the qe is too high
    if (container_one_qe >= best_qe) {
        return;
    }
    // Test if the remaining packages (asserts that they are sorted highest to lowest) are enough to fill container one
    if (best_count != std::numeric_limits<int>::max() && container_weights[0] != goal_weight) {
        int sum = std::accumulate(
            weights.begin() + solution.size(),
            weights.begin() + solution.size() + std::min(weights.size() - solution.size(), static_cast<size_t>(best_count - container_one_count)),
            0
        );
        if (sum < goal_weight - container_weights[0]) {
            return;
        }
    }

    // The weight is distributed well.
    if (solution.size() == weights.size()) {
        best_count = container_one_count;
        best_qe = container_one_qe;
        std::cerr << "Found a solution count=" << best_count << " qe=" << best_qe << std::endl;
    }

    // Try all possiblities
    for (int container = 0; container < containers; ++container) {
        solution.push_back(container);
        branch_and_cut(weights, goal_weight, containers, solution, best_count, best_qe);
        solution.pop_back();
    }
}

int main() {
    std::vector<int> weights;
    for (int w; std::cin >> w; weights.push_back(w))
        ;
    int total_weight = std::accumulate(weights.begin(), weights.end(), 0);
    assert(total_weight % 3 == 0);

    // I want to minimize the amount of packets in the first container,
    // therefore sorting from largest to smallest makes sense.
    std::sort(weights.begin(), weights.end(), std::greater<>());
    std::vector<int> solution;
    int best_count = std::numeric_limits<int>::max();
    long double best_qe = std::numeric_limits<long double>::max();
    branch_and_cut(weights, total_weight / 3, 3, solution, best_count, best_qe);
    std::cout << static_cast<long long>(best_qe) << std::endl;
    
    best_count = std::numeric_limits<int>::max();
    best_qe = std::numeric_limits<long double>::max();
    branch_and_cut(weights, total_weight / 4, 4, solution, best_count, best_qe);
    std::cout << static_cast<long long>(best_qe) << std::endl;

    return 0;
}
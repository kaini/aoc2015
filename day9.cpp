#include <iostream>
#include <map>
#include <unordered_set>
#include <tuple>
#include <optional>

using edge = std::tuple<std::string, std::string>;

int find_result(
        const std::unordered_set<std::string>& points,
        const std::map<edge, int>& edges,
        const std::optional<std::string>& prev_point,
        bool shortest) {
    if (points.empty()) {
        return 0;
    }

    std::unordered_set<std::string> next_points = points;
    int result_dist = shortest ? std::numeric_limits<int>::max() : std::numeric_limits<int>::min();
    for (const auto& point : points) {
        next_points.erase(point);
        int dist = find_result(next_points, edges, point, shortest);
        if (prev_point.has_value()) {
            dist += edges.at({*prev_point, point});
        }
        next_points.insert(point);

        result_dist = shortest ? std::min(result_dist, dist) : std::max(result_dist, dist);
    }

    return result_dist;
}

int main() {
    std::map<edge, int> edges;
    std::unordered_set<std::string> points;
    std::string source, to, dest, equals;
    int dist;
    while (std::cin >> source >> to >> dest >> equals >> dist) {
        edges[{source, dest}] = dist;
        edges[{dest, source}] = dist;
        points.insert(source);
        points.insert(dest);
    }

    std::cout << find_result(points, edges, std::optional<std::string>(), true) << std::endl;
    std::cout << find_result(points, edges, std::optional<std::string>(), false) << std::endl;

    return 0;
}
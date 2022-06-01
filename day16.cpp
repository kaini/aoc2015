#include <iostream>
#include <unordered_map>
#include <vector>
#include <regex>
#include <functional>

static const std::regex attribute("([a-z]+): ([0-9]+)");

using op = std::function<bool(int, int)>;
static const op eq = [](int a, int b) { return a == b; };
static const op lt = [](int a, int b) { return a < b; };
static const op gt = [](int a, int b) { return a > b; };

static int find(
    const std::unordered_map<std::string, int>& reference,
    const std::unordered_map<std::string, op>& ops,
    const std::vector<std::unordered_map<std::string, int>>& sues
) {
    for (size_t i = 0; i < sues.size(); ++i) {
        bool matches = true;
        for (const auto& attr : sues[i]) {
            if (!ops.at(attr.first)(attr.second, reference.at(attr.first))) {
                matches = false;
                break;
            }
        }
        if (matches) {
            return i + 1;
        }
    }
    return -1;
}

int main() {
    std::unordered_map<std::string, int> reference;
    reference["children"] = 3;
    reference["cats"] = 7;
    reference["samoyeds"] = 2;
    reference["pomeranians"] = 3;
    reference["akitas"] = 0;
    reference["vizslas"] = 0;
    reference["goldfish"] = 5;
    reference["trees"] = 3;
    reference["cars"] = 2;
    reference["perfumes"] = 1;

    std::unordered_map<std::string, op> ops1;
    ops1["children"] = eq;
    ops1["cats"] = eq;
    ops1["samoyeds"] = eq;
    ops1["pomeranians"] = eq;
    ops1["akitas"] = eq;
    ops1["vizslas"] = eq;
    ops1["goldfish"] = eq;
    ops1["trees"] = eq;
    ops1["cars"] = eq;
    ops1["perfumes"] = eq;

    std::unordered_map<std::string, op> ops2;
    ops2["children"] = eq;
    ops2["cats"] = gt;
    ops2["samoyeds"] = eq;
    ops2["pomeranians"] = lt;
    ops2["akitas"] = eq;
    ops2["vizslas"] = eq;
    ops2["goldfish"] = lt;
    ops2["trees"] = gt;
    ops2["cars"] = eq;
    ops2["perfumes"] = eq;

    std::vector<std::unordered_map<std::string, int>> sues;
    for (std::string line; std::getline(std::cin, line);) {
        std::unordered_map<std::string, int> sue;
        std::smatch match;
        while (std::regex_search(line, match, attribute)) {
            sue[match[1]] = std::stoi(match[2]);
            line = match.suffix();
        }
        sues.emplace_back(std::move(sue));
    }

    std::cout << find(reference, ops1, sues) << std::endl;
    std::cout << find(reference, ops2, sues) << std::endl;

    return 0;
}

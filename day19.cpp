#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <regex>

static const std::regex rule_regex("(.+) => (.+)");

struct reduction {
    size_t begin;
    const std::string& replacement;
    const std::string& source;
};

static void search(
    const std::unordered_map<std::string, std::vector<std::string>>& rules,
    int max_replacement_size,
    std::string& molecule,
    int start,
    std::unordered_set<std::string>& seen,
    int steps,
    int& min_steps
) {
    if (seen.find(molecule) != seen.end()) {
        return;
    }

    if (steps >= min_steps) {
        return;
    }

    if (molecule == "e") {
        std::cout << "found at " << steps << " steps" << std::endl;
        std::cout << "Note: 200 is the correct soltution but the search will not terminate. "
            << "Even after searching the internet there does not seem to be a general solution that terminates. "
            << "Some people had success with a greedy approach, but this does not seem to work with all inputs. "
            << "So, we are left with that :)"
            << std::endl;
        min_steps = steps;
        return;
    }

    std::vector<reduction> reductions;
    for (const auto &rule : rules) {
        for (const auto& replacement : rule.second) {
            if (rule.first == "e" && molecule.size() != replacement.size()) {
                // Only allow "e" as a possible last step.
                continue;
            }
            size_t at = start;
            while (at < molecule.size()) {
                at = molecule.find(replacement, at);
                if (at < molecule.size()) {
                    reductions.push_back({at, replacement, rule.first});
                    at += 1;
                }
            }
        }
    }

    for (const auto& r : reductions) {
        molecule.erase(r.begin, r.replacement.size());
        molecule.insert(r.begin, r.source);
        search(
            rules,
            max_replacement_size,
            molecule,
            std::max(0, static_cast<int>(r.begin) - max_replacement_size),
            seen,
            steps + 1,
            min_steps
        );
        molecule.erase(r.begin, r.source.size());
        molecule.insert(r.begin, r.replacement);
    }

    if (reductions.empty()) {
        seen.emplace(molecule);
    }
}

int main() {
    // This solution assumes, that "e" does not occur in any molecule name. This is the case.

    std::unordered_map<std::string, std::vector<std::string>> rules;
    for (std::string line; std::getline(std::cin, line) && !line.empty();) {
        std::smatch match;
        bool matches = std::regex_match(line, match, rule_regex);
        assert(matches);
        rules[match[1].str()].emplace_back(match[2].str());
    }

    std::string molecule;
    std::getline(std::cin, molecule);
    
    std::unordered_set<std::string> molecules;
    for (const auto& rule : rules) {
        size_t at = 0;
        while (at < molecule.size()) {
            at = molecule.find(rule.first, at);
            if (at < molecule.size()) {
                molecule.erase(at, rule.first.size());
                for (const auto& replacement : rule.second) {
                    molecule.insert(at, replacement);
                    molecules.emplace(molecule);
                    molecule.erase(at, replacement.size());
                }
                molecule.insert(at, rule.first);
                at += 1;
            }
        }
    }
    std::cout << molecules.size() << std::endl;

    int max_replacement_size = 0;
    for (const auto& rule : rules) {
        for (const auto& replacement : rule.second) {
            max_replacement_size = std::max(max_replacement_size, static_cast<int>(replacement.size()));
        }
    }

    std::unordered_set<std::string> seen;
    int min_steps = std::numeric_limits<int>::max();
    search(rules, max_replacement_size, molecule, 0, seen, 0, min_steps);

    return 0;
}

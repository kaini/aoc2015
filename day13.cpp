#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <tuple>

int permutate(
        const std::set<std::string>& persons,
        std::vector<std::string>& permutation,
        const std::map<std::tuple<std::string, std::string>, int>& happiness
) {
    if (persons.empty()) {
        int result = 0;
        for (size_t i = 0; i < permutation.size(); ++i) {
            size_t next = i + 1;
            if (next >= permutation.size()) next = 0;
            result += happiness.at({permutation[i], permutation[next]});

            size_t prev = i - 1;
            if (prev == (size_t)-1) prev = permutation.size() - 1;
            result += happiness.at({permutation[i], permutation[prev]});
        }
        return result;
    }

    auto next_persons = persons;
    int result = std::numeric_limits<int>::min();
    for (const auto& person : persons) {
        next_persons.erase(person);
        permutation.push_back(person);
        
        result = std::max(result, permutate(next_persons, permutation, happiness));

        permutation.erase(permutation.end() - 1);
        next_persons.insert(person);
    }
    return result;
}

int main() {
    std::map<std::tuple<std::string, std::string>, int> happiness;
    std::set<std::string> persons;
    for (;;) {
        std::string subject, neighbour, gain_lose, dummy;
        int amount;
        std::cin >> subject >> dummy >> gain_lose >> amount >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> neighbour;
        if (std::cin.eof()) {
            break;
        }
        neighbour.erase(neighbour.end() - 1);  // remove the .
        happiness[{subject, neighbour}] = (gain_lose == "gain" ? 1 : -1) * amount;
        persons.insert(subject);
    }

    std::vector<std::string> permutation;
    int result = permutate(persons, permutation, happiness);
    std::cout << result << std::endl;

    for (const auto& person : persons) {
        happiness[{"Me", person}] = 0;
        happiness[{person, "Me"}] = 0;
    }
    persons.insert("Me");
    std::cout << permutate(persons, permutation, happiness) << std::endl;

    return 0;
}
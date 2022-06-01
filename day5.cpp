#include <regex>
#include <iostream>
#include <vector>

int main() {
    std::vector<std::string> words;
    for (;;) {
        std::string word;
        std::cin >> word;
        if (std::cin.eof()) {
            break;
        }
        words.push_back(word);
    }

    std::regex three_vowels("[aeiou].*[aeiou].*[aeiou]");
    std::regex one_twice("(.)\\1");
    std::regex bad_words("ab|cd|pq|xy");
    std::cout << std::count_if(words.begin(), words.end(), [&](const auto& word) {
        return std::regex_search(word, three_vowels) && std::regex_search(word, one_twice) && !std::regex_search(word, bad_words);
    }) << std::endl;

    std::regex pair("(..).*\\1");
    std::regex letter_pair_with_sep("(.).\\1");
    std::cout << std::count_if(words.begin(), words.end(), [&](const auto& word) {
        return std::regex_search(word, pair) && std::regex_search(word, letter_pair_with_sep);
    }) << std::endl;

    return 0;
}
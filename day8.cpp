#include <iostream>
#include <vector>

int main() {
    std::vector<std::string> lines;
    std::string line;
    while (std::cin >> line) {
        lines.push_back(line);
    }

    size_t total_len = 0;
    size_t parsed_len = 0;
    for (const auto& line : lines) {
        total_len += line.size();
        for (size_t i = 1; i < line.length() - 1; ++i) {
            if (line[i] == '\\') {
                if (line[i + 1] == 'x') {
                    i += 3;
                } else {
                    i += 1;
                }
            }
            parsed_len += 1;
        }
    }
    std::cout << total_len - parsed_len << std::endl;

    size_t encoded_len = 0;
    for (const auto& line : lines) {
        encoded_len += 2;
        for (char c : line) {
            if (c == '"' || c == '\\') {
                encoded_len += 2;
            } else {
                encoded_len += 1;
            }
        }
    }
    std::cout << encoded_len - total_len << std::endl;

    return 0;
}
#include <iostream>
#include <regex>

void increment(std::string& password, int at = 1) {
    char result = password[password.length() - at] += 1;
    if (result == 'z' + 1) {
        password[password.length() - at] = 'a';
        increment(password, at + 1);
    }
}

static std::regex increasing("abc|bcd|cde|def|efg|fgh|ghi|hij|ijk|jkl|klm|lmn|mno|nop|opq|pqr|qrs|rst|stu|tuv|uvw|vwx|wxy|xyz");
static std::regex confusing("[iol]");
static std::regex pairs("(.)\\1.*(.)\\2");

void find_next(std::string& password) {
    do {
        increment(password);
    } while (!(std::regex_search(password, increasing) && !std::regex_search(password, confusing) && std::regex_search(password, pairs)));
}

int main() {
    std::string password;
    std::cin >> password;

    find_next(password);
    std::cout << password << std::endl;
    
    find_next(password);
    std::cout << password << std::endl;

    return 0;
}
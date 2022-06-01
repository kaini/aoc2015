#include <iostream>
#include <unordered_map>
#include <variant>
#include <vector>

using jstring = std::string;
using jnumber = int;
struct jarray;
struct jobject;
using jvalue = std::variant<jstring, jnumber, jarray, jobject>;

struct jarray : std::vector<jvalue> {};
struct jobject : std::unordered_map<std::string, jvalue> {};

void skip_whitespace(const std::string& string, size_t& at) {
    while (at < string.length() && isspace(string.at(at)))
        at += 1;
}

jvalue parse_value(const std::string& string, size_t& at);

jstring parse_string(const std::string& string, size_t& at) {
    jstring result;
    char c;
    while ((c = string.at(++at)) != '"')
        result += c;
    at += 1;
    return result;
}

jnumber parse_number(const std::string& string, size_t& at) {
    std::string number;
    do {
        number += string.at(at);
        at += 1;
    } while ('0' <= string.at(at) && string.at(at) <= '9');
    return std::stoi(number);
}

jarray parse_array(const std::string& string, size_t& at) {
    at += 1;  // skip [
    skip_whitespace(string, at);

    jarray result;
    bool first = true;
    for (;;) {
        if (string.at(at) == ']') {
            at += 1;
            break;
        } else if (string.at(at) == ',' || first) {
            if (first) {
                first = false;
            } else {
                at += 1;  // skip ,
                skip_whitespace(string, at);
            }
            result.emplace_back(parse_value(string, at));
            skip_whitespace(string, at);
        } else {
            assert(false);
        }
    }

    return result;
}

jobject parse_object(const std::string& string, size_t& at) {
    at += 1;  // skip {
    skip_whitespace(string, at);

    jobject result;
    bool first = true;
    for (;;) {
        if (string.at(at) == '}') {
            at += 1;
            break;
        } else if (string.at(at) == ',' || first) {
            if (first) {
                first = false;
            } else {
                at += 1;  // skip ,
                skip_whitespace(string, at);
            }
            std::string key = parse_string(string, at);
            skip_whitespace(string, at);
            at += 1;  // skip :
            skip_whitespace(string, at);
            result[key] = parse_value(string, at);
            skip_whitespace(string, at);
        }
    }

    return result;
}

jvalue parse_value(const std::string& string, size_t& at) {
    skip_whitespace(string, at);
    char c = string.at(at);
    if (c == '"') return parse_string(string, at);
    else if (c == '[') return parse_array(string, at);
    else if (c == '{') return parse_object(string, at);
    else if (('0' <= c && c <= '9') || c == '+' || c == '-') return parse_number(string, at);
    else assert(false);
}

int sum(const jvalue& root, bool without_red) {
    const jnumber *number;
    const jarray *array;
    const jobject *object;
    if ((number = std::get_if<jnumber>(&root))) {
        return *number;
    } else if ((array = std::get_if<jarray>(&root))) {
        int result = 0;
        for (const jvalue& item : *array) {
            result += sum(item, without_red);
        }
        return result;
    } else if ((object = std::get_if<jobject>(&root))) {
        int result = 0;
        for (const auto& entry : *object) {
            const jstring* string;
            if (without_red && (string = std::get_if<jstring>(&entry.second)) && *string == "red") {
                return 0;
            }

            result += sum(entry.second, without_red);
        }
        return result;
    } else {
        return 0;
    }
}

int main() {
    std::string json;
    char c;
    while (std::cin.get(c)) {
        json += c;
    }

    size_t at = 0;
    jvalue root = parse_value(json, at);
    std::cout << sum(root, false) << std::endl;
    std::cout << sum(root, true) << std::endl;


    return 0;
}

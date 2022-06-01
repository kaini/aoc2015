#include <iostream>
#include <unordered_map>
#include <variant>

enum class type {
    constant,
    land,
    lor,
    lshift,
    rshift,
    lnot,
};

struct value {
    type type;
    std::variant<int, std::string> lhs;
    std::variant<int, std::string> rhs;

    bool evaluate(const std::unordered_map<std::string, value>& system) {
        if (type == type::constant && std::holds_alternative<int>(lhs)) {
            return false;
        }
        if (std::holds_alternative<std::string>(lhs) && system.at(std::get<std::string>(lhs)).type != type::constant) {
            return false;
        }
        if (std::holds_alternative<std::string>(rhs) && system.at(std::get<std::string>(rhs)).type != type::constant) {
            return false;
        }

        int lhs = std::holds_alternative<std::string>(this->lhs) ?
            std::get<int>(system.at(std::get<std::string>(this->lhs)).lhs) :
            std::get<int>(this->lhs);
        int rhs = std::holds_alternative<std::string>(this->rhs) ?
            std::get<int>(system.at(std::get<std::string>(this->rhs)).lhs) :
            std::get<int>(this->rhs);
        switch (type) {
            case type::constant: this->lhs = lhs; break;
            case type::land: this->lhs = lhs & rhs; break;
            case type::lor: this->lhs = lhs | rhs; break;
            case type::lnot: this->lhs = ~lhs; break;
            case type::rshift: this->lhs = lhs >> rhs; break;
            case type::lshift: this->lhs = (lhs << rhs) & 0xFFFF; break;
            default: assert(false); break;
        }
        type = type::constant;
        this->rhs = 0;
        return true;
    }
};

std::variant<int, std::string> parse_argument(const std::string str) {
    try {
        return std::stoi(str);
    } catch (const std::invalid_argument&) {
        return str;
    }
}

int evaluate(std::unordered_map<std::string, value> wires) {
    bool did_evaluate;
    do {
        did_evaluate = false;
        for (auto& wire : wires) {
            did_evaluate |= wire.second.evaluate(wires);
        }
    } while (did_evaluate);
    return std::get<int>(wires.at("a").lhs);
}

int main() {
    std::unordered_map<std::string, value> wires;
    for (;;) {
        value value;

        std::string lhs_str, op_str;
        std::cin >> lhs_str >> op_str;
        if (std::cin.eof()) {
            break;
        }
        if (op_str == "->") {
            value.type = type::constant;
            value.lhs = parse_argument(lhs_str);
        } else if (lhs_str == "NOT") {
            std::string arrow;
            std::cin >> arrow;
            value.type = type::lnot;
            value.lhs = parse_argument(op_str);
        } else {
            std::string rhs_str, arrow;
            std::cin >> rhs_str >> arrow;
            if (op_str == "AND") value.type = type::land;
            else if (op_str == "OR") value.type = type::lor;
            else if (op_str == "LSHIFT") value.type = type::lshift;
            else if (op_str == "RSHIFT") value.type = type::rshift;
            else { std::cerr << op_str << std::endl; assert(false); }
            value.lhs = parse_argument(lhs_str);
            value.rhs = parse_argument(rhs_str);
        }

        std::string dest;
        std::cin >> dest;
        wires[dest] = value;
    }

    int result = evaluate(wires);
    std::cout << result << std::endl;

    wires["b"] = {type::constant, result, 0};
    std::cout << evaluate(wires) << std::endl;

    return 0;
}
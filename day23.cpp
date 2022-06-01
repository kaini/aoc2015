#include <iostream>
#include <vector>

enum class op {
    hlf, tpl, inc, jmp, jie, jio
};

struct memory {
    int a = 0;
    int b = 0;
};

struct instr {
    op op;
    int memory::*r;
    int offset;
};

static void parse_regsiter(instr& instr) {
    std::string r;
    std::cin >> r;
    if (*r.rbegin() == ',') {
        r.erase(r.end() - 1);
    }
    if (r == "a") {
        instr.r = &memory::a;
    } else if (r == "b") {
        instr.r = &memory::b;
    } else {
        assert(false);
    }
}

static void run(const std::vector<instr>& program, memory& memory) {
    size_t pc = 0;
    while (pc < program.size()) {
        const auto& instr = program.at(pc);
        switch (instr.op) {
            case op::hlf:
                memory.*instr.r /= 2;
                pc += 1;
                break;
            case op::tpl:
                memory.*instr.r *= 3;
                pc += 1;
                break;
            case op::inc:
                memory.*instr.r += 1;
                pc += 1;
                break;
            case op::jmp:
                pc += instr.offset;
                break;
            case op::jie:
                if (memory.*instr.r % 2 == 0) {
                    pc += instr.offset;
                } else {
                    pc += 1;
                }
                break;
            case op::jio:
                if (memory.*instr.r == 1) {
                    pc += instr.offset;
                } else {
                    pc += 1;
                }
                break;
        }
    }
}

int main() {
    std::vector<instr> program;
    for (;;) {
        std::string op_str;
        std::cin >> op_str;
        if (std::cin.eof()) {
            break;
        }
        instr instr;
        if (op_str == "hlf") {
            instr.op = op::hlf;
            parse_regsiter(instr);
        } else if (op_str == "tpl") {
            instr.op = op::tpl;
            parse_regsiter(instr);
        } else if (op_str == "inc") {
            instr.op = op::inc;
            parse_regsiter(instr);
        } else if (op_str == "jmp") {
            instr.op = op::jmp;
            std::cin >> instr.offset;
        } else if (op_str == "jie") {
            instr.op = op::jie;
            parse_regsiter(instr);
            std::cin >> instr.offset;
        } else if (op_str == "jio") {
            instr.op = op::jio;
            parse_regsiter(instr);
            std::cin >> instr.offset;
        } else {
            assert(false);
        }
        program.push_back(instr);
    }

    memory memory;
    run(program, memory);
    std::cout << memory.b << std::endl;

    memory = {1, 0};
    run(program, memory);
    std::cout << memory.b << std::endl;

    return 0;
}

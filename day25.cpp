#include <iostream>

static const constexpr long long seed = 20151125ll;
static const constexpr long long factor = 252533ll;
static const constexpr long long module = 33554393ll;

int main() {
    // Zero based!
    int col, row;
    std::string dummy;
    std::cin >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy
            >> dummy >> dummy >> dummy >> dummy >> dummy >> row >> dummy >> dummy >> col;
    row -= 1;
    col -= 1;

    // The index of the first column of the target row increases by the row number for each row.
    // Therefore the number of the destination row's first column can be found by the Gaussian Sum Formula.
    // I use row + col as the destionation row, such that I can then follow the diagonal with + col.
    // The result is off by one compared to the problem description, i.e., it is zero-based.
    int index = (row + col) * ((row + col) + 1) / 2 + col;

    // Now I only need to calculate (seed * factor ^ index) % module.
    // This can be done by applying fast-exp to factor ^ index modulo module.
    // And finally multiplying it with seed module module.
    long long result = 1;
    long long power = index;
    for (long long mask = 1ll << (std::numeric_limits<long long>::digits - 1); mask; mask >>= 1) {
        result = (result * result) % module;
        if (power & mask) {
            result = (result * factor) % module;
        }
    }
    result = (result * seed) % module;
    std::cout << result << std::endl;

    return 0;
}
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>

static std::vector<int> find_primes(int limit) {
    std::vector<bool> sieve(limit + 1, true);

    sieve[0] = false;
    sieve[1] = false;
    for (size_t i = 0; i < sieve.size(); ++i) {
        if (sieve[i]) {
            for (size_t j = i + i; j < sieve.size(); j += i) {
                sieve[j] = false;
            }
        }
    }

    std::vector<int> result;
    for (size_t i = 0; i < sieve.size(); ++i) {
        if (sieve[i]) {
            result.push_back(static_cast<int>(i));
        }
    }
    return result;
}

struct factor {
    int factor;
    int count;
};

static std::vector<factor> factorize(const std::vector<int>& primes, int n) {
    assert(n > 0);
    std::vector<factor> result;
    for (int i = 0; n != 1; ++i) {
        while (n % primes[i] == 0) {
            n /= primes[i];
            if (result.empty() || result.rbegin()->factor != primes[i]) {
                result.push_back({primes[i], 1});
            } else {
                result.rbegin()->count += 1;
            }
        }
    }
    return result;
}

static void find_divisors(const std::vector<factor>& factors, std::vector<int>& divisors, size_t at = 0, int product = 1) {
    if (at < factors.size()) {
        find_divisors(factors, divisors, at + 1, product);
        for (int i = 1; i <= factors[at].count; ++i) {
            product *= factors[at].factor;
            find_divisors(factors, divisors, at + 1, product);
        }
    } else {
        divisors.push_back(product);
    }
}

int main() {
    int goal = 0;
    std::cin >> goal;
    assert(goal > 0);

    auto primes = find_primes(goal);
    auto primes_set = std::unordered_set(primes.begin(), primes.end());
    std::cerr << "There are " << primes.size() << " prime numbers up to " << goal << std::endl;

    std::unordered_map<int, int> divisor_sums;
    for (int house = 2;; ++house) {
        int divisor_sum;
        if (primes_set.find(house) != primes_set.end()) {
            // If the house is a prime number, then the sum of divisors is 1 + n.
            divisor_sum = house + 1;
        } else {
            // Otherwise find the first prime divisor and mulitply the divisor functions.
            // It holds that d(ab) = d(a) * d(b) if gcd(a, b) = 1
            // https://en.wikipedia.org/wiki/Divisor_function
            for (int p : primes) {
                if (house % p == 0) {
                    int a = house, b = 1, power_sum = 0;
                    while (a % p == 0) {
                        a /= p;
                        power_sum += b;
                        b *= p;
                    }
                    power_sum += b;
                    if (a == 1) {
                        // Sum from p^0 + p^1 ... + p^n. This has to be used if house = p^n.
                        divisor_sum = power_sum;
                    } else {
                        // Otherwise multiply the divisor sums of the two coprime factors.
                        divisor_sum = divisor_sums.at(a) * divisor_sums.at(b);
                    }
                    break;
                }
            }
        }
        divisor_sums[house] = divisor_sum;
        
        if (divisor_sum * 10 >= goal) {
            std::cout << house << std::endl;
            break;
        }
    }

    std::vector<int> divisors;
    for (int house = 2;; ++house) {
        int presents = 0;

        divisors.clear();
        find_divisors(factorize(primes, house), divisors);
        for (int divisor : divisors) {
            if (divisor * 50 >= house) {
                presents += divisor * 11;
            }
        }

        if (presents >= goal) {
            std::cout << house << std::endl;
            break;
        }
    }

    return 0;
}
#include <iostream>
#include <CommonCrypto/CommonDigest.h>

int main() {
    std::string secret;
    std::cin >> secret;

    bool found5 = false;
    for (int i = 1;; ++i) {
        std::string input = secret + std::to_string(i);
        unsigned char digest[16];
        CC_MD5(input.c_str(), input.size(), digest);
        if (!found5 && digest[0] == 0 && digest[1] == 0 && (digest[2] & 0xF0) == 0) {
            std::cout << i << std::endl;
            found5 = true;
        }
        if (digest[0] == 0 && digest[1] == 0 && digest[2] == 0) {
            std::cout << i << std::endl;
            break;
        }
    }

    return 0;
}
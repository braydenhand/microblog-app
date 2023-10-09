#include <iostream>
#include <string>
#include <openssl/hmac.h>
#include <bitset>

std::string Str_to_Bin(std::string s);
int SHA256_DIGEST_LENGTH = 32;
// Function to compute HMAC-SHA256
std::string computeHMAC(const std::string& key, const std::string& message) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    HMAC_CTX* ctx = HMAC_CTX_new();

    HMAC_Init_ex(ctx, key.c_str(), key.size(), EVP_sha256(), NULL);
    HMAC_Update(ctx, reinterpret_cast<const unsigned char*>(message.c_str()), message.size());
    HMAC_Final(ctx, digest, NULL);

    HMAC_CTX_free(ctx);

    // Truncate to 64 bits (8 bytes)
    std::string truncatedResult(reinterpret_cast<char*>(digest), 8);

    return truncatedResult;
}

int main() {
    // 64-bit binary input message (example)
    std::string binaryMessage = Str_to_Bin("BRAYDENK");
    
    // Secret key (example)
    std::string secretKey = "PASSWORD";

    // Calculate HMAC-SHA256
    std::string hmacValue = computeHMAC(secretKey, binaryMessage);
    for (int i=0; i<15;i++){
        hmacValue = computeHMAC(secretKey,hmacValue);
    }
    std::cout << "HMAC-SHA256: " << hmacValue << std::endl;

    std::cout << "\n" << std::endl;
    std::string value = "";
    value = Str_to_Bin(hmacValue);
    std::cout << "HMAC-Bin: " << value << std::endl;
    return 0;
}
std::string Str_to_Bin(std::string input) {
    std::string binaryString = "";

    for (char c : input) {
        // Convert each character to its ASCII value and then to a binary string.
        std::bitset<8> binaryChar(static_cast<unsigned char>(c));
        binaryString += binaryChar.to_string();
    }

    return binaryString;
}
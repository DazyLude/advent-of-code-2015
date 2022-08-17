#include <iomanip>
#include <string>
#include <array>
#include <iostream>
#include <openssl/evp.h>

bool check_md5_incomplete_five(const unsigned char* md_value) {
    for (int i = 0; i < 2; i++) {
        if ((int)md_value[i] != 0) { return true; }
    }
    if ((int)md_value[2] < 16) { return false; }
    return true;
}

bool check_md5_incomplete_six(const unsigned char* md_value) {
    for (int i = 0; i < 3; i++) {
        if ((int)md_value[i] != 0) { return true; }
    }
    return false;
}

int main(int argc, char* argv[])
{
    EVP_MD_CTX* mdctx;
    const EVP_MD* md;
    std::string code { "iwrupvqb" };
    
    std::array<unsigned char, 16> md_value;
    std::cout << std::hex;
    unsigned int md_len, i;
    md = EVP_md5();

    long long int answer { 0 };
    do {
        std::string advent_coin = code;
        advent_coin += std::to_string(answer);
        mdctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex2(mdctx, md, NULL);
        EVP_DigestUpdate(mdctx, advent_coin.data(), advent_coin.size());
        EVP_DigestFinal_ex(mdctx, md_value.data(), &md_len);
        EVP_MD_CTX_free(mdctx);
        ++answer;
    } while (check_md5_incomplete_six(md_value.data()));
    std::cout << "hash is: ";
    for (int i = 0; i < md_len; i++) {
        printf("%02x", md_value[i]);
    }
    std::cout << std::endl << std::dec << "answer is: " << --answer;
}
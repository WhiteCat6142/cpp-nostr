#ifndef NOSTR_UTIL_HPP_
#define NOSTR_UTIL_HPP_

#include <string>
#include <vector>
#include <random>

#include <openssl/sha.h>

namespace cpp_nostr
{

std::string bytes2hex(const unsigned char *data, const size_t len)
{
    const char hexchars[] = "0123456789abcdef";
    std::string s;
    s.reserve((len << 1) + 1);

    size_t j = 0;
    for (size_t i = 0; i < len; i++)
    {
        unsigned char b = data[i];

        s.push_back(hexchars[b >> 4]);
        s.push_back(hexchars[b & 0xF]);
    }

    return s;
}

std::vector<uint8_t> hex2bytes(const std::string &s)
{
    std::vector<uint8_t> v;
    v.reserve(s.size());
    for (auto it = s.cbegin(); it != s.cend(); ++it)
    {
        char b = *it;
        unsigned char c = (b < 'a') ? b - '0' : b - 'a' + 10;
        c <<= 4;
        ++it;
        b = *it;
        c += (b < 'a') ? b - '0' : b - 'a' + 10;
        v.push_back(c);
    }
    return v;
}

std::string sha256(const char *message, const size_t len)
{

    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha_ctx;
    SHA256_Init(&sha_ctx);                 // コンテキストを初期化
    SHA256_Update(&sha_ctx, message, len); // 注意！！　こちらに変更する必要がある
    SHA256_Final(digest, &sha_ctx);        // digest に出力

    return bytes2hex(digest, SHA256_DIGEST_LENGTH);
}

int fill_random(unsigned char* data, size_t size) {
    std::random_device rng;
    for(auto it=data;it<data+size;++it)
        *it=rng();
    return 0;
}

}

#endif
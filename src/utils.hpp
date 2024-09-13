#ifndef NOSTR_UTIL_HPP_
#define NOSTR_UTIL_HPP_

#include <string>
#include <vector>
#include <random>

#include <openssl/sha.h>

namespace cpp_nostr
{

std::string bytes2hex(const uint8_t *data, const size_t len)
{
    const char hexchars[] = "0123456789abcdef";
    std::string s;
    s.reserve((len << 1) + 1);

    for (size_t i = 0; i < len; i++)
    {
        uint8_t b = data[i];

        s.push_back(hexchars[b >> 4]);
        s.push_back(hexchars[b & 0xF]);
    }

    return s;
}

std::vector<uint8_t> hex2bytes(const std::string &s)
{
    std::vector<uint8_t> v;
    v.reserve(s.size()>>1);
    for (auto it = s.cbegin(); it != s.cend(); ++it)
    {
        char b = *it;
        uint8_t c = (b < 'a') ? b - '0' : b - 'a' + 10;
        c <<= 4;
        ++it;
        b = *it;
        c += (b < 'a') ? b - '0' : b - 'a' + 10;
        v.push_back(c);
    }
    return v;
}

std::vector<uint8_t> sha256(const char *message, const size_t len)
{
    std::vector<uint8_t> digest(SHA256_DIGEST_LENGTH);
    SHA256_CTX sha_ctx;
    SHA256_Init(&sha_ctx);
    SHA256_Update(&sha_ctx, message, len);
    SHA256_Final(digest.data(), &sha_ctx);

    return digest;
}

int fill_random(uint8_t* data, size_t size) {
    std::random_device rng;
    if((size&7)==0)
    {
    uint64_t *data2 = (uint64_t*)data;
    for(auto it=data2;it<data2+(size>>3);++it)
        *it=rng();
    }
    else{
    for(auto it=data;it<data+size;++it)
        *it=rng();
    }
    return 1;
}

}

#endif
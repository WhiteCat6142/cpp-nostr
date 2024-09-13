#ifndef CPP_NOSTR_NIP19_HPP_
#define CPP_NOSTR_NIP19_HPP_

#include "./utils.hpp"
#include <string>
#include <vector>
#include <optional>

#include <libbech32/bech32.h>

namespace cpp_nostr
{
    class NIP19 final
    {
    private:
        template <int from, int to, typename Iterator, typename Fn>
        static void convert_bits(Iterator at, Iterator end, Fn fn)
        {
            constexpr unsigned int input_mask = ~((~0U) << from);
            constexpr unsigned int output_mask = ~((~0U) << to);
            unsigned int accum = 0;
            int sz = 0;
            while (at != end)
            {
                unsigned int val = (*at) & input_mask;
                sz += from;
                accum = (accum << from) | val;
                while (sz >= to)
                {
                    unsigned int b = (accum >> (sz - to)) & output_mask;
                    fn(b);
                    sz -= to;
                }
                ++at;
            }
            if constexpr (to < from)
            {
                if (sz)
                {
                    accum <<= (to - sz);
                    unsigned int b = accum & output_mask;
                    fn(b);
                }
            }
        }

    public:
        static std::string encode(const std::string &hrp, const std::vector<uint8_t> &code)
        {
            std::vector<uint8_t> pk;
            pk.reserve(32);
            convert_bits<8, 5>(code.begin(), code.end(),
                               [&](unsigned char c) mutable
                               {
                                   pk.push_back(c);
                               });
            return bech32::encodeUsingOriginalConstant(hrp, pk);
        }

        static std::optional<std::string> decode(const std::string &s, std::vector<uint8_t> &out)
        {
            bech32::DecodedResult decodedResult = bech32::decode(s);
            out.clear();
            out.reserve((decodedResult.dp.size()<<3)/5+1);
            // 5bits -> 8bits
            convert_bits<5, 8>(decodedResult.dp.begin(), decodedResult.dp.end(),
                               [&](unsigned char c) mutable
                               { out.push_back(c); });
            if (decodedResult.encoding != bech32::Encoding::Bech32)
                return std::nullopt;
            else
                return decodedResult.hrp;
        }
    };
}

#endif
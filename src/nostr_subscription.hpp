#ifndef CPP_NOSTR_SUBCRIPTION_HPP
#define CPP_NOSTR_SUBCRIPTION_HPP

#include "nostr_event.hpp"
#include <string>

namespace cpp_nostr
{
    class NostrSubscription
    {
    private:
    public:
    uint32_t limit = 0;
        NostrSubscription()
        {
        }
        ~NostrSubscription() = default;
        std::string encode() const
        {
            return R"({"limit": 10})";
        }
    };
}

#endif
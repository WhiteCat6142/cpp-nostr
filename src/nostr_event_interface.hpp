#ifndef CPP_NOSTR_EVENT_INTERFACE_HPP
#define CPP_NOSTR_EVENT_INTERFACE_HPP

#include "nostr_event.hpp"
#include <string>
#include <vector>
#include <optional>

namespace cpp_nostr
{
    class NostrEventInterface
    {
    public:
        static std::optional<std::vector<uint8_t>> generateKey()
        {
            return std::vector<uint8_t>{};
        }
        static bool finalize_event(NostrEvent &ev, const std::vector<uint8_t> &sk)
        {
            return false;
        }
        static std::string encode(const NostrEvent &ev)
        {
            return "";
        }
        static bool verify_event(const NostrEvent &ev)
        {
            return false;
        }
        static NostrEvent decode(const std::string &s)
        {
            return NostrEvent();
        }
        static std::vector<uint8_t> get_publickey(const std::vector<uint8_t> &code)
        {
            return std::vector<uint8_t>{};
        }
    };
}

#endif

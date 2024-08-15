#ifndef CPP_NOSTR_RELAY_UTILS_HPP
#define CPP_NOSTR_RELAY_UTILS_HPP

#include "nostr_event.hpp"
#include "nostr_subscription.hpp"
#include "nostr_relay.hpp"
#include <random>
#include <fmt/core.h>

namespace cpp_nostr
{
    class NostrRelayUtils final
    {
    public:
        static std::string makeUnsubscribeCommand(const NostrEventSubId sub_id)
        {
            return fmt::format(R"(["EOSE","{}"])", sub_id);
        }
        static std::string makeSubscribeCommand(const NostrEventSubId sub_id, const NostrSubscription &sub)
        {
            return fmt::format(R"(["REQ","{}","{}"])", sub_id, sub.encode());
        }
        static std::string makePublishCommand(const std::string &ev)
        {
            return fmt::format(R"(["EVENT",{}])",ev);
        }
        static NostrEventSubId makeUniqueSubId()
        {
            std::random_device rng;
            return rng();
        }
    };
}

#endif
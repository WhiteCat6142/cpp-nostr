#ifndef CPP_NOSTR_RELAY_HPP
#define CPP_NOSTR_RELAY_HPP

#include "nostr_event.hpp"
#include "nostr_subscription.hpp"
#include "nostr_relay_interface.hpp"
#include <string>
#include <vector>
#include <functional>
#include <future>

namespace cpp_nostr
{
    using NostrEventCallback = std::function<void(NostrEvent &)>;
    using NostrEventSubId = uint32_t;
    using NostrEventKinds = std::vector<int>;

    class NostrRelay
    {
    public:
        virtual NostrEventSubId subscribe(NostrEventCallback callback, const NostrSubscription &sub) = 0;
        virtual bool unsubscribe(const NostrEventSubId id) = 0;
        virtual std::future<bool> publish(const std::string &ev) = 0;
    };
}

#endif
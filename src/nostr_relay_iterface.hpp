#ifndef CPP_NOSTR_RELAY_INTERFACE_HPP
#define CPP_NOSTR_RELAY_INTERFACE_HPP

#include "nostr_event.hpp"
#include <string>
#include <functional>

namespace cpp_nostr
{
    using NostrEventCallback = std::function<void(const NostrEvent&)>; 
    class NostrRelayInterface
    {
    protected:
        NostrRelayInterface()
        {
        }

    public:
        virtual ~NostrRelayInterface() = default;
        virtual NostrEventSubId subscribe(const NostrEventCallback callback, const NostrEventKinds& kinds, const uint32_t limit) = 0;
        virtual bool unsubscribe(const NostrEventSubId id) = 0;
        virtual bool publish(const std::string &ev) = 0;
    };
}

#endif
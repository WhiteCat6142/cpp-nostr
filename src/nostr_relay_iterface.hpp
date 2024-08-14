#ifndef CPP_NOSTR_RELAY_INTERFACE_HPP
#define CPP_NOSTR_RELAY_INTERFACE_HPP

#include "nostr_event.hpp"
#include <string>
#include <functional>

namespace cpp_nostr
{
    using Func = std::function<void(const NostrEvent&)>; 
    class NostrRelayInterface
    {
    protected:
        NostrRelayInterface()
        {
        }

    public:
        virtual ~NostrRelayInterface()
        {
        }
        virtual bool subscribe(const Func callback, const NostrEventKinds& kinds, const uint32_t limit) = 0;
        virtual bool publish(const NostrEvent &ev) = 0;
    };
}

#endif
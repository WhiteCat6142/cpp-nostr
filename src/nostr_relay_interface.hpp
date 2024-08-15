#ifndef CPP_NOSTR_RELAY_INTERFACE_HPP
#define CPP_NOSTR_RELAY_INTERFACE_HPP

#include "nostr_event.hpp"
#include <string>
#include <functional>

namespace cpp_nostr
{
    class NostrRelayInterface
    {
    protected:
        std::function<void(const std::string&)> callback;

    public:
        NostrRelayInterface()
        {
        }
        virtual ~NostrRelayInterface() = default;
        virtual bool send(const std::string &cmd) = 0;
        void set_callback(std::function<void(const std::string&)> callback_)
        {
            callback=callback_;
        }
    };
}

#endif
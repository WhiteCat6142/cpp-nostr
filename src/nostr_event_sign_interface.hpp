#ifndef CPP_NOSTR_EVENT_SIGN_INTERFACE_HPP
#define CPP_NOSTR_EVENT_SIGN_INTERFACE_HPP

#include "nostr_event.hpp"
#include <string>

namespace cpp_nostr
{
class NostrEventSignInterface
{
   public:
    virtual ~NostrEventSignInterface()
    {
    }
    virtual bool sign_event(const unsigned char *sk, NostrEvent &ev) const = 0;
    virtual const std::string encode(const NostrEvent &ev) const = 0;
};
}

#endif

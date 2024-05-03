#ifndef NOSTR_EVENT_SIGN_INTERFACE_HPP
#define NOSTR_EVENT_SIGN_INTERFACE_HPP

#include "nostr_event.hpp"
#include <string>

namespace rx_nostr
{
class NostrEventSignInterface
{
   public:
    virtual ~NostrEventSignInterface()
    {
    }
    virtual bool sign_event(const unsigned char *sk, NostrEvent &ev) const = 0;
};
}  // namespace rx_nostr

#endif  // NOSTR_EVENT_SIGN_INTERFACE_HPP

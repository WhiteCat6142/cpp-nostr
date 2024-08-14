#ifndef CPP_NOSTR_EVENT_INTERFACE_HPP
#define CPP_NOSTR_EVENT_INTERFACE_HPP

#include "nostr_event.hpp"
#include <string>

namespace cpp_nostr
{
    class NostrEventInterface
    {
    protected:
        NostrEvent* ev;

    public:
        NostrEventInterface(NostrEvent *ev_) : ev(ev_)
        {
        }
        virtual ~NostrEventInterface()
        {
        }
        virtual bool finalize_event(const std::vector<uint8_t> &sk) = 0;
        virtual const std::string encode() const = 0;
        static bool verify_event(const NostrEvent &ev)
        {
            return false;
        }
        static NostrEvent decode(const std::string &s)
        {
            return NostrEvent();
        }
    };
}

#endif

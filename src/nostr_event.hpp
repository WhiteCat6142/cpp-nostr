#ifndef CPP_NOSTR_EVENT_HPP
#define CPP_NOSTR_EVENT_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace cpp_nostr
{
using NostrEventId        = std::string;
using NostrEventPubkey    = std::string;
using NostrEventKind      = int;
using NostrEventTags      = std::vector<std::vector<std::string>>;
using NostrEventSignature = std::string;
using NostrEventContent   = std::string;
using NostrEventCreatedAt = uint64_t;

class NostrEvent final
{
   public:
    NostrEventId        id;
    NostrEventPubkey    pubkey;
    NostrEventCreatedAt created_at;
    NostrEventKind      kind;
    NostrEventTags      tags;
    NostrEventContent   content;
    NostrEventSignature sig;
};
}

#endif
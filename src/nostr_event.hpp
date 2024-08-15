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
using NostrEventKinds     = std::vector<int>;
using NostrEventTagItem   = std::string;
using NostrEventTags      = std::vector<std::vector<std::string>>;
using NostrEventSignature = std::string;
using NostrEventContent   = std::string;
using NostrEventSubId     = int;
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
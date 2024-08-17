#ifndef CPP_NOSTR_SUBCRIPTION_HPP
#define CPP_NOSTR_SUBCRIPTION_HPP

#include "nostr_event.hpp"
#include <string>
#include <vector>
#include <optional>
#include <map>
#include <fmt/core.h>
#include <fmt/ranges.h>

namespace cpp_nostr
{
    class NostrSubscription
    {
    private:
    public:
        std::optional<std::vector<std::string>> ids = std::nullopt;
        std::optional<std::vector<std::string>> authors  = std::nullopt;
        std::optional<std::vector<int>> kinds = std::nullopt;
        std::optional<std::map<std::string, std::vector<std::string>>> tags = std::nullopt;
        std::optional<uint64_t> since = std::nullopt;
        std::optional<uint64_t> until = std::nullopt;
        std::optional<uint32_t> limit = std::nullopt;
        NostrSubscription *prev = nullptr;

        std::string encode() const
        {
            std::string s = (prev != nullptr) ? prev->encode() : "";
            if (prev != nullptr)
                s.append(", ");
            s.append("{");
            bool first = true;
            if (ids)
            {
                if (!first)
                    s.append(", ");
                s.append(fmt::format(R"("ids": {})", *ids));
                first = false;
            }
            if (authors)
            {
                if (!first)
                    s.append(", ");
                s.append(fmt::format(R"("authors": {})", *authors));
                first = false;
            }
            if (kinds)
            {
                if (!first)
                    s.append(", ");
                s.append(fmt::format(R"("kinds": [{}])", fmt::join(*kinds, ", ")));
                first = false;
            }
            if (tags)
            {
                if (!first)
                    s.append(", ");
                for(const auto p : *tags)
                {
                    s.append(fmt::format(R"("{}": {})", p.first, p.second));
                }
                first = false;
            }
            if (since)
            {
                if (!first)
                    s.append(", ");
                s.append(fmt::format(R"("since": {})", *since));
                first = false;
            }
            if (until)
            {
                if (!first)
                    s.append(", ");
                s.append(fmt::format(R"("until": {})", *until));
                first = false;
            }
            if (limit)
            {
                if (!first)
                    s.append(", ");
                s.append(fmt::format(R"("limit": {})", *limit));
                first = false;
            }
            s.append("}");
            return s;
        }
    };
}

#endif
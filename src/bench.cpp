#include <benchmark/benchmark.h>
#include <vector>
#include <optional>
#include <map>

#include <nostr_event_yyjson.hpp>
#include <utils.hpp>
#include <nip19.hpp>

using namespace cpp_nostr;

static void BM_FtVector(benchmark::State &state)
{
    std::vector<uint8_t> sk = *NostrEventYYJSON::generateKey();
    std::vector<std::vector<std::string>> vec{};
    NostrEvent ev{
        .created_at = 0,
        .kind = 1,
        .tags = vec,
        .content = ""};
    NostrEventYYJSON::finalize_event(ev, sk);

    for (auto _ : state)
    {
        for (int i = 0; i < state.range(0); ++i)
        {
            NostrEventYYJSON::verify_event(ev);
        }
    }
}
BENCHMARK(BM_FtVector)->Range(1, 1 << 8);

BENCHMARK_MAIN();
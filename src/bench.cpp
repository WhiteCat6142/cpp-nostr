#include <benchmark/benchmark.h>
#include <vector>
#include <optional>
#include <map>

#include <nostr_event_yyjson.hpp>
#include <utils.hpp>
#include <nip19.hpp>

using namespace cpp_nostr;

static void BM_Verify(benchmark::State &state)
{
    const std::vector<uint8_t> sk = *NostrEventYYJSON::generateKey();
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

static void BM_GenKey(benchmark::State &state)
{
    for (auto _ : state)
    {
        for (int i = 0; i < state.range(0); ++i)
        {
            NostrEventYYJSON::generateKey();
        }
    }
}

static void BM_Finalize(benchmark::State &state)
{
    const std::vector<uint8_t> sk = *NostrEventYYJSON::generateKey();
    const std::vector<std::vector<std::string>> vec{};
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
            NostrEvent e(ev);
            NostrEventYYJSON::finalize_event(e, sk);
        }
    }
}

static void BM_Decode(benchmark::State &state)
{
    const std::vector<uint8_t> sk = *NostrEventYYJSON::generateKey();
    const std::vector<std::vector<std::string>> vec{};
    NostrEvent ev{
        .created_at = 0,
        .kind = 1,
        .tags = vec,
        .content = ""};
    NostrEventYYJSON::finalize_event(ev, sk);
    const std::string str = NostrEventYYJSON::encode(ev);

    for (auto _ : state)
    {
        for (int i = 0; i < state.range(0); ++i)
        {
            NostrEventYYJSON::decode(str);
        }
    }
}

static void BM_Encode(benchmark::State &state)
{
    const std::vector<uint8_t> sk = *NostrEventYYJSON::generateKey();
    const std::vector<std::vector<std::string>> vec{};
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
            NostrEventYYJSON::encode(ev);
        }
    }
}

BENCHMARK(BM_Verify)->Range(1, 1 << 8);
BENCHMARK(BM_GenKey)->Range(1, 1 << 8);
BENCHMARK(BM_Finalize)->Range(1, 1 << 8);
BENCHMARK(BM_Decode)->Range(1, 1 << 8);
BENCHMARK(BM_Encode)->Range(1, 1 << 8);

BENCHMARK_MAIN();
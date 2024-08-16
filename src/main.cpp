// #include <rx_nostr.hpp>
// #include <logger_stdout.hpp>
#include <thread>
#include <iostream>

#include <nostr_event_yyjson.hpp>
#include <utils.hpp>
#include <nip19.hpp>
#include "nostr_subscription.hpp"
#include "nostr_relay_interface.hpp"
#include "nostr_relay.hpp"
#include "nostr_relay_simple.hpp"
#include "nostr_relay_libhv.hpp"

#include <ctime>

using namespace std::chrono_literals;
using namespace cpp_nostr;

static const int MAX_EVENTS = 300;
// static LoggerInterface *logger = nullptr;
static int count = 0;

void callback(const NostrEvent &event)
{
    if (count >= MAX_EVENTS)
    {
        return;
    }
    // logger->log(LogLevel::INFO, event.content);
}

time_t now()
{
    time_t n;
    std::time(&n);
    return n;
}

std::string sign(char *nsec)
{
    std::vector<uint8_t> sk;
    if (!NIP19::decode(std::string(nsec), sk))
        return "";
    std::vector<std::vector<std::string>> vec{};
    NostrEvent ev{
        .created_at = now(),
        .kind = 1,
        .tags = vec,
        .content = "test"};
    NostrEventYYJSON i(&ev);
    i.finalize_event(sk);
    return i.encode();
}

int main(int argc, char *argv[])
{

    std::vector<uint8_t> data = hex2bytes("7e7e9c42a91bfef19fa929e5fda1b72e0ebc1a4c1141673e2794234d86addf4e");
    auto npub = "npub10elfcs4fr0l0r8af98jlmgdh9c8tcxjvz9qkw038js35mp4dma8qzvjptg";
    std::vector<uint8_t> pk;
    std::cout << NIP19::decode(std::string(npub), pk).value() << std::endl;
    std::cout << (pk == data) << std::endl;

    char *message = "Sample Message";

    std::cout << sha256(message, strlen(message)) << std::endl;
/*
    if (argc > 1)
    {
        auto e = sign(argv[1]);
        auto e2 = NostrEventYYJSON::decode(e);
        std::cout << NostrEventYYJSON::verify_event(e2) << std::endl;
        NostrEventYYJSON i(&e2);
        auto y = i.encode();
        std::cout << e << std::endl;
        std::cout << y << std::endl;
    }*/

    auto knostr = R"({"id":"93dc70f965af436095ba1d60d5c66ee235fdf82222e78238022317d73a95565f","pubkey":"6a36c1a62cba047b1cdb93bef316c6617c79816e32b80166c471c30bdb77e526","created_at":1723649902,"kind":1,"tags":[],"content":"test","sig":"8d08e7ec3134288fee4db14842652771f765ecbd7676c442529c9ea9ff4861153919559a7757a77593525774a44ccf15fc2af982b6501ca4f34bf3454ca57f11"})";
    auto e3 = NostrEventYYJSON::decode(knostr);
    std::cout << NostrEventYYJSON::verify_event(e3) << std::endl;

std::cout << "t"<< std::endl;
    NostrRelayLibhv relay;
    NostrRelaySimple rx(&relay);
    relay.connect("wss://relay-jp.nostr.wirednet.jp/");
    NostrSubscription subx;
    rx.subscribe([](NostrEvent &ev){
        std::cout << "t"<< std::endl;
        NostrEventYYJSON i(&ev);
        std::cout << i.encode() << std::endl;
    },subx);
    std::this_thread::sleep_for(20000ms);
    /*

    logger = new LoggerStdout();
    RxNostr rx_nostr(logger);

    logger->log(LogLevel::INFO, "[main] subscribe");
    auto ret = rx_nostr.subscribe(
        callback,
        std::vector<NostrEventKind>{1},
        ,
        MAX_EVENTS);

    if (!ret)
    {
        goto FINALIZE;
    }

    logger->log(LogLevel::INFO, "[main] Wait 3 seconds");
    std::this_thread::sleep_for(3000ms);

    if (argc > 1)
    {
        rx_nostr.send(sign(argv[1]),"wss://relay-jp.nostr.wirednet.jp/");
    }

    logger->log(LogLevel::INFO, "[main] unsubscribe");
    rx_nostr.unsubscribe();

    logger->log(LogLevel::INFO, "[main] wait 3 seconds");
    std::this_thread::sleep_for(3000ms);

    logger->log(LogLevel::INFO, "[main] bye");


FINALIZE:

    if (logger != nullptr)
    {
        delete logger;
    }*/

    return 0;
}
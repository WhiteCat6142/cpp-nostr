// #include <rx_nostr.hpp>
// #include <logger_stdout.hpp>
#include <thread>
#include <iostream>

#include <nostr_event_yyjson.hpp>
#include <utils.hpp>
#include <nip19.hpp>

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

    if (argc > 1)
    {
        auto e = sign(argv[1]);
        auto e2 = NostrEventYYJSON::decode(e);
        NostrEventYYJSON i(&e2);
        auto y = i.encode();
        std::cout << e << std::endl;
        std::cout << y << std::endl;
    }
    /*

    logger = new LoggerStdout();
    RxNostr rx_nostr(logger);

    logger->log(LogLevel::INFO, "[main] subscribe");
    auto ret = rx_nostr.subscribe(
        callback,
        std::vector<NostrEventKind>{1},
        "wss://relay-jp.nostr.wirednet.jp/",
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
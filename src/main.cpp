//#include <rx_nostr.hpp>
//#include <logger_stdout.hpp>
#include <thread>
#include <iostream>

#include <nostr_event_sign_yyjson.hpp>
#include <utils.hpp>

#include <libbech32/bech32.h>

#include <ctime>

using namespace std::chrono_literals;
using namespace cpp_nostr;

static const int MAX_EVENTS = 300;
//static LoggerInterface *logger = nullptr;
static int count = 0;

void callback(const NostrEvent &event)
{
    if (count >= MAX_EVENTS)
    {
        return;
    }
    //logger->log(LogLevel::INFO, event.content);
}

template <int from, int to, typename Iterator, typename Fn>
void convert_bits(Iterator at, Iterator end, Fn fn)
{
    constexpr unsigned int input_mask = ~((~0U) << from);
    constexpr unsigned int output_mask = ~((~0U) << to);
    unsigned int accum = 0;
    int sz = 0;
    while (at != end)
    {
        unsigned int val = (*at) & input_mask;
        sz += from;
        accum = (accum << from) | val;
        while (sz >= to)
        {
            unsigned int b = (accum >> (sz - to)) & output_mask;
            fn(b);
            sz -= to;
        }
        ++at;
    }
    if constexpr (to < from)
    {
        if (sz)
        {
            accum <<= (to - sz);
            unsigned int b = accum & output_mask;
            fn(b);
        }
    }
}

time_t now()
{
    time_t n;
    std::time(&n);
    return n;
}

std::string sign(char *nsec)
{

    unsigned char sk[32];
    bech32::DecodedResult decoded = bech32::decode(nsec);
    convert_bits<5, 8>(decoded.dp.begin(), decoded.dp.end(),
                       [&, pos = 0U](unsigned char c) mutable
                       {
                           if (pos < 32)
                               sk[pos++] = c;
                       });
    NostrEvent ev;
    std::vector<std::vector<std::string>> vec{};
    ev.content = "test";
    ev.created_at = now();
    ev.kind = 1;
    ev.tags = vec;
    NostrEventSignYYJSON i;
    i.sign_event(sk, ev);
    return i.encode(ev);
}

int main(int argc, char *argv[])
{

    std::vector<uint8_t> data = hex2bytes("7e7e9c42a91bfef19fa929e5fda1b72e0ebc1a4c1141673e2794234d86addf4e");
    bech32::DecodedResult decodedResult = bech32::decode("npub10elfcs4fr0l0r8af98jlmgdh9c8tcxjvz9qkw038js35mp4dma8qzvjptg");
    uint8_t sk[32];
    // 5bits -> 8bits
    convert_bits<5, 8>(decodedResult.dp.begin(), decodedResult.dp.end(),
                       [&, pos = 0U](unsigned char c) mutable
                       {
                           if (pos < 32)
                               sk[pos++] = c;
                       });
    std::vector<uint8_t> re(std::begin(sk), std::end(sk));
    std::cout << decodedResult.hrp << std::endl;
    std::copy(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;
    std::copy(re.begin(), re.end(), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;
    std::cout << (data == re) << std::endl;
    std::cout << (decodedResult.encoding == bech32::Encoding::Bech32) << std::endl;

    uint8_t pk[52];
    convert_bits<8, 5>(re.begin(), re.end(),
                       [&, pos = 0U](unsigned char c) mutable
                       {
                           if (pos < 52)
                               pk[pos++] = c;
                       });
    std::vector<uint8_t> re2(std::begin(pk), std::end(pk));
    std::cout << (decodedResult.dp == re2) << std::endl;
    std::string str = bech32::encodeUsingOriginalConstant(decodedResult.hrp, re2);
    std::cout << str << std::endl;

    char *message = "Sample Message";

    std::cout << sha256(message, strlen(message)) << std::endl;

    if (argc > 1)
        std::cout << sign(argv[1]) << std::endl;
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
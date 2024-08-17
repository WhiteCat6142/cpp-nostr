#include <thread>
#include <iostream>
#include <optional>
#include <map>

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
static int count = 0;


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
    return NostrEventYYJSON::encode(ev);
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

    NostrRelayLibhv relay;
    NostrRelaySimple rx(&relay);
    relay.connect("wss://relay-jp.nostr.wirednet.jp/");
    NostrEventKinds list {1};
    NostrSubscription subx{
        .kinds = list,
        .since = (now()),
        .limit = 0
    };
    std::map<std::string, std::vector<std::string>> list2 {{"#p",{"6a36c1a62cba047b1cdb93bef316c6617c79816e32b80166c471c30bdb77e526"}}};
    NostrSubscription suby{
        .kinds = list,
        .tags = list2,
        .since = (now()-3600),
//        .prev = &subx
    };
    std::cout << suby.encode() << std::endl;
    rx.subscribe([](NostrEvent &ev)
                 {
        if(ev.kind!=1)
        return;
        std::cout << NostrEventYYJSON::encode(ev) << std::endl; }, subx);

    if (argc > 1)
    {
        auto e = sign(argv[1]);
        auto e2 = NostrEventYYJSON::decode(e);
        std::cout << NostrEventYYJSON::verify_event(e2) << std::endl;
        std::cout << e << std::endl;
        auto f = rx.publish(e2);
        auto r = f.wait_for(100ms);
        if (r==std::future_status::timeout)
        std::cout << "timeouted" << std::endl;
        else
        {
        bool result = f.get();
        std::cout << result << std::endl;
        }
    }

    auto knostr = R"({"id":"93dc70f965af436095ba1d60d5c66ee235fdf82222e78238022317d73a95565f","pubkey":"6a36c1a62cba047b1cdb93bef316c6617c79816e32b80166c471c30bdb77e526","created_at":1723649902,"kind":1,"tags":[],"content":"test","sig":"8d08e7ec3134288fee4db14842652771f765ecbd7676c442529c9ea9ff4861153919559a7757a77593525774a44ccf15fc2af982b6501ca4f34bf3454ca57f11"})";
    auto e3 = NostrEventYYJSON::decode(knostr);
    std::cout << NostrEventYYJSON::verify_event(e3) << std::endl;

    std::cout << "t" << std::endl;

    std::this_thread::sleep_for(20000ms);

    return 0;
}
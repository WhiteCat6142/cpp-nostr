#include <rx_nostr.hpp>
#include <logger_stdout.hpp>
#include <thread>

using namespace std::chrono_literals;
using namespace rx_nostr;

static const int        MAX_EVENTS = 300;
static LoggerInterface* logger     = nullptr;
static int              count      = 0;

void callback(const NostrEvent& event)
{
    if (count >= MAX_EVENTS) {
        return;
    }
    logger->log(LogLevel::INFO,event.content);
}

int main(void)
{
    logger = new LoggerStdout();
    RxNostr rx_nostr(logger);

    logger->log(LogLevel::INFO, "[main] subscribe");
    auto ret = rx_nostr.subscribe(
        callback,
        std::vector<NostrEventKind>{1},
        "wss://relay-jp.nostr.wirednet.jp/",
        MAX_EVENTS);

    if (!ret) {
        goto FINALIZE;
    }

    logger->log(LogLevel::INFO, "[main] Wait 3 seconds");
    std::this_thread::sleep_for(3000ms);

    logger->log(LogLevel::INFO, "[main] unsubscribe");
    rx_nostr.unsubscribe();

    logger->log(LogLevel::INFO, "[main] wait 3 seconds");
    std::this_thread::sleep_for(3000ms);

    logger->log(LogLevel::INFO, "[main] bye");

FINALIZE:

    if (logger != nullptr) {
        delete logger;
    }

    return 0;
}
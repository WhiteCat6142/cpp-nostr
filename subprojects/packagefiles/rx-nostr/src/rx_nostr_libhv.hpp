#ifndef RX_NOSTR_LIBHV_HPP
#define RX_NOSTR_LIBHV_HPP

#include <cstdio>
#include <thread>
#include "hv/WebSocketClient.h"
#include "implode.hpp"
#include "nostr_event.hpp"
#include "rx_nostr_interface.hpp"
#include "logger_interface.hpp"
#include "nostr_event_decode_interface.hpp"

namespace rx_nostr
{
class RxNostrLibhv final : public RxNostrInterface
{
   public:
    RxNostrLibhv(LoggerInterface* logger, NostrEventDecodeInterface* decoder)
    {
        this->logger       = logger;
        this->decoder      = decoder;
        this->sub_id       = nullptr;
        this->eose_cmd     = "";
        this->is_connected = false;
        this->setReconnectInterval(1000, 10000);
    }

    ~RxNostrLibhv()
    {
        if (this->is_connected) {
            this->unsubscribe();
        }
    }

    bool subscribe(
        const NostrEventCallback callback,
        const NostrEventKinds&   kinds,
        const std::string&       relay,
        const uint32_t           limit)
    {
        this->callback = callback;

        this->ws.onopen = [this]() {
            this->onOpen();
        };

        this->ws.onmessage = [this](const std::string& msg) {
            this->onMessage(msg);
        };

        this->ws.onclose = [this]() {
            this->onClose();
        };

        // TODO: check result
        auto result = this->ws.open(relay.c_str());

        // wait until connected
        while (!ws.isConnected()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // make unique sub_id
        this->sub_id   = this->makeUniqueSubId();
        this->eose_cmd = this->makeEOSECommand(this->sub_id);

        // send subscribe message
        auto cmd = this->makeSubscribeCommand(kinds, limit);
        ws.send(cmd);

        this->is_connected = true;
        return true;
    }

    bool unsubscribe()
    {
        if (!this->is_connected) {
            this->logger->log(LogLevel::WARNING, "cannot unsubscribe while not connected");
            return false;
        }

        //Send unsubscribe message
        auto cmd = this->makeUnsubscribeCommand(this->sub_id);
        ws.send(cmd);

        //TODO: check result
        auto result        = ws.close();
        this->is_connected = false;
        return true;
    }

    bool send(const std::string& event, const std::string& relay)
    {
        hv::WebSocketClient ws0;
        ws0.onopen = [this]() {
        };
        ws0.onmessage = [this](const std::string& msg) {
        };
        ws0.onclose = []() {
        };
        ws0.setReconnect(&this->reconn);
        ws0.open(relay.c_str());
        while (!ws0.isConnected()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        ws0.send("[\"EVENT\","+event+"]");
        ws0.close();
        return true;
    }

    bool setReconnectInterval(const uint32_t min_timeout, const uint32_t max_timeout)
    {
        if (this->is_connected) {
            this->logger->log(LogLevel::WARNING, "cannot set reconnect interval while connected");
            return false;
        }

        reconn_setting_init(&this->reconn);
        this->reconn.min_delay    = min_timeout;
        this->reconn.max_delay    = max_timeout;
        this->reconn.delay_policy = 2;
        return true;
    }

   private:
    LoggerInterface*           logger;
    NostrEventDecodeInterface* decoder;
    NostrEventSubId            sub_id;
    std::string                eose_cmd;
    hv::WebSocketClient        ws;
    reconn_setting_t           reconn;
    NostrEventCallback         callback;
    bool                       is_connected;

    NostrEventSubId makeUniqueSubId()
    {
        return "rx_nostr";
    }

    std::string makeUnsubscribeCommand(const char* sub_id)
    {
        char cmd[128];

        std::snprintf(
            cmd,
            sizeof(cmd),
            "[\"CLOSE\",\"%s\"]",
            sub_id);

        return std::string(cmd);
    }

    std::string makeEOSECommand(NostrEventSubId sub_id)
    {
        char cmd[128];

        std::snprintf(
            cmd,
            sizeof(cmd),
            "[\"EOSE\",\"%s\"]",
            sub_id);

        return std::string(cmd);
    }

    std::string makeSubscribeCommand(const NostrEventKinds& kinds, const uint32_t limit)
    {
        // Make kinds string [1, 2, 3] -> "1, 2, 3"
        std::string kinds_str = implode(kinds.begin(), kinds.end(), ",");

        char cmd[128];

        std::snprintf(
            cmd,
            sizeof(cmd),
            "[\"REQ\", \"%s\", {\"kinds\":[%s], \"limit\":%d}]",
            this->sub_id,
            kinds_str.c_str(),
            limit);

        this->logger->log(LogLevel::DEBUG, std::string("subscribe command: ") + cmd);
        return std::string(cmd);
    }

    void onOpen(void)
    {
        this->logger->log(LogLevel::INFO, "open");
    }

    void onMessage(const std::string& msg)
    {
        // dump raw message
        //this->logger->log(LogLevel::DEBUG, msg);

        if (msg.find(this->eose_cmd) != std::string::npos) {
            auto cmd = this->makeUnsubscribeCommand(this->sub_id);
            this->logger->log(LogLevel::DEBUG, "receive EOSE, send unsubscribe command.");
            this->logger->log(LogLevel::DEBUG, cmd);
            this->ws.send(cmd);
            return;
        }

        auto event = NostrEvent();
        if (!this->decoder->decode(msg, this->sub_id, event)) {
            return;
        }

        this->callback(event);
    }

    void onClose()
    {
        this->logger->log(LogLevel::INFO, "close");
    }
};
}  // namespace rx_nostr

#endif  // RX_NOSTR_LIBHV_HPP

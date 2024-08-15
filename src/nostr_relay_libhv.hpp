#ifndef CPP_NOSTR_LIBHV_HPP
#define CPP_NOSTR_LIBHV_HPP

#include "./nostr_event.hpp"
#include "./nostr_relay_interface.hpp"
#include <cstdio>
#include <thread>
#include "hv/WebSocketClient.h"

using namespace std::chrono_literals;

//quill

namespace cpp_nostr
{
class NostrRelayLibhv final : public NostrRelayInterface
{
   public:
    NostrRelayLibhv()
    {
        is_connected = false;
        setReconnectInterval(1000, 10000);
    }

    ~NostrRelayLibhv()
    {
        if (is_connected) {
            //this->unsubscribe();
            ws.close();
        }
    }

    bool connect(const std::string &relay)
    {
        ws.onopen = [this]() {
            this->onOpen();
        };
        ws.onmessage = [this](const std::string& msg) {
            this->onMessage(msg);
        };
        ws.onclose = [this]() {
            this->onClose();
        };
        auto result = ws.open(relay.c_str());
        is_connected = true;
        return true;
    }

    bool send(const std::string &str) override
    {
        while (!ws.isConnected()) {
            std::this_thread::sleep_for(100ms);
        }
        ws.send(str);
    }


    bool unsubscribe()
    {
        this->is_connected = false;
        return true;
    }

    bool setReconnectInterval(const uint32_t min_timeout, const uint32_t max_timeout)
    {
        if (this->is_connected) {
            return false;
        }

        reconn_setting_init(&this->reconn);
        this->reconn.min_delay    = min_timeout;
        this->reconn.max_delay    = max_timeout;
        this->reconn.delay_policy = 2;
        return true;
    }

   private:
    hv::WebSocketClient        ws;
    reconn_setting_t           reconn;
    bool                       is_connected;

    void onOpen(void)
    {
    }

    void onMessage(const std::string& msg)
    {
        this->callback(msg);
    }

    void onClose()
    {
    }
};
}

#endif
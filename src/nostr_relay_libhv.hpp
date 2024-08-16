#ifndef CPP_NOSTR_LIBHV_HPP
#define CPP_NOSTR_LIBHV_HPP

#include "./nostr_event.hpp"
#include "./nostr_relay_interface.hpp"
#include <cstdio>
#include <thread>
#include <memory>
#include "hv/WebSocketClient.h"
#include "hv/EventLoopThread.h"
#include <iostream>

using namespace std::chrono_literals;

// quill

namespace cpp_nostr
{
    class NostrRelayLibhv final : public NostrRelayInterface
    {

    private:
        hv::WebSocketClient *ws;
        hv::EventLoopThread *loop_thread;
        bool is_connected = false;

    public:
        NostrRelayLibhv() : NostrRelayInterface()
        {
            is_connected =false;
            loop_thread = new hv::EventLoopThread();
            loop_thread->start();

            ws = new hv::WebSocketClient(loop_thread->loop());
        }

        ~NostrRelayLibhv()
        {
            if (is_connected)
            {
                // this->unsubscribe();
                ws->close();
                loop_thread->stop();
                loop_thread->join();
            }
            delete ws;
            delete loop_thread;
        }

        bool connect(const std::string &relay)
        {
            setReconnectInterval(1000, 10000);
            ws->onopen = []() {
            };
            ws->onmessage = callback;
            ws->onclose = []() {
            };
            auto result = ws->open(relay.c_str());
            is_connected = true;
            return true;
        }

        bool send(const std::string &str) override
        {
            while (!ws->isConnected())
            {
                std::this_thread::sleep_for(100ms);
            }
            
            int res = ws->send(str);
            return (res==0);
        }

        bool unsubscribe()
        {
            this->is_connected = false;
            return true;
        }

        bool setReconnectInterval(const uint32_t min_timeout, const uint32_t max_timeout)
        {
            if (is_connected)
            {
                return false;
            }

            ws->setPingInterval(10000);

            reconn_setting_t reconn;

            reconn_setting_init(&reconn);
            reconn.min_delay = min_timeout;
            reconn.max_delay = max_timeout;
            reconn.delay_policy = 2;
            ws->setReconnect(&reconn);
            return true;
        }
    };
}

#endif
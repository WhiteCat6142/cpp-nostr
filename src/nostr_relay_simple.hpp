#ifndef CPP_NOSTR_RELAY_SIMPLE_HPP
#define CPP_NOSTR_RELAY_SIMPLE_HPP

#include "cpp_yyjson.hpp"

#include "nostr_event.hpp"
#include "nostr_subscription.hpp"
#include "nostr_relay_interface.hpp"
#include "nostr_relay.hpp"
#include "nostr_relay_utils.hpp"
#include "nostr_event_yyjson.hpp"

#include <string>
#include <vector>
#include <functional>
#include <future>
#include <unordered_map>
#include <utility>

using namespace yyjson;

namespace cpp_nostr
{
    class NostrRelaySimple final : public NostrRelay
    {
    private:
        NostrRelayInterface *relay;
        std::unordered_map<NostrEventSubId, NostrEventCallback> list;
        std::unordered_map<std::string, std::promise<bool>*> pub_list;

    public:
        NostrRelaySimple(NostrRelayInterface *relay_) : relay(relay_)
        {
            relay_->set_callback([this](const std::string &msg)
                                 {
                auto obj = read(msg);
                if(!obj.is_array())
                return;
                auto val = *obj.as_array();
                auto iter = val.begin();
                if(!(*iter).is_string())
                return;
                auto t = *((*iter).as_string());

                if(t=="EVENT")
                {
                    ++iter;
                    if(!(*iter).is_string())
                        return;
                    std::string id = std::string(*(*iter).as_string());
                    NostrEventSubId sub_id = (NostrEventSubId)std::stoul(id);
                    NostrEventCallback &fn = this->list.at(sub_id);
                    ++iter;
                    NostrEvent ev = cast<NostrEvent>(*iter);
                    if(NostrEventYYJSON::verify_event(ev))
                    fn(ev);
                    else
                    std::cout << "invalid sig" <<std::endl;
                }
                if(t=="OK")
                {
                    ++iter;
                    std::string id = std::string(*(*iter).as_string());
                    std::cout << id <<std::endl;
                    std::cout << (this->pub_list.size()) <<std::endl;
                    std::promise<bool> *pr = this->pub_list.at(id);
                    pr->set_value(true);
                    pub_list.erase(id);
                    delete pr;
                } });
        }
        NostrEventSubId subscribe(NostrEventCallback callback, const NostrSubscription &sub) override
        {
            NostrEventSubId sub_id = NostrRelayUtils::makeUniqueSubId();

            std::string s = NostrRelayUtils::makeSubscribeCommand(sub_id, sub);
            relay->send(s);
            list.insert_or_assign(sub_id, callback);
            return sub_id;
        }
        bool unsubscribe(const NostrEventSubId sub_id) override
        {
            relay->send(NostrRelayUtils::makeUnsubscribeCommand(sub_id));
            return true;
        }
        std::future<bool> publish(const std::string &id, const std::string &ev)
        {
            std::promise<bool> *pr = new std::promise<bool>();
            std::future<bool> ft = pr->get_future();
            if (!relay->send(NostrRelayUtils::makePublishCommand(ev)))
            {
                pr->set_value(false);
                return ft;
            }
            pub_list.insert_or_assign(std::string(id), pr);
            return ft;
        }
    };
}

#endif
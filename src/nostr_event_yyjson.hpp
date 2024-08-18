#ifndef CPP_NOSTR_EVENT_YYJSON_HPP_
#define CPP_NOSTR_EVENT_YYJSON_HPP_

#include "cpp_yyjson.hpp"
#include "./nostr_event.hpp"
// #include "logger_interface.hpp"
#include "./nostr_event_interface.hpp"

#include "./utils.hpp"
#include <string>
#include <vector>
#include <optional>
#include <secp256k1.h>
#include <secp256k1_schnorrsig.h>

using namespace yyjson;

namespace cpp_nostr
{
    class NostrEventYYJSON final : public NostrEventInterface
    {
    private:
        static const std::string writeJson(const NostrEvent &ev)
        {
            const auto a = array{0, ev.pubkey, ev.created_at, ev.kind, ev.tags, ev.content};
            return std::string(a.write());
        }

        static secp256k1_context *get_context(secp256k1_keypair &keypair, const std::vector<uint8_t> &sk_)
        {
            if (sk_.size() != 32)
                return nullptr;
            const unsigned char *sk = sk_.data();
            secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
            
            uint8_t randomize[32];
            if (!fill_random(randomize, sizeof(randomize)))
                goto FAIL;
            if (!secp256k1_context_randomize(ctx, randomize))
                goto FAIL;

            if (!secp256k1_ec_seckey_verify(ctx, sk))
                goto FAIL;

            if (!secp256k1_keypair_create(ctx, &keypair, sk))
                goto FAIL;

            return ctx;

            FAIL:
            secp256k1_context_destroy(ctx);
            std::cout << "fail" << std::endl;
            return nullptr;
        }

    public:
        NostrEventYYJSON()
        {
        }

        static bool finalize_event(NostrEvent &ev, const std::vector<uint8_t> &sk_)
        {
            secp256k1_keypair keypair;
            secp256k1_context *ctx = get_context(keypair, sk_);
            if (ctx == nullptr)
            return false;

            secp256k1_xonly_pubkey pubkey;
            if (!secp256k1_keypair_xonly_pub(ctx, &pubkey, nullptr, &keypair))
                goto FAIL;

            unsigned char serialized_pubkey[32];
            if (!secp256k1_xonly_pubkey_serialize(ctx, serialized_pubkey, &pubkey))
                goto FAIL;

            goto NEXT;
        FAIL:
            secp256k1_context_destroy(ctx);
            std::cout << "fail" << std::endl;
            return false;
        NEXT:
            ev.pubkey = bytes2hex(serialized_pubkey, sizeof(serialized_pubkey));

            const std::string input63 = writeJson(ev);

            const std::string id = sha256(input63.c_str(), input63.length());
            ev.id = id;
            const std::vector<uint8_t> digest = hex2bytes(id);

            uint8_t aux[32] = {0};
            if (!fill_random(aux, sizeof(aux)))
                goto FAIL;

            uint8_t sig[64] = {0};
            if (!secp256k1_schnorrsig_sign32(ctx, sig, digest.data(), &keypair, aux))
                goto FAIL;

            if (!secp256k1_schnorrsig_verify(ctx, sig, digest.data(), 32, &pubkey))
                goto FAIL;

            secp256k1_context_destroy(ctx);

            ev.sig = bytes2hex(sig, 64);
            return true;
        }

        static bool verify_event(const NostrEvent &ev)
        {
            bool res = false;

            const std::string input63 = writeJson(ev);
            const std::string id = sha256(input63.c_str(), input63.length());
            if (ev.id != id)
                return false;

            secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);
            std::vector<uint8_t> serialized_pubkey = hex2bytes(ev.pubkey);
            std::vector<uint8_t> sig = hex2bytes(ev.sig);
            std::vector<uint8_t> msg = hex2bytes(ev.id);
            secp256k1_xonly_pubkey pubkey;
            if (sig.size() != 64)
                goto FAIL;
            if (msg.size() != 32)
                goto FAIL;
            if (!secp256k1_xonly_pubkey_parse(ctx, &pubkey, serialized_pubkey.data()))
                goto FAIL;
            res = secp256k1_schnorrsig_verify(ctx, sig.data(), msg.data(), 32, &pubkey);
            secp256k1_context_destroy(ctx);
            return res;
        FAIL:
            secp256k1_context_destroy(ctx);
            return false;
        }

        static std::string encode(const NostrEvent &ev)
        {
            const auto a = object(ev);
            return std::string(a.write());
        }

        static NostrEvent decode(const std::string &s)
        {
            auto obj = read(s);
            return cast<NostrEvent>(obj);
        }

        static std::optional<std::vector<uint8_t>> get_publickey(const std::vector<uint8_t> &sk_)
        {
            secp256k1_keypair keypair;
            secp256k1_xonly_pubkey pubkey;

            secp256k1_context *ctx = get_context(keypair, sk_);
            if (ctx == nullptr)
            return std::nullopt;

            if (!secp256k1_keypair_xonly_pub(ctx, &pubkey, nullptr, &keypair))
                goto FAIL;

            unsigned char serialized_pubkey[32];
            if (!secp256k1_xonly_pubkey_serialize(ctx, serialized_pubkey, &pubkey))
                goto FAIL;

            goto NEXT;
        FAIL:
            secp256k1_context_destroy(ctx);
            std::cout << "fail" << std::endl;
            return std::nullopt;
        NEXT:

            std::vector<uint8_t> pk(serialized_pubkey, serialized_pubkey + sizeof(serialized_pubkey));
            return pk;
            
        }
    };
}

#endif
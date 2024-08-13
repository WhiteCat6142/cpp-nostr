#ifndef CPP_NOSTR_EVENT_SIGN_YYJSON_HPP_
#define CPP_NOSTR_EVENT_SIGN_YYJSON_HPP_

#include "cpp_yyjson.hpp"
#include "./nostr_event.hpp"
//#include "logger_interface.hpp"
#include "./nostr_event_sign_interface.hpp"

#include "./utils.hpp"
#include <secp256k1.h>
#include <secp256k1_schnorrsig.h>

using namespace yyjson;

namespace cpp_nostr
{
    class NostrEventSignYYJSON final : public NostrEventSignInterface
    {
    private:
        static const std::string writeJson(const NostrEvent &ev)
        {
            const auto a = array{0,ev.pubkey,ev.created_at,ev.kind,ev.tags,ev.content};
            return std::string(a.write());
        }

    public:
        bool sign_event(const uint8_t *sk, NostrEvent &ev) const
        {
            secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);

            if (!secp256k1_ec_seckey_verify(ctx, sk))
                goto FAIL;

            secp256k1_keypair keypair;
            if (!secp256k1_keypair_create(ctx, &keypair, sk))
                goto FAIL;

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

            const uint8_t *digest = hex2bytes(id).data();

            uint8_t aux[32];
            if (!fill_random(aux, sizeof(aux)))
                goto FAIL;
            std::cout << "fail" << std::endl;

            uint8_t sig[64] = {0};
            if (!secp256k1_schnorrsig_sign32(ctx, sig, digest, &keypair, aux))
                goto FAIL;
            secp256k1_context_destroy(ctx);

            ev.sig = bytes2hex(sig, 64);
            return true;
        }

        const std::string encode(const NostrEvent &ev) const
        {
            const auto a = object{
                {"id", ev.id},
                {"pubkey", ev.pubkey},
                {"created_at", ev.created_at},
                {"kind", ev.kind},
                {"tags", ev.tags},
                {"content", ev.content},
                {"sig", ev.sig}
            };
            return std::string(a.write());
        }
    };
}

#endif
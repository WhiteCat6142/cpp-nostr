#ifndef RX_NOSTR_EVENT_SIGN_YYJSON_HPP_
#define RX_NOSTR_EVENT_SIGN_YYJSON_HPP_

#include "yyjson.h"
#include "nostr_event.hpp"
#include "logger_interface.hpp"
#include "./nostr_event_sign_interface.hpp"

#include "random.h"
#include <include/secp256k1.h>
#include <include/secp256k1_schnorrsig.h>

#include <include/hash.h>

namespace rx_nostr
{
    class NostrEventSignYYJSON final : public NostrEventSignInterface
    {
    private:
        static char *digest2hex(const unsigned char *data, size_t len)
        {
            static const char hexchars[] = "0123456789abcdef";
            char *s = new char[(len << 1) + 1];

            size_t j = 0;
            for (size_t i = 0; i < len; i++)
            {
                unsigned char b = data[i];

                s[j++] = hexchars[b >> 4];
                s[j++] = hexchars[b & 0xF];
            }
            s[j] = 0;

            return s;
        }

        static char *writeJson(NostrEvent &ev)
        {
            yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);

            yyjson_mut_val *a = yyjson_mut_arr(doc);
            yyjson_mut_doc_set_root(doc, a);
            yyjson_mut_arr_add_int(doc, a, 0);
            yyjson_mut_arr_add_str(doc, a, ev.pubkey);
            yyjson_mut_arr_add_uint(doc, a, ev.created_at);
            yyjson_mut_arr_add_int(doc, a, ev.kind);
            auto arr = yyjson_mut_arr_add_arr(doc, a);
            for (auto &x : ev.tags)
            {
                auto child = yyjson_mut_arr_add_arr(doc, arr);
                for (auto &str : x)
                {
                    yyjson_mut_arr_add_str(doc, child, str);
                }
            }
            yyjson_mut_arr_add_str(doc, a, ev.content);

            char *json = yyjson_mut_write(doc, 0, NULL);
            yyjson_mut_doc_free(doc);
            return json;
        }

    public:
        bool sign_event(const unsigned char *sk, NostrEvent &ev) const
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

            ev.pubkey = digest2hex(serialized_pubkey, sizeof(serialized_pubkey));

            char *input63 = writeJson(ev);

            unsigned char *digest = sha256_hash((const unsigned char *)input63, strlen(input63));
            free(input63);

            auto id = digest2hex(digest, 32);
            ev.id = id;

            unsigned char aux[32];
            if (!fill_random(aux, sizeof(aux)))
                goto FAIL;

            uint8_t sig[64] = {0};
            if (!secp256k1_schnorrsig_sign32(ctx, sig, digest, &keypair, aux))
                goto FAIL;
            secp256k1_context_destroy(ctx);

            ev.sig = digest2hex(sig, 64);
            return true;
        }

        std::string encode(NostrEvent &ev) const
        {
            yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);

            yyjson_mut_val *a = yyjson_mut_obj(doc);
            yyjson_mut_doc_set_root(doc, a);
            yyjson_mut_obj_add_str(doc, a, "id", ev.id);
            yyjson_mut_obj_add_str(doc, a, "pubkey", ev.pubkey);
            yyjson_mut_obj_add_uint(doc, a, "created_at", ev.created_at);
            yyjson_mut_obj_add_int(doc, a, "kind", ev.kind);
            auto arr = yyjson_mut_obj_add_arr(doc, a, "tags");
            for (auto &x : ev.tags)
            {
                auto child = yyjson_mut_arr_add_arr(doc, arr);
                for (auto &str : x)
                {
                    yyjson_mut_arr_add_str(doc, child, str);
                }
            }
            yyjson_mut_obj_add_str(doc, a, "content", ev.content);
            yyjson_mut_obj_add_str(doc, a, "sig", ev.sig);

            char *json = yyjson_mut_write(doc, 0, NULL);
            yyjson_mut_doc_free(doc);
            return std::string(json);
        }
    };
} // namespace rx_nostr

#endif // RX_NOSTR_EVENT_SIGN_YYJSON_HPP_
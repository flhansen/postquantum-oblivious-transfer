#ifndef OPENCRYPTO_OBLIVIOUS_TRANSFER_H
#define OPENCRYPTO_OBLIVIOUS_TRANSFER_H

#include "type.h"

struct OpenCrypto_OT_public_parameters {
    const byte* generator;
    const byte* modulus;
};

struct OpenCrypto_OT_sender_keys {
    const byte* secret_key;
    const byte* public_key;
};

struct OpenCrypto_OT_receiver_keys {
    const byte* secret_key;
    const byte* public_key;
    const byte* k_b;
};

void OpenCrypto_OT_keygen_sender(struct OpenCrypto_OT_public_parameters pp, struct OpenCrypto_OT_sender_keys* out_sender);
void OpenCrypto_OT_keygen_receiver(struct OpenCrypto_OT_public_parameters pp, int public_key_sender, int c, struct OpenCrypto_OT_receiver_keys* out_receiver);
void OpenCrypto_OT_encrypt(struct OpenCrypto_OT_public_parameters pp, struct OpenCrypto_OT_sender_keys sender, int public_key_receiver, const byte** messages, unsigned int number_messages, byte** out_ciphers);
void OpenCrypto_OT_decrypt(struct OpenCrypto_OT_receiver_keys receiver, int* ciphers, int number_ciphers, int* out_messages);

#endif
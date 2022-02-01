#ifndef OPENCRYPTO_OBLIVIOUS_TRANSFER_H
#define OPENCRYPTO_OBLIVIOUS_TRANSFER_H

#include "type.h"

struct OpenCrypto_OT_public_parameters {
    int generator;
    int modulus;
};

struct OpenCrypto_OT_sender_keys {
    int secret_key;
    int public_key;
};

struct OpenCrypto_OT_receiver_keys {
    int secret_key;
    int public_key;
    int k_b;
};

void OpenCrypto_OT_keygen_sender(struct OpenCrypto_OT_public_parameters pp, struct OpenCrypto_OT_sender_keys* out_sender);
void OpenCrypto_OT_keygen_receiver(struct OpenCrypto_OT_public_parameters pp, int public_key_sender, int c, struct OpenCrypto_OT_receiver_keys* out_receiver);
void OpenCrypto_OT_encrypt(struct OpenCrypto_OT_public_parameters pp, struct OpenCrypto_OT_sender_keys sender, int public_key_receiver, int* messages, unsigned int number_messages, int* out_ciphers);
void OpenCrypto_OT_decrypt(struct OpenCrypto_OT_receiver_keys receiver, int* ciphers, int number_ciphers, int* out_messages);

#endif
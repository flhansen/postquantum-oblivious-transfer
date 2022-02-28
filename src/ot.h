#ifndef OPENCRYPTO_OBLIVIOUS_TRANSFER_H
#define OPENCRYPTO_OBLIVIOUS_TRANSFER_H

#include <gmp.h>

#include "type.h"

struct _OpenCrypto_OT_public_parameters {
    unsigned int number_bits;
    mpz_t generator;
    mpz_t modulus;
    gmp_randstate_t rand_state;
};

struct _OpenCrypto_OT_sender_keys {
    mpz_t secret_key;
    mpz_t public_key;
};

struct _OpenCrypto_OT_receiver_keys {
    mpz_t secret_key;
    mpz_t public_key;
    mpz_t k_b;
};

typedef struct _OpenCrypto_OT_public_parameters OpenCrypto_OT_public_parameters;
typedef struct _OpenCrypto_OT_sender_keys OpenCrypto_OT_sender_keys;
typedef struct _OpenCrypto_OT_receiver_keys OpenCrypto_OT_receiver_keys;

void OpenCrypto_OT_init(OpenCrypto_OT_public_parameters *pp);
void OpenCrypto_OT_pp_clear(OpenCrypto_OT_public_parameters *pp);
void OpenCrypto_OT_sender_clear(OpenCrypto_OT_sender_keys *sender);
void OpenCrypto_OT_receiver_clear(OpenCrypto_OT_receiver_keys *receiver);
void OpenCrypto_OT_init_sender(OpenCrypto_OT_public_parameters* pp, OpenCrypto_OT_sender_keys* out_sender);
void OpenCrypto_OT_init_receiver(OpenCrypto_OT_public_parameters* pp, const mpz_t public_key_sender, int c, OpenCrypto_OT_receiver_keys* out_receiver);
void OpenCrypto_OT_encrypt(OpenCrypto_OT_public_parameters* pp, OpenCrypto_OT_sender_keys* sender, const mpz_t public_key_receiver, byte messages[2][255], unsigned int number_messages, byte out_ciphers[2][255]);
void OpenCrypto_OT_decrypt(OpenCrypto_OT_public_parameters* pp, OpenCrypto_OT_receiver_keys* receiver, byte ciphers[2][255], unsigned int number_ciphers, byte out_messages[2][255]);

#endif

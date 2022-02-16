#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <gmp.h>

#include "ot.h"
#include "math.h"
#include "crypto.h"

void OpenCrypto_OT_init(OpenCrypto_OT_public_parameters* pp) {
    mpz_init(pp->generator);
    mpz_init(pp->modulus);

    gmp_randinit_default(pp->rand_state);
    gmp_randseed_ui(pp->rand_state, clock());
}

void OpenCrypto_OT_pp_clear(OpenCrypto_OT_public_parameters *pp) {
    mpz_clear(pp->generator);
    mpz_clear(pp->modulus);
    pp->number_bits = 0;
}

void OpenCrypto_OT_sender_clear(OpenCrypto_OT_sender_keys *sender) {
    mpz_clear(sender->secret_key);
    mpz_clear(sender->public_key);
}

void OpenCrypto_OT_receiver_clear(OpenCrypto_OT_receiver_keys *receiver) {
    mpz_clear(receiver->secret_key);
    mpz_clear(receiver->public_key);
    mpz_clear(receiver->k_b);
}

void OpenCrypto_OT_init_sender(OpenCrypto_OT_public_parameters* pp, OpenCrypto_OT_sender_keys* out_sender) {
    mpz_init(out_sender->secret_key);
    mpz_init(out_sender->public_key);

    mpz_urandomb(out_sender->secret_key, pp->rand_state, pp->number_bits);
    mpz_powm(out_sender->public_key, pp->generator, out_sender->secret_key, pp->modulus);
}

void OpenCrypto_OT_init_receiver(OpenCrypto_OT_public_parameters* pp, const mpz_t public_key_sender, int c, OpenCrypto_OT_receiver_keys* out_receiver) {
    mpz_init(out_receiver->secret_key);
    mpz_init(out_receiver->public_key);
    mpz_init(out_receiver->k_b);

    mpz_urandomb(out_receiver->secret_key, pp->rand_state, pp->number_bits);

    mpz_powm(out_receiver->k_b, public_key_sender, out_receiver->secret_key, pp->modulus);

    mpz_powm(out_receiver->public_key, pp->generator, out_receiver->secret_key, pp->modulus);

    mpz_t A;
    mpz_init(A);
    mpz_powm_ui(A, public_key_sender, c, pp->modulus);
    mpz_mul(out_receiver->public_key, A, out_receiver->public_key);
    mpz_clear(A);
}

void OpenCrypto_OT_encrypt(OpenCrypto_OT_public_parameters* pp, OpenCrypto_OT_sender_keys* sender, const mpz_t public_key_receiver, byte messages[][255], unsigned int number_messages, byte out_ciphers[][255]) {
    // Calculate key k_i = (PK_r / (PK_s)^i)^(SK_s) % p
    for (int i = 0; i < number_messages; i++) {
        mpz_t k_i;
        mpz_init(k_i);

        mpz_t A;
        mpz_init(A);
        mpz_powm_ui(A, sender->public_key, i, pp->modulus);

        mpz_div(k_i, public_key_receiver, A);
        mpz_powm(k_i, k_i, sender->secret_key, pp->modulus);

        OpenCrypto_Crypto_XOR_encrypt(out_ciphers[i], messages[i], strlen(messages[i]), k_i);

        mpz_clear(A);
        mpz_clear(k_i);
    }
}

void OpenCrypto_OT_decrypt(OpenCrypto_OT_public_parameters* pp, OpenCrypto_OT_receiver_keys* receiver, byte ciphers[][255], unsigned int number_ciphers, byte out_messages[][255]) {
    for (int i = 0; i < number_ciphers; i++) {
        OpenCrypto_Crypto_XOR_decrypt(out_messages[i], ciphers[i], strlen(ciphers[i]), receiver->k_b);
        out_messages[i][strlen(ciphers[i])] = '\0';
    }
}
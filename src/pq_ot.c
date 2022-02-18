#include <time.h>

#include "pq_ot.h"

void OpenCrypto_PQOT_init_public_params(PQOT_public_parameters *pp) {
    gmp_randinit_default(pp->rand_state);
    gmp_randseed_ui(pp->rand_state, clock());
}

void OpenCrypto_PQOT_init_sender(PQOT_sender_keys *out_sender, PQOT_public_parameters *pp) {

}

void OpenCrypto_PQOT_init_receiver(PQOT_receiver_keys *out_receiver, PQOT_public_parameters* pp, int c, const mpz_t public_key_sender) {

}

void OpenCrypto_PQOT_clear_public_params(PQOT_public_parameters *pp) {

}

void OpenCrypto_PQOT_clear_sender(PQOT_sender_keys *sender) {

}

void OpenCrypto_PQOT_clear_receiver(PQOT_receiver_keys *receiver) {

}

void OpenCrypto_PQOT_encrypt(byte out_ciphers[][255], PQOT_public_parameters *pp, byte messages[][255], unsigned int number_messages, PQOT_sender_keys *sender, const mpz_t public_key_receiver) {

}

void OpenCrypto_PQOT_decrypt(byte out_messages[][255], PQOT_public_parameters *pp, byte ciphers[][255], unsigned int number_ciphers, PQOT_receiver_keys *reveiver) {

}

#include <gmp.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "pqot.h"
#include "../includes/P434_api.h"

void OpenCrypto_PQOT_init_public_params(PQOT_public_parameters *pp) {

}

void OpenCrypto_PQOT_init_sender(PQOT_sender_keys *out_sender, PQOT_public_parameters *pp) {

}

void OpenCrypto_PQOT_init_receiver(PQOT_receiver_keys *out_receiver, PQOT_public_parameters *pp, int c, const mpz_t public_key_sender) {

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

#ifndef OPENCRYPTO_CRYPTO_H
#define OPENCRYPTO_CRYPTO_H

#include <gmp.h>

#include "type.h"

void OpenCrypto_Crypto_XOR_encrypt(byte* out_result, byte* message, unsigned int message_length, mpz_t key);
void OpenCrypto_Crypto_XOR_decrypt(byte* out_result, byte* cipher, unsigned int message_length, mpz_t key);

#endif
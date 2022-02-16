#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include <openssl/sha.h>

#include "crypto.h"
#include "type.h"

void OpenCrypto_Crypto_XOR_encrypt(byte* out_result, byte* message, unsigned int message_length, mpz_t key) {
    byte *key_bytes = (byte*)key->_mp_d;
    int number_key_bytes = key->_mp_size * sizeof(key->_mp_d[0]);

    byte key_hash[SHA256_DIGEST_LENGTH];
    SHA256(key_bytes, number_key_bytes, key_hash);

    for (int i = 0; i < message_length; i++)
        out_result[i] = message[i] ^ key_hash[i % SHA256_DIGEST_LENGTH];
}

void OpenCrypto_Crypto_XOR_decrypt(byte* out_result, byte* cipher, unsigned int message_length, mpz_t key) {
    byte *key_bytes = (byte*)key->_mp_d;
    int number_key_bytes = key->_mp_size * sizeof(key->_mp_d[0]);

    byte key_hash[SHA256_DIGEST_LENGTH];
    SHA256(key_bytes, number_key_bytes, key_hash);

    for (int i = 0; i < message_length; i++)
        out_result[i] = cipher[i] ^ key_hash[i % SHA256_DIGEST_LENGTH];
}
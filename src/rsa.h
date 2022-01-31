#ifndef OPENCRYPTO_RSA_H
#define OPENCRYPTO_RSA_H

void OpenCrypto_RSA_keygen(int* out_public_key, int* out_secret_key);
int OpenCrypto_RSA_encrypt(int message, int public_key, int n);
int OpenCrypto_RSA_decrypt(int cipher, int secret_key, int n);
int OpenCrypto_RSA_generate_keypair(int* out_public_key, int* out_secret_key, int totient);

#endif
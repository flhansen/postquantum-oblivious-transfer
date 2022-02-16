#include "rsa.h"
#include "math.h"

void OpenCrypto_RSA_keygen(int* out_public_key, int* out_secret_key) {
    int p = OpenCrypto_math_random_prime();
    int q = OpenCrypto_math_random_prime();

    int modulus = p * q;
    int totient = (p-1) * (q-1);
    OpenCrypto_RSA_generate_keypair(out_public_key, out_secret_key, totient);
}

int OpenCrypto_RSA_encrypt(int message, int public_key, int n) {
    return OpenCrypto_math_fast_power(message, public_key, n);
}

int OpenCrypto_RSA_decrypt(int cipher, int secret_key, int n) {
    return OpenCrypto_math_fast_power(cipher, secret_key, n);
}

int OpenCrypto_RSA_generate_keypair(int* out_public_key, int* out_secret_key, int totient) {
    // Start by generating a random public key
    int coprime;

    do {
        coprime = rand();
    } while(OpenCrypto_math_gcd(totient, coprime) != 1);

    coprime %= totient;

    // Continue generating a random secret key
    int secret;

    do {
        secret = rand();
    } while((secret * coprime) % totient != 1);

    secret %= totient;

    // Assign the outputs
    *out_public_key = coprime;
    *out_secret_key = secret;
}
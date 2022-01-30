#include "ot.h"
#include "math.h"

void OpenCrypto_ot_keygen_sender(struct OpenCrypto_ot_public_parameters pp, struct OpenCrypto_ot_sender_keys* out_sender) {
    int a = rand() % pp.modulus;
    out_sender->secret_key = a;
    out_sender->public_key = OpenCrypto_math_fast_power(pp.generator, a, pp.modulus);
}

void OpenCrypto_ot_keygen_receiver(struct OpenCrypto_ot_public_parameters pp, int public_key_sender, struct OpenCrypto_ot_receiver_keys* out_receiver) {
    int b = rand() % pp.modulus;
    char c = rand() % 2;

    out_receiver->secret_key = b;
    out_receiver->k_b = OpenCrypto_math_fast_power(public_key_sender, b, pp.modulus);
    out_receiver->public_key = OpenCrypto_math_fast_power(pp.generator, b, pp.modulus);

    if (c == 1)
        out_receiver->public_key = (out_receiver->public_key * public_key_sender) % pp.modulus;
}

void OpenCrypto_ot_encrypt(struct OpenCrypto_ot_public_parameters pp, struct OpenCrypto_ot_sender_keys sender, int public_key_receiver, int* messages, unsigned int number_messages, int* out_ciphers) {
    int k_0 = OpenCrypto_math_fast_power(public_key_receiver, sender.secret_key, pp.modulus);
    int k_1 = OpenCrypto_math_fast_power(public_key_receiver / sender.public_key, sender.secret_key, pp.modulus);

    out_ciphers[0] = messages[0] ^ k_0;
    out_ciphers[1] = messages[1] ^ k_1;
}

int OpenCrypto_ot_decrypt(struct OpenCrypto_ot_receiver_keys receiver, int e_c) {
    return e_c ^ receiver.k_b;
}
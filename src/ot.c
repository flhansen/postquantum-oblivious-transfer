#include "ot.h"
#include "math.h"

void OpenCrypto_OT_keygen_sender(struct OpenCrypto_OT_public_parameters pp, struct OpenCrypto_OT_sender_keys* out_sender) {
    int a = rand() % pp.modulus;
    out_sender->secret_key = a;
    out_sender->public_key = OpenCrypto_math_fast_power(pp.generator, a, pp.modulus);
}

void OpenCrypto_OT_keygen_receiver(struct OpenCrypto_OT_public_parameters pp, int public_key_sender, int c, struct OpenCrypto_OT_receiver_keys* out_receiver) {
    int b = rand() % pp.modulus;

    out_receiver->secret_key = b;
    out_receiver->k_b = OpenCrypto_math_fast_power(public_key_sender, b, pp.modulus);
    out_receiver->public_key = OpenCrypto_math_fast_power(pp.generator, b, pp.modulus);

    if (c == 1)
        out_receiver->public_key = (out_receiver->public_key * public_key_sender) % pp.modulus;
}

void OpenCrypto_OT_encrypt(struct OpenCrypto_OT_public_parameters pp, struct OpenCrypto_OT_sender_keys sender, int public_key_receiver, const byte** messages, unsigned int number_messages, byte** out_ciphers) {
    int k_0 = OpenCrypto_math_fast_power(public_key_receiver, sender.secret_key, pp.modulus);
    int k_1 = OpenCrypto_math_fast_power(public_key_receiver / sender.public_key, sender.secret_key, pp.modulus);

    OpenCrypto_math_xor(messages[0], k_0, strlen(k_0), out_ciphers[0]);
    OpenCrypto_math_xor(messages[1], k_1, strlen(k_1), out_ciphers[1]);
}

void OpenCrypto_OT_decrypt(struct OpenCrypto_OT_receiver_keys receiver, const byte** ciphers, int number_ciphers, byte** out_messages) {
    for (int i = 0; i < number_ciphers; i++)
        OpenCrypto_math_xor(ciphers[i], receiver.k_b, strlen(receiver.k_b), out_messages[i]);
}
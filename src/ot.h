#ifndef OPENCRYPTO_OBLIVIOUS_TRANSFER_H
#define OPENCRYPTO_OBLIVIOUS_TRANSFER_H

struct OpenCrypto_ot_public_parameters {
    int generator;
    int modulus;
};

struct OpenCrypto_ot_sender_keys {
    int secret_key;
    int public_key;
};

struct OpenCrypto_ot_receiver_keys {
    int secret_key;
    int public_key;
    int k_b;
};

void OpenCrypto_ot_keygen_sender(struct OpenCrypto_ot_public_parameters pp, struct OpenCrypto_ot_sender_keys* out_sender);
void OpenCrypto_ot_keygen_receiver(struct OpenCrypto_ot_public_parameters pp, int public_key_sender, struct OpenCrypto_ot_receiver_keys* out_receiver);
void OpenCrypto_ot_encrypt(struct OpenCrypto_ot_public_parameters pp, struct OpenCrypto_ot_sender_keys sender, int public_key_receiver, int* messages, unsigned int number_messages, int* out_ciphers);
int OpenCrypto_ot_decrypt(struct OpenCrypto_ot_receiver_keys receiver, int e_c);

#endif
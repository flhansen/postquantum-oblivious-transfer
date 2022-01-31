#include "test.h"
#include "../src/utils.h"
#include "../src/math.h"
#include "../src/ot.h"

void test_ot_protocol() {
    struct OpenCrypto_ot_public_parameters pp;
    pp.generator = 3;
    pp.modulus = 11;

    // Sender-side initialization
    int messages[] = {10, 3};
    struct OpenCrypto_ot_sender_keys sender;
    OpenCrypto_ot_keygen_sender(pp, &sender);

    // Receiver-side initialization
    int ciphers[2];
    int choice = 0;
    struct OpenCrypto_ot_receiver_keys receiver;
    OpenCrypto_ot_keygen_receiver(pp, sender.public_key, choice, &receiver);

    // Sender-side encryption
    OpenCrypto_ot_encrypt(pp, sender, receiver.public_key, messages, ARRAY_LENGTH(messages), ciphers);

    // Receiver-side decryption
    int reconstructed_messages[2];
    OpenCrypto_ot_decrypt(receiver, ciphers, ARRAY_LENGTH(ciphers), reconstructed_messages);

    // Check, if the receiver is available to decrypt the chosen message only
    EXPECT_TO_BE_EQUAL(reconstructed_messages[0], messages[0]);
    EXPECT_NOT_TO_BE_EQUAL(reconstructed_messages[1], messages[1]);
}

int main() {
    void (*tests[])() = { test_ot_protocol };
    TEST_RUN(tests, 1);

    return 0;
}
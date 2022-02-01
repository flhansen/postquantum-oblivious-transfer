#include <stdio.h>

#include "test.h"
#include "../src/utils.h"
#include "../src/math.h"
#include "../src/ot.h"

void test_ot_protocol() {
    struct OpenCrypto_OT_public_parameters pp;
    pp.generator = 3;
    pp.modulus = 11;

    // Sender-side initialization
    int messages[] = {10, 3};
    struct OpenCrypto_OT_sender_keys sender;
    OpenCrypto_OT_keygen_sender(pp, &sender);

    // Receiver-side initialization
    int ciphers[2];
    int choice = 0;
    struct OpenCrypto_OT_receiver_keys receiver;
    OpenCrypto_OT_keygen_receiver(pp, sender.public_key, choice, &receiver);

    // Sender-side encryption
    OpenCrypto_OT_encrypt(pp, sender, receiver.public_key, messages, ARRAY_LENGTH(messages), ciphers);

    // Receiver-side decryption
    int reconstructed_messages[2];
    OpenCrypto_OT_decrypt(receiver, ciphers, ARRAY_LENGTH(ciphers), reconstructed_messages);

    // Check, if the receiver is available to decrypt the chosen message only
    EXPECT_TO_BE_EQUAL(reconstructed_messages[0], messages[0]);
    EXPECT_NOT_TO_BE_EQUAL(reconstructed_messages[1], messages[1]);
}

void test(const char** messages, unsigned int number_messages) {
    for (int i = 0; i < number_messages; i++) {
        printf("%d, %s\n", i, messages[i]);
    }
}

void test_OT_protocol() {
    const char* messages[] = {"Test me, I'm a string!\0", "I'm a string, too!\0"};
    test(messages, ARRAY_LENGTH(messages));
}

int main() {
    void (*tests[])() = { test_OT_protocol };
    TEST_RUN(tests, 1);

    return 0;
}
#include <stdio.h>

#include "test.h"
#include "../src/utils.h"
#include "../src/math.h"
#include "../src/ot.h"

test_result test_ot_protocol() {
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
    test_result result = expect_to_be(reconstructed_messages[0], messages[0]);
    result            &= expect_not_to_be(reconstructed_messages[1], messages[1]);

    return result;
}

int main() {
    unit_test tests[] = { test_ot_protocol };
    TEST_RUN(tests, 1);

    return 0;
}
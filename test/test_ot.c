#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

#include "test.h"
#include "../src/utils.h"
#include "../src/math.h"
#include "../src/ot.h"

test_result test_ot_protocol() {
    OpenCrypto_OT_public_parameters pp;
    OpenCrypto_OT_init(&pp);
    mpz_set_ui(pp.generator, 5);
    mpz_set_ui(pp.modulus, 23);
    pp.number_bits = 128;

    // Sender side initialization
    OpenCrypto_OT_sender_keys sender;
    OpenCrypto_OT_keygen_sender(&pp, &sender);

    // Receiver side initialization
    int choice = 1;
    OpenCrypto_OT_receiver_keys receiver;
    OpenCrypto_OT_keygen_receiver(&pp, sender.public_key, choice, &receiver);

    // Sender-side encryption
    unsigned char ciphers[2][255];
    memset(ciphers, 0, 2*255);

    unsigned char messages[][255] = {
        "Test me, I'm a short string!",
        "Hello friend, can you read me?"
    };

    OpenCrypto_OT_encrypt(&pp, &sender, receiver.public_key, messages, 2, ciphers);

    // Receiver-side decryption
    unsigned char reconstructed_messages[2][255];
    OpenCrypto_OT_decrypt(&pp, &receiver, ciphers, 2, reconstructed_messages);

    // Cleanup
    OpenCrypto_OT_pp_clear(&pp);
    OpenCrypto_OT_sender_clear(&sender);
    OpenCrypto_OT_receiver_clear(&receiver);

    test_result test_result = TEST_PASSED;
    test_result &= expect_int_to_be(strncmp(reconstructed_messages[1], messages[1], strlen(messages[1])), 0);
    test_result &= expect_int_not_to_be(strncmp(reconstructed_messages[0], messages[0], strlen(messages[0])), 0);

    return test_result;
}

int main() {
    unit_test tests[] = { TEST(test_ot_protocol) };
    run_tests(tests, ARRAY_LENGTH(tests));

    return 0;
}
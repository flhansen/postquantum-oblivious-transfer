#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

#include "test.h"
#include "../src/utils.h"
#include "../src/math.h"
#include "../src/ot.h"
#include "../src/pqot.h"

test_result test_pqot_protocol() {
    PQOT_public_parameters pp = {0};
    OpenCrypto_PQOT_init_public_params(&pp);

    OpenCrypto_PQOT_clear_public_params(&pp);
    return TEST_PASSED;
}

test_result test_ot_protocol() {
    OpenCrypto_OT_public_parameters pp;
    OpenCrypto_OT_init(&pp);
    mpz_set_ui(pp.generator, 5);
    mpz_set_ui(pp.modulus, 23);
    pp.number_bits = 128;

    // Sender side initialization
    OpenCrypto_OT_sender_keys sender;
    OpenCrypto_OT_init_sender(&pp, &sender);

    // Receiver side initialization
    int choice = 3;
    OpenCrypto_OT_receiver_keys receiver;
    OpenCrypto_OT_init_receiver(&pp, sender.public_key, choice, &receiver);

    // Sender-side encryption
    unsigned char ciphers[5][255];
    memset(ciphers, 0, 5*255);

    unsigned char messages[][255] = {
        "Test me, I'm a short string!",
        "Hello friend, can you read me?",
        "Goodbye, my almost lover",
        "Singing the deathmatch",
        "I'm in the dark"
    };

    OpenCrypto_OT_encrypt(&pp, &sender, receiver.public_key, messages, 5, ciphers);

    // Receiver-side decryption
    unsigned char reconstructed_messages[5][255];
    OpenCrypto_OT_decrypt(&pp, &receiver, ciphers, 5, reconstructed_messages);

    // Cleanup
    OpenCrypto_OT_pp_clear(&pp);
    OpenCrypto_OT_sender_clear(&sender);
    OpenCrypto_OT_receiver_clear(&receiver);

    test_result test_result = TEST_PASSED;

    for (int i = 0; i < 5; i++) {
        if (i == choice)
            test_result &= expect_int_to_be(strncmp(reconstructed_messages[i], messages[i], strlen(messages[i])), 0);
        else
            test_result &= expect_int_not_to_be(strncmp(reconstructed_messages[i], messages[i], strlen(messages[i])), 0);
    }

    return test_result;
}

int main() {
    unit_test tests[] = { TEST(test_ot_protocol), TEST(test_pqot_protocol) };
    run_tests(tests, ARRAY_LENGTH(tests));

    return 0;
}

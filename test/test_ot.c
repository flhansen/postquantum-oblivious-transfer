#include "test.h"
#include "../src/ot.h"
#include "../src/math.h"

void test_keygen_sender() {
    struct OpenCrypto_ot_public_parameters pp;
    pp.generator = 3;
    pp.modulus = 11;

    struct OpenCrypto_ot_sender_keys sender;
    OpenCrypto_ot_keygen_sender(pp, &sender);
    ASSERT_EQUALS(sender.public_key, OpenCrypto_math_fast_power(pp.generator, sender.secret_key, pp.modulus));
}

void test_keygen_receiver() {
    struct OpenCrypto_ot_public_parameters pp;
    pp.generator = 3;
    pp.modulus = 11;

    struct OpenCrypto_ot_sender_keys sender;
    OpenCrypto_ot_keygen_sender(pp, &sender);

    struct OpenCrypto_ot_receiver_keys receiver;
    OpenCrypto_ot_keygen_receiver(pp, sender.public_key, &receiver);

    // TODO: Implement assertions here.
}

void test_encrypt() {
    // TODO: Implement assertions here.
}

void test_decrypt() {
    // TODO: Implement assertions here.
}

int main() {
    void (*tests[])() = {test_keygen_sender, test_keygen_receiver, test_encrypt, test_decrypt};
    TEST_RUN(tests, 4);

    return 0;
}
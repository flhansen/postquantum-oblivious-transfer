#include "test.h"
#include "../src/rsa.h"

test_result test_rsa_protocol() {
    int n = 11;
    int public_key_alice, secret_key_alice;
    OpenCrypto_RSA_keygen(&public_key_alice, &secret_key_alice);

    test_result result = TEST_PASSED;

    for (int i = 0; i < 100; i++) {
        int message = rand() % n;
        int cipher = OpenCrypto_RSA_encrypt(message, public_key_alice, n);
        int reconstructed_message = OpenCrypto_RSA_decrypt(cipher, secret_key_alice, n);
        result &= expect_to_be(reconstructed_message, message);
    }

    return result;
}

int main() {
    unit_test tests[] = { test_rsa_protocol };
    TEST_RUN(tests, 1);
}
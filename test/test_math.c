#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"
#include "../src/type.h"
#include "../src/utils.h"
#include "../src/math.h"

test_result test_math_xor() {
    const byte* message = "This is a test string!";
    const byte* key = "th1s1s4t3stk3y";

    byte* result = malloc(strlen(message) + 1);
    OpenCrypto_math_xor(message, key, strlen(key), result);
    result[strlen(message)] = '\0';

    test_result test_result = TEST_PASSED;

    for (int i = 0; i < strlen(message); i++)
        test_result &= expect_to_be(result[i], message[i] ^ key[i % strlen(key)]);

    free(result);
    return test_result;
}

int main() {
    unit_test tests[] = { test_math_xor };
    TEST_RUN(tests, 1);

    return 0;
}
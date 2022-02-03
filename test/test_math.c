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
        test_result &= expect_byte_to_be(result[i], message[i] ^ key[i % strlen(key)]);

    free(result);
    return test_result;
}

test_result test_math_add() {
    const byte number1[] = {0x00, 0xff}; // 255
    const byte number2[] = {0x00, 0x01}; // 1

    // Initialize the result with 0 and execute the add operation over number1
    // and number2.
    byte result[] = {0x00, 0x00}; // 0
    OpenCrypto_math_add(number1, number2, 2, result);

    // Check if the result is 256. We just have to check, if the 9th bit is set
    // to 1 (2^8 = 256).
    expect_byte_to_be(result[0], 0x01);
    expect_byte_to_be(result[1], 0x00);
}

test_result test_math_div() {
    return TEST_ERROR;
}

int main() {
    unit_test tests[] = { test_math_xor, test_math_add, test_math_div };
    TEST_RUN(tests, 3);

    return 0;
}
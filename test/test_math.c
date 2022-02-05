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

test_result test_math_inv() {
    const byte number[] = {0x04, 0xF7};
    const byte expectation[] = {0xFB, 0x08};

    byte inverse[] = {0x00, 0x00};
    OpenCrypto_math_inv(number, 2, inverse);

    test_result result = TEST_PASSED;
    result &= expect_byte_to_be(inverse[0], expectation[0]);
    result &= expect_byte_to_be(inverse[1], expectation[1]);
    return result;
}

test_result test_math_add() {
    const byte number1[] = {0x00, 0xff}; // 255
    const byte number2[] = {0xff, 0xff}; 

    // Initialize the result with 0 and execute the add operation over number1
    // and number2.
    byte result[] = {0x00, 0x00}; // 0
    OpenCrypto_math_add(number1, number2, 2, result);

    // Check if the result is 256. We just have to check, if the 9th bit is set
    // to 1 (2^8 = 256).
    test_result test_result = TEST_PASSED;
    test_result &= expect_byte_to_be(result[0], 0x00);
    test_result &= expect_byte_to_be(result[1], 0xff);
    return test_result;
}

test_result test_math_sub() {
    const byte number1[] = {0xB7, 0xA9};
    const byte number2[] = {0x8A, 0x01};

    byte result[] = {0x00, 0x00};
    OpenCrypto_math_sub(number1, number2, 2, result);

    test_result test_result = TEST_PASSED;
    test_result &= expect_byte_to_be(result[0], 0x2D);
    test_result &= expect_byte_to_be(result[1], 0xA8);
    return test_result;
}

test_result test_math_less_than() {
    const byte number[] = {0x01, 0x00};
    const byte bigger_number[] = {0xff, 0x00};
    const byte lower_number[] = {0x00, 0xff};

    test_result test_result = TEST_PASSED;
    test_result &= expect_int_to_be(OpenCrypto_math_less_than(number, bigger_number, 2), 1);
    test_result &= expect_int_to_be(OpenCrypto_math_less_than(number, lower_number, 2), 0);
    return test_result;
}

test_result test_math_div() {
    return TEST_ERROR;
}

int main() {
    unit_test tests[] = { TEST(test_math_xor), TEST(test_math_inv), TEST(test_math_add), TEST(test_math_sub), TEST(test_math_less_than), TEST(test_math_div) };
    run_tests(tests, ARRAY_LENGTH(tests));

    return 0;
}
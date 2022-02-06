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
    const byte number3[] = {0x00, 0x00};
    const byte number4[] = {0xB7, 0xA9};

    byte result1[] = {0x00, 0x00};
    byte result2[] = {0x00, 0x00};
    byte result3[] = {0x00, 0x00};
    OpenCrypto_math_sub(number1, number2, 2, result1);
    OpenCrypto_math_sub(number1, number3, 2, result2);
    OpenCrypto_math_sub(number1, number4, 2, result3);

    test_result test_result = TEST_PASSED;
    test_result &= expect_byte_to_be(result1[0], 0x2D);
    test_result &= expect_byte_to_be(result1[1], 0xA8);
    test_result &= expect_byte_to_be(result2[0], 0xB7);
    test_result &= expect_byte_to_be(result2[1], 0xA9);
    test_result &= expect_byte_to_be(result3[0], 0x00);
    test_result &= expect_byte_to_be(result3[1], 0x00);
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

test_result test_math_mul() {
    const byte number[] = {0x00, 0x80}; // This is 128
    const byte factor1[] = {0x00, 0x04}; // This is 4
    const byte factor2[] = {0x00, 0x01}; // This is 1
    const byte factor3[] = {0x00, 0x00}; // This is 0

    // Calculate 128 * 4
    byte result1[] = {0x00, 0x00};
    byte result2[] = {0x00, 0x00};
    byte result3[] = {0x00, 0x00};
    OpenCrypto_math_mul(number, factor1, 2, result1);
    OpenCrypto_math_mul(number, factor2, 2, result2);
    OpenCrypto_math_mul(number, factor3, 2, result3);

    // Test if the result is 512
    test_result test_result = TEST_PASSED;
    test_result &= expect_byte_to_be(result1[0], 0x02); 
    test_result &= expect_byte_to_be(result1[1], 0x00);
    test_result &= expect_byte_to_be(result2[0], 0x00); 
    test_result &= expect_byte_to_be(result2[1], 0x80);
    test_result &= expect_byte_to_be(result3[0], 0x00); 
    test_result &= expect_byte_to_be(result3[1], 0x00);
    return test_result;
}

test_result test_math_div() {
    const byte number1[] = {0x00, 0xfe};
    const byte number2[] = {0x00, 0x02};
    const byte number3[] = {0x00, 0x00};

    byte result1[] = {0x00, 0x00};
    byte result2[] = {0x00, 0x00};
    OpenCrypto_math_div(number1, number2, 2, result1);
    OpenCrypto_math_div(number3, number2, 2, result2);

    test_result test_result = TEST_PASSED;
    test_result &= expect_byte_to_be(result1[0], 0x00);
    test_result &= expect_byte_to_be(result1[1], 0x7f);
    test_result &= expect_byte_to_be(result2[0], 0x00);
    test_result &= expect_byte_to_be(result2[1], 0x00);
    return test_result;
}

test_result test_math_mod() {
    const byte dividend1[] = {0x00, 0xff}; // This is 255
    const byte dividend2[] = {0x00, 0x00}; // This is 0
    const byte divisor[]  = {0x00, 0x04}; // This is 4

    byte result1[] = {0x00, 0x00};
    byte result2[] = {0x00, 0x00};
    OpenCrypto_math_mod(dividend1, divisor, 2, result1);
    OpenCrypto_math_mod(dividend2, divisor, 2, result2);

    test_result test_result = TEST_PASSED;
    test_result &= expect_byte_to_be(result1[0], 0x00);
    test_result &= expect_byte_to_be(result1[1], 0x03);
    test_result &= expect_byte_to_be(result2[0], 0x00);
    test_result &= expect_byte_to_be(result2[1], 0x00);
    return test_result;
}

test_result test_math_eq() {
    const byte number1[] = {0x0e, 0xf3};
    const byte number2[] = {0x0e, 0xf3};
    const byte number3[] = {0x0e, 0xf2};

    test_result test_result = TEST_PASSED;
    test_result &= expect_int_to_be(OpenCrypto_math_eq(number1, number2, 2), 1);
    test_result &= expect_int_to_be(OpenCrypto_math_eq(number1, number3, 2), 0);
    return test_result;
}

test_result test_math_neq() {
    const byte number1[] = {0x0e, 0xf3};
    const byte number2[] = {0x0e, 0xf3};
    const byte number3[] = {0x0e, 0xf2};

    test_result test_result = TEST_PASSED;
    test_result &= expect_int_to_be(OpenCrypto_math_neq(number1, number2, 2), 0);
    test_result &= expect_int_to_be(OpenCrypto_math_neq(number1, number3, 2), 1);
    return test_result;
}

test_result test_math_pow() {
    const byte base[]     = {0x00, 0x03}; // This is 3
    const byte exponent[] = {0x00, 0x03}; // This is 3
    const byte p[]        = {0x00, 0x19}; // This is 25

    // Calculate 3^3 mod 25
    byte result[] = {0x00, 0x00};
    OpenCrypto_math_pow(base, exponent, p, 2, result);

    // Test if result is 3^3 mod 25 = 27 mod 25 = 2
    test_result test_result = TEST_PASSED;
    test_result &= expect_byte_to_be(result[0], 0x00);
    test_result &= expect_byte_to_be(result[1], 0x02);
    return test_result;
}

int main() {
    unit_test tests[] = {
        TEST(test_math_xor),
        TEST(test_math_inv),
        TEST(test_math_add),
        TEST(test_math_sub),
        TEST(test_math_less_than),
        TEST(test_math_mul),
        TEST(test_math_div),
        TEST(test_math_mod),
        TEST(test_math_eq),
        TEST(test_math_neq),
        TEST(test_math_pow)
    };

    run_tests(tests, ARRAY_LENGTH(tests));

    return 0;
}
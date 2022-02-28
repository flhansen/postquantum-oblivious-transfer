#include <stdint.h>
#include <stdio.h>

#include "test.h"
#include "../src/type.h"
#include "../src/utils.h"
#include "../src/math.h"

test_result test_math_init() {
    bignum_t number = oc_math_init();

    test_result tr = TEST_PASSED;
    tr &= number->num_bytes == 1;
    tr &= number->data[0] == 0x00;

    oc_math_clear(number);
    return tr;
}

test_result test_math_initb() {
    unsigned int number_bytes = 100;
    bignum_t num = oc_math_initb(number_bytes);

    test_result tr = TEST_PASSED;
    tr &= num->data != NULL;
    tr &= num->num_bytes == number_bytes;
    oc_math_clear(num);
    return tr;
}

test_result test_math_realloc() {
    bignum_t number = oc_math_init();
    oc_math_assign(number, 256);
    oc_math_realloc(number, 3);

    test_result tr = TEST_PASSED;
    tr &= number->num_bytes == 3;
    tr &= number->data[0] == 0x00;
    tr &= number->data[1] == 0x01;
    tr &= number->data[2] == 0x00;

    oc_math_clear(number);
    return tr;
}

test_result test_math_cpy() {
    bignum_t number = oc_math_init();
    bignum_t copy = oc_math_init();

    oc_math_assign(number, 0xB12000);
    oc_math_cpy(copy, number);

    test_result tr = TEST_PASSED;
    tr &= number->data != copy->data;
    
    for (int i = 0; i < number->num_bytes; i++)
        tr &= number->data[i] == copy->data[i];

    oc_math_clear(number);
    oc_math_clear(copy);
    return tr;
}

test_result test_math_minify() {
    bignum_t number = oc_math_init();

    oc_math_assign(number, 0xFF);
    oc_math_minify(number);

    test_result tr = TEST_PASSED;
    tr &= number->num_bytes == 1;

    oc_math_clear(number);
    return tr;
}

test_result test_math_assign() {
    bignum_t num = oc_math_init();
    oc_math_assign(num, 643);

    test_result tr = TEST_PASSED;

    for (int i = 2; i < num->num_bytes; i++)
        tr &= num->data[i] == 0x00;

    tr &= num->data[1] == 0x02;
    tr &= num->data[0] == 0x83;
    oc_math_clear(num);

    return tr;
}

test_result test_math_add() {
    bignum_t number1 = oc_math_init();
    bignum_t number2 = oc_math_init();
    bignum_t number3 = oc_math_init();

    oc_math_assign(number1, 632);
    oc_math_assign(number2, 321);
    oc_math_add(number3, number1, number2);

    test_result tr = TEST_PASSED;
    tr &= number3->num_bytes == sizeof(uint64_t);

    for (int i = 2; i < number3->num_bytes; i++)
        tr &= number3->data[i] == 0x00;

    tr &= number3->data[1] == 0x03;
    tr &= number3->data[0] == 0xB9;

    oc_math_clear(number1);
    oc_math_clear(number2);
    oc_math_clear(number3);
    return tr;
}

test_result test_math_sub() {
    bignum_t number1 = oc_math_init();
    bignum_t number2 = oc_math_init();
    bignum_t number3 = oc_math_init();

    oc_math_assign(number1, 0x0F00);
    oc_math_assign(number2, 0x0E80);
    oc_math_sub(number3, number1, number2);

    test_result tr = TEST_PASSED;
    tr &= number3->num_bytes == sizeof(uint64_t);

    for (int i = 2; i < number3->num_bytes; i++)
        tr &= number3->data[i] == 0x00;

    tr &= number3->data[1] == 0x00;
    tr &= number3->data[0] == 0x80;

    oc_math_clear(number1);
    oc_math_clear(number2);
    oc_math_clear(number3);
    return tr;
}

test_result test_math_lshift() {
    bignum_t number = oc_math_initb(3);
    number->data[0] = 0x01;
    number->data[1] = 0x02;
    number->data[2] = 0x03;

    oc_math_lshift(number, number, 2);

    test_result tr = TEST_PASSED;
    tr &= number->num_bytes == 5;
    tr &= number->data[0] == 0x00;
    tr &= number->data[1] == 0x00;
    tr &= number->data[2] == 0x01;
    tr &= number->data[3] == 0x02;
    tr &= number->data[4] == 0x03;

    oc_math_clear(number);
    return tr;
}

test_result test_math_add_with_overflow() {
    unsigned int number_bytes = 8;
    bignum_t number1 = oc_math_initb(number_bytes);
    bignum_t number2 = oc_math_initb(number_bytes);
    bignum_t number3 = oc_math_initb(number_bytes);

    oc_math_assign(number1, 0xB165070000000000);
    oc_math_assign(number2, 0x8000000000000000);
    oc_math_add(number3, number1, number2);

    // Test if the the number will be resized when an overflow is produced.
    test_result tr = TEST_PASSED;
    tr &= number3->num_bytes == 9;
    tr &= number3->data[8] == 0x01;
    tr &= number3->data[7] == 0x31;
    tr &= number3->data[6] == 0x65;
    tr &= number3->data[5] == 0x07;
    tr &= number3->data[4] == 0x00;
    tr &= number3->data[3] == 0x00;
    tr &= number3->data[2] == 0x00;
    tr &= number3->data[1] == 0x00;
    tr &= number3->data[0] == 0x00;

    oc_math_clear(number1);
    oc_math_clear(number2);
    oc_math_clear(number3);
    return tr;
}

test_result test_math_mul_with_overflow() {
    unsigned char mul_result;
    unsigned char overflow = oc_math_mul_overflow(&mul_result, 24, 15);

    test_result tr = TEST_PASSED;
    tr &= mul_result == 0x68;
    tr &= overflow == 0x01;
    return tr;
}

test_result test_math_mul() {
    bignum_t number1 = oc_math_init();
    bignum_t number2 = oc_math_init();
    bignum_t number3 = oc_math_init();

    oc_math_assign(number1, 0xB16507);
    oc_math_assign(number2, 0x800000);
    oc_math_mul(number3, number1, number2);

    test_result tr = TEST_PASSED;
    tr &= number3->num_bytes == 6;
    tr &= number3->data[5] == 0x58;
    tr &= number3->data[4] == 0xB2;
    tr &= number3->data[3] == 0x83;
    tr &= number3->data[2] == 0x80;
    tr &= number3->data[1] == 0x00;
    tr &= number3->data[0] == 0x00;

    oc_math_clear(number1);
    oc_math_clear(number2);
    oc_math_clear(number3);
    return tr;
}

int main() {
    unit_test tests[] = {
        TEST(test_math_init),
        TEST(test_math_initb),
        TEST(test_math_realloc),
        TEST(test_math_cpy),
        TEST(test_math_minify),
        TEST(test_math_assign),
        TEST(test_math_add_with_overflow),
        TEST(test_math_mul_with_overflow),
        TEST(test_math_add),
        TEST(test_math_sub),
        TEST(test_math_lshift),
        TEST(test_math_mul)
    };

    run_tests(tests, ARRAY_LENGTH(tests));
}

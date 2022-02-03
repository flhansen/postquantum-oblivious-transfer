#include "test.h"

test_result expect_byte_to_be(char value, char expectation) {
    return value == expectation;
}

test_result expect_int_to_be(int value, int expectation) {
    return value == expectation;
}

test_result expect_char_not_to_be(char value, char expectation) {
    return value != expectation;
}

test_result expect_int_not_to_be(int value, int expectation) {
    return value != expectation;
}
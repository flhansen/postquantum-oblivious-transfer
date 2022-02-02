#include "test.h"

test_result expect_to_be(char value, char expectation) {
    return value == expectation;
}

test_result expect_to_be(int value, int expectation) {
    return value == expectation;
}

test_result expect_not_to_be(char value, char expectation) {
    return value != expectation;
}

test_result expect_not_to_be(int value, int expectation) {
    return value != expectation;
}
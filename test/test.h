#ifndef TEST_H
#define TEST_H

#include <assert.h>
#include <stdio.h>

typedef int test_result;
typedef test_result (*unit_test)();

#define TEST_ERROR 0
#define TEST_PASSED 1

#define EXPECT_TO_BE_EQUAL(value, expected) (assert(value == expected))
#define EXPECT_NOT_TO_BE_EQUAL(value, expected) (assert(value != expected))

#define TEST_RUN(tests, number_tests) \
    int number_passed = 0; \
    for (int i = 0; i < number_tests; i++) { \
        test_result result = (*tests[i])(); \
        \
        if (result == TEST_PASSED) \
            number_passed++; \
    } \
    printf("%d/%d passed tests, %d failed tests\n", number_passed, number_tests, number_tests - number_passed)

test_result expect_byte_to_be(char value, char expectation);
test_result expect_int_to_be(int value, int expectation);

test_result expect_byte_not_to_be(char value, char expectation);
test_result expect_int_not_to_be(int value, int expectation);

#endif
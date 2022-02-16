#ifndef TEST_H
#define TEST_H

#include <assert.h>
#include <stdio.h>

typedef int test_result;

struct _unit_test {
    const char* test_name;
    test_result (*test)();
};

typedef struct _unit_test unit_test;

#define TEST(x) {#x, x}
#define TEST_ERROR 0
#define TEST_PASSED 1

void run_tests(unit_test* tests, unsigned int number_tests);
test_result expect_byte_to_be(char value, char expectation);
test_result expect_int_to_be(int value, int expectation);
test_result expect_byte_not_to_be(char value, char expectation);
test_result expect_int_not_to_be(int value, int expectation);

#endif
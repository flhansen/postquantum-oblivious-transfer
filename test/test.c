#include <stdio.h>

#include "test.h"

void run_tests(unit_test* tests, unsigned int number_tests) {
    int number_passed = 0;
    for (int i = 0; i < number_tests; i++) {
        test_result result = tests[i].test();

        if (result == TEST_PASSED) {
            printf("\033[92m%s passed\033[0m\n", tests[i].test_name);
            number_passed++;
        }
        else {
            printf("\033[91m%s failed\033[0m\n", tests[i].test_name);
        }
    }

    printf("----------------------\n");
    printf("%d/%d passed tests, %d failed tests\n", number_passed, number_tests, number_tests - number_passed);
}

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
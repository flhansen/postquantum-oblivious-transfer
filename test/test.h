#ifndef TEST_H
#define TEST_H

#include <assert.h>

#define TEST_RUN(tests, number_tests) \
    for (int i = 0; i < number_tests; i++) { \
        (*tests[i])(); \
    }

#define EXPECT_TO_BE_EQUAL(value, expected) (assert(value == expected))
#define EXPECT_NOT_TO_BE_EQUAL(value, expected) (assert(value != expected))

#endif
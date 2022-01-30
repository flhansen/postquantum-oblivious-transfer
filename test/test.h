#ifndef TEST_H
#define TEST_H

#include <assert.h>

#define TEST_RUN(tests, number_tests) \
    for (int i = 0; i < number_tests; i++) { \
        (*tests[i])(); \
    }

#define ASSERT_EQUALS(value, expected) (assert(value == expected))

#endif
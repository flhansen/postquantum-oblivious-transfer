#include <stdio.h>
#include <stdlib.h>

#include "test.h"
#include "../src/type.h"
#include "../src/utils.h"
#include "../src/math.h"

void test_math_xor() {
    const byte* message = "This is a test string!";
    const byte* key = "th1s1s4t3stk3y";

    byte result[23];
    OpenCrypto_math_xor(message, key, 14, result);
    result[22] = '\0';

    printf("%s\n", message);
    printf("%s\n", key);
    printf("%s\n", result);

    for (int i = 0; i < 23; i++) {
        printf("%d, %c\n", result[i], result[i]);
    }
}

int main() {
    void (*tests[])() = { test_math_xor };
    TEST_RUN(tests, 1);

    return 0;
}
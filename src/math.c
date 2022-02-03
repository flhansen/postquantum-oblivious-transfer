#include <stdlib.h>

#include "utils.h"
#include "math.h"

int OpenCrypto_math_gcd(int a, int b) {
    int remainer;

    while (b != 0) {
        remainer = a % b;
        a = b;
        b = remainer;
    }

    return a;
}

int OpenCrypto_math_randnrange(int from, int to) {
    return rand() % (to - from + 1) + from;
}

int OpenCrypto_math_fast_power(int x, unsigned int y, int p) {
    // Initialize the result
    int res = 1;

    // Make sure x is modulus p
    x = x % p;

    while (y > 0) {
        // If y is odd, multiply x with the result
        if (y % 2 != 0)
            res = (res * x) % p;

        // Now y must be even
        y = y / 2;
        x = (x*x) % p;
    }

    return res;
}

int OpenCrypto_math_rabin_miller(int candidate, int number_test_iterations) {
    if (candidate == 2 || candidate == 3)
        return OPENCRYPTO_TRUE;

    if (candidate <= 1 || candidate % 2 == 0)
        return OPENCRYPTO_FALSE;

    int s = 0;
    int r = candidate - 1;

    while (r & 1 == 0) {
        s++;
        r /= 2;
    }

    for (int i = 0; i < number_test_iterations; i++) {
        int a = OpenCrypto_math_randnrange(2, candidate - 1);
        int x = OpenCrypto_math_fast_power(a, r, candidate);

        if (x != 1 && x != candidate - 1) {
            int j = 1;

            while (j < s && x != candidate - 1) {
                x = OpenCrypto_math_fast_power(x, 2, candidate);

                if (x == 1)
                    return OPENCRYPTO_FALSE;

                j++;
            }

            if (x != candidate - 1)
                return OPENCRYPTO_FALSE;
        }
    }

    return OPENCRYPTO_TRUE;
}

int OpenCrypto_math_random_prime() {
    int candidate;

    do {
        // Generate random odd integer and test if its prime
        candidate = rand() | 1;
    } while (OpenCrypto_math_rabin_miller(candidate, 128) != OPENCRYPTO_TRUE);

    return candidate;
}

void OpenCrypto_math_xor(const byte* message, const byte* key, unsigned int key_size, byte* out_result) {
    int i = 0;

    while (message[i] != '\0') {
        out_result[i] = message[i] ^ key[i % key_size];
        i++;
    }
}

void OpenCrypto_math_add(const byte* number1, const byte* number2, unsigned int size, byte* out_result) {
    for (unsigned int i = size, overflow = 0; i > 0; i--) {
        unsigned short buffer = number1[i] + number2[i] + overflow;
        out_result[i] = (byte)buffer;
        overflow = buffer >> 8;
    }
}

void OpenCrypto_math_div(const byte* dividend, const byte* divisor, byte* out_result) {
    if (dividend == NULL || divisor == NULL)
        return;

    
}
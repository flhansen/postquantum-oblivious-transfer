#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void OpenCrypto_math_inv(const byte* number, unsigned int size, byte* out_result) {
    for (unsigned int i = 0; i < size; i++)
        out_result[i] = ~number[i];
}

void OpenCrypto_math_add(const byte* number1, const byte* number2, unsigned int size, byte* out_result) {
    byte overflow = 0;

    for (int i = size-1; i >= 0; i--) {
        // We use a buffer, which has more capacity of bits so we can track the overflow
        unsigned short buffer = number1[i] + number2[i] + overflow;
        out_result[i] = (byte)buffer;
        overflow = buffer >> 8;
    }

    if (overflow > 0)
        out_result[size-1] += overflow;
}

void OpenCrypto_math_sub(const byte* number1, const byte* number2, unsigned int size, byte* out_result) {
    if (OpenCrypto_math_eq(number1, number2, size)) {
        memset(out_result, 0, size);
    } else {
        OpenCrypto_math_inv(number2, size, out_result);
        OpenCrypto_math_add(number1, out_result, size, out_result);
    }
}

void OpenCrypto_math_mul(const byte* number1, const byte* number2, unsigned int size, byte* out_result) {
    // Turn every bit of out_result to zero
    byte* mul_result = malloc(size);
    memset(mul_result, 0, size);

    // Allocate memory for the counter n
    byte* n = malloc(size);
    memset(n, 0, size);

    while (OpenCrypto_math_less_than(n, number2, size)) {
        // Create a one
        byte* one = malloc(size);
        memset(one, 0, size);
        one[size-1] = 1;

        // Add the number to itself number2 times
        OpenCrypto_math_add(mul_result, number1, size, mul_result);

        // Increment n by one
        OpenCrypto_math_add(n, one, size, n);
        free(one);
    }

    memcpy(out_result, mul_result, size);

    free(n);
    free(mul_result);
}

void OpenCrypto_math_div(const byte* dividend, const byte* divisor, unsigned int size, byte* out_result) {
    byte* zero = malloc(size);
    memset(zero, 0, size);
    
    if (OpenCrypto_math_eq(dividend, zero, size)) {
        free(zero);
        memset(out_result, 0, size);
        return;
    }

    free(zero);

    // Initialize the result as one (neutral element of division).
    memset(out_result, 0, size);
    out_result[size-1] = 1;

    int continue_subtracting = 1;

    // Initialize the the buffer as zero (neutral element of addition). The
    // buffer will be used to count how often the divisor fits in the dividend
    // by adding the divisor to it every loop.
    byte* buffer = malloc(2);
    memset(buffer, 0, size);

    do {
        // As already mentioned, the divisor is added to the buffer.
        OpenCrypto_math_add(buffer, divisor, size, buffer);

        // Check if divisor + buffer is less than the divident. If not, we know
        // that we found the result.
        continue_subtracting = OpenCrypto_math_less_than(buffer, dividend, size);

        if (continue_subtracting) {
            // Create a one in our byte representation using heap memory (the
            // amount of bits of the representation format could be very big).
            byte* one = malloc(size);
            memset(one, 0, size);
            one[size-1] = 1;

            // Increment the result, which means, that we are counting, how
            // often the divisor fits in the dividend.
            OpenCrypto_math_add(out_result, one, size, out_result);

            // Free the temporary 'one' memory.
            free(one);
        }

    } while (continue_subtracting);

    // Free the buffer memory.
    free(buffer);
}

void OpenCrypto_math_mod(const byte* dividend, const byte* divisor, unsigned int size, byte* out_result) {
    // Copy the value of dividend, we will subtract divisor from it later.
    memcpy(out_result, dividend, size);
    int continue_subtracting;

    do {
        // Check if the divisor still fits inside the copied dividend remainer
        continue_subtracting = OpenCrypto_math_less_than(divisor, out_result, size);

        if (continue_subtracting) {
            // We need to create a temp array instead of manipulating out_result
            // in one shot. Otherwise out_result would contain inverse bits.
            byte* temp = malloc(size);
            OpenCrypto_math_sub(out_result, divisor, size, temp);
            memcpy(out_result, temp, size);
            free(temp);
        }
    } while(continue_subtracting);
}

void OpenCrypto_math_pow(const byte* base, const byte* exponent, const byte* p, unsigned int size, byte* out_result) {
    byte* x = malloc(size);
    memcpy(x, base, size);

    byte* y = malloc(size);
    memcpy(y, exponent, size);

    // Initialize the result
    memset(out_result, 0, size);
    out_result[size-1] = 1;

    // Make sure x is modulus p
    OpenCrypto_math_mod(x, p, size, x);

    byte* zero = malloc(size);
    memset(zero, 0, size);
    byte* one = malloc(size);
    memset(one, 0, size);
    one[size-1] = 1;
    byte* two = malloc(size);
    memset(two, 0, size);
    two[size-1] = 2;


    while(OpenCrypto_math_less_than(zero, y, size)) {
        // If y is odd, multiply x with the result
        byte* ymod2 = malloc(size);
        OpenCrypto_math_mod(y, two, size, ymod2);


        if (OpenCrypto_math_neq(ymod2, zero, size)) {
            OpenCrypto_math_mul(out_result, x, size, out_result);
            OpenCrypto_math_mod(out_result, p, size, out_result);

            byte* temp = malloc(size);
            OpenCrypto_math_sub(y, one, size, temp);
            memcpy(y, temp, size);
            free(temp);
        }

        // Now y must be even
        OpenCrypto_math_div(y, two, size, y);
        OpenCrypto_math_mul(x, x, size, x);
        OpenCrypto_math_mod(x, p, size, x);

        free(ymod2);
    }

    free(x);
    free(y);
    free(zero);
    free(one);
    free(two);
}

int OpenCrypto_math_less_than(const byte* number1, const byte* number2, unsigned int size) {
    int less = 0;
    int found_msb = 0;

    for (unsigned int i = 0; i < size && !found_msb; i++) {
        found_msb = number1[i] > 0 || number2[i] > 0;
        less = number1[i] < number2[i];
    }

    return less;
}

int OpenCrypto_math_eq(const byte* number1, const byte* number2, unsigned int size) {
    int numbers_are_equal = 1;

    for (unsigned int i = 0; i < size && numbers_are_equal; i++)
        numbers_are_equal = number1[i] == number2[i];

    return numbers_are_equal;
}

int OpenCrypto_math_neq(const byte* number1, const byte* number2, unsigned int size) {
    return !OpenCrypto_math_eq(number1, number2, size);
}
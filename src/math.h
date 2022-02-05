#ifndef OPENCRYPTO_MATH_H
#define OPENCRYPTO_MATH_H

#include "type.h"

int OpenCrypto_math_gcd(int a, int b);
int OpenCrypto_math_randnrange(int from, int to);
int OpenCrypto_math_fast_power(int x, unsigned int y, int p);
int OpenCrypto_math_rabin_miller(int candidate, int number_test_iterations);
int OpenCrypto_math_random_prime();
void OpenCrypto_math_xor(const byte* message, const byte* key, unsigned int key_size, byte* out_result);
void OpenCrypto_math_inv(const byte* number, unsigned int size, byte* out_result);
void OpenCrypto_math_add(const byte* number1, const byte* number2, unsigned int size, byte* out_result);
void OpenCrypto_math_sub(const byte* number1, const byte* number2, unsigned int size, byte* out_result);
void OpenCrypto_math_div(const byte* dividend, const byte* divisor, byte* out_result);

#endif
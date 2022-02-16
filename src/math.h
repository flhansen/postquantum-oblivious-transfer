#ifndef OPENCRYPTO_MATH_H
#define OPENCRYPTO_MATH_H

#include "type.h"

int OpenCrypto_math_gcd(int a, int b);
int OpenCrypto_math_randnrange(int from, int to);
int OpenCrypto_math_fast_power(int x, unsigned int y, int p);
int OpenCrypto_math_rabin_miller(int candidate, int number_test_iterations);
int OpenCrypto_math_random_prime();

#endif
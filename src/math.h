#ifndef OPENCRYPTO_MATH_H
#define OPENCRYPTO_MATH_H

#include <stdint.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

typedef struct {
    unsigned char *data;
    uint64_t num_bytes;
} _ap_num;

typedef _ap_num *bignum_t;

void oc_math_print(bignum_t num);
bignum_t oc_math_init();
bignum_t oc_math_initb(uint64_t number_bytes);
void oc_math_clear(bignum_t num);
void oc_math_realloc(bignum_t num, uint64_t size);
void oc_math_cpy(bignum_t dest, bignum_t src);
void oc_math_minify(bignum_t num);
void oc_math_assign(bignum_t num, uint64_t val);
unsigned char oc_math_add_overflow(unsigned char *out, unsigned char num1, unsigned char num2);
unsigned char oc_math_mul_overflow(unsigned char *out, unsigned char num1, unsigned char num2);
void oc_math_lshift(bignum_t out, bignum_t num, uint64_t n);
void oc_math_add(const bignum_t out, const bignum_t num1, const bignum_t num2);
void oc_math_sub(const bignum_t out, const bignum_t num1, const bignum_t num2);
void oc_math_mul(bignum_t out, const bignum_t num1, const bignum_t num2);

#endif
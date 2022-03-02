#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "math.h"

/**
 * Prints the given number in hex representation.
 */
void oc_math_print(bignum_t num) {
    for (int i = num->num_bytes-1; i >= 0; i--)
        printf("%02X ", num->data[i]);

    printf("\n");
}

/**
 * Initializes an arbitrary large number and return a pointer to the created structure.
 *
 * Parameters:
 *   number_bytes: The number of bytes, which should be allocated for the number
 */
bignum_t oc_math_init() {
    bignum_t out = malloc(sizeof(bignum_t));
    out->data = malloc(1);

    out->num_bytes = 1;
    memset(out->data, 0, out->num_bytes);

    return out;
}

/**
 * Initializes an arbitrary large number and return a pointer to the created structure.
 *
 * Parameters:
 *   number_bytes: The number of bytes, which should be allocated for the number
 */
bignum_t oc_math_initb(uint64_t number_bytes) {
    bignum_t out = malloc(sizeof(bignum_t));
    out->data = malloc(number_bytes);

    out->num_bytes = number_bytes;
    memset(out->data, 0, out->num_bytes);

    return out;
}

/**
 * Reallocates memory for the given number.
 *
 * Parameters:
 *   num: The number, which should be resized
 *   size: The new size of the number in bytes
 */
void oc_math_realloc(bignum_t num, uint64_t size) {
    uint64_t old_size = num->num_bytes;

    num->num_bytes = size;
    num->data = realloc(num->data, size);

    if (old_size < size)
        memset(&num->data[old_size], 0, size - old_size);
}

/**
 * Copies a number.
 *
 * Parameters:
 *   dest: The destination
 *   src: The source
 */
void oc_math_cpy(bignum_t dest, bignum_t src) {
    oc_math_realloc(dest, src->num_bytes);
    dest->num_bytes = src->num_bytes;
    memcpy(dest->data, src->data, src->num_bytes);
}

/**
 * Removes leading zeros from the number.
 *
 * Parameters:
 *   num: The number, which should be minified
 */
void oc_math_minify(bignum_t num) {
    uint64_t num_leading_zeros = 0;
    uint8_t non_zero_found = 0;

    for (int i = num->num_bytes-1; i >= 0 && !non_zero_found; i--) {
        non_zero_found = num->data[i] != 0x00;

        if (!non_zero_found)
            num_leading_zeros++;
    }

    oc_math_realloc(num, num->num_bytes - num_leading_zeros);
}

/**
 * Assigns a value to an arbitrary large number.
 *
 * Parameters:
 *   num: The number, which should be assigned to
 *   val: The value
 */
void oc_math_assign(bignum_t num, uint64_t val) {
    if (num->num_bytes < sizeof(val))
        oc_math_realloc(num, sizeof(val));

    memset(num->data, 0, num->num_bytes);
    int num_effective_bytes = num->num_bytes;
    int num_max_bytes = sizeof(val);

    if (num_effective_bytes > num_max_bytes)
        num_effective_bytes = num_max_bytes;

    for (int i = 0; i < num_effective_bytes; i++)
        num->data[i] = val >> (i * 8);
}

/**
 * This function adds two numbers represented by one byte and returns the overflow.
 *
 * Parameters:
 *   out: The variable, which contains the result of the addition
 *   num1: The first operand
 *   num2: The second operand
 *
 * Returns:
 *   A byte (unsigned char) with the overflow information
 */
unsigned char oc_math_add_overflow(unsigned char *out, unsigned char num1, unsigned char num2) {
    *out = num1 + num2;

    unsigned char left = num1 >> 4;
    unsigned char right = (unsigned char)(num1 << 4) >> 4;
    unsigned char overflow = (((right + num2) >> 4) + left) >> 4;
    return overflow;
}

/**
 * This function multiplies two numbers represented by one byte and returns the overflow.
 *
 * Parameters:
 *   out: The target, which will contain the result of the multiplication
 *   num1: The first operand
 *   num2: The second operand
 *
 * Returns:
 *   A byte (unsigend char) with the overflow information
 */
unsigned char oc_math_mul_overflow(unsigned char *out, unsigned char num1, unsigned char num2) {
    unsigned char s0, s1, s2, s3;

    unsigned char lo1 = ((0x01 << 4) - 1) & num1;
    unsigned char hi1 = num1 >> 4;
    unsigned char lo2 = ((0x01 << 4) - 1) & num2;
    unsigned char hi2 = num2 >> 4;

    unsigned char x = lo1 * lo2;
    s0 = ((0x01 << 4) - 1) & x;

    x = hi1 * lo2 + (x >> 4);
    s1 = ((0x01 << 4) - 1) & x;
    s2 = x >> 4;

    x = s1 + lo1 * hi2;
    s1 = ((0x01 << 4) - 1) & x;

    x = s2 + hi1 * hi2 + (x >> 4);
    s2 = ((0x01 << 4) - 1) & x;
    s3 = x >> 4;

    *out = s1 << 4 | s0;
    return s3 << 4 | s2;
}

/**
 * This function shifts the given number by n places to left.
 *
 * Parameters:
 *   out: The result
 *   num: The number
 *   n: The number of shift operations
 */
void oc_math_lshift(bignum_t out, bignum_t num, uint64_t n) {
    uint64_t new_size = num->num_bytes + n;
    unsigned char *data = malloc(new_size);
    memcpy(data, num->data, num->num_bytes);

    for (int i = 0; i < n; i++) {
        for (int j = new_size-1; j >= 1; j--) {
            data[j] = data[j-1];
        }

        data[0] = 0x00;
    }

    free(out->data);
    out->data = data;
    out->num_bytes = new_size;
}

/**
 * This function adds two arbitrary large numbers and store the result into an automatically growing number structure.
 *
 * Parameters:
 *   out: The target variable, which contains the result of the addition
 *   num1: The first operand
 *   num2: The second operand
 */
void oc_math_add(bignum_t out, const bignum_t num1, const bignum_t num2) {
    // Copy the values and make sure, both numbers have the same size.
    uint64_t size = MAX(num1->num_bytes, num2->num_bytes);
    unsigned char *data1 = malloc(size);
    unsigned char *data2 = malloc(size);
    memset(data1, 0, size);
    memset(data2, 0, size);
    memcpy(data1, num1->data, num1->num_bytes);
    memcpy(data2, num2->data, num2->num_bytes);

    if (out->num_bytes < size)
        oc_math_realloc(out, size);

    memset(out->data, 0, out->num_bytes);
    unsigned char overflow = 0;

    for (int i = 0; i < num1->num_bytes; i++) {
        unsigned char n2;

        // Detect overflow while adding the previous overflow to the second operand.
        overflow = oc_math_add_overflow(&n2, data2[i], overflow);

        // Detect overflow and sum it up for the next byte being processed.
        overflow += oc_math_add_overflow(&out->data[i], data1[i], n2);
    }

    // If there is still an overflow remaining, allocate more space for the
    // number and place the overflow into the new allocated byte.
    if (overflow > 0) {
        oc_math_realloc(out, out->num_bytes + 1);
        out->data[out->num_bytes-1] = overflow;
    }

    // Free the allocated memory.
    free(data1);
    free(data2);
}

/**
 * This function subtracts one arbitrary large number from another.
 *
 * Parameters:
 *   out: The target variable, which contains the result of the subtraction
 *   num1: The first operand
 *   num2: The second operand
 */
void oc_math_sub(const bignum_t out, const bignum_t num1, const bignum_t num2) {
    // Copy the values and make sure, both numbers have the same size.
    uint64_t size = MAX(num1->num_bytes, num2->num_bytes);
    unsigned char *data1 = malloc(size);
    unsigned char *data2 = malloc(size);
    memcpy(&data1[size-num1->num_bytes], num1->data, num1->num_bytes);
    memcpy(&data2[size-num2->num_bytes], num2->data, num2->num_bytes);

    for (int i = 0; i < size; i++)
        data2[i] = ~data2[i];

    if (out->num_bytes < size)
        oc_math_realloc(out, size);

    memset(out->data, 0, out->num_bytes);
    unsigned char overflow = 0;

    for (int i = 0; i < num1->num_bytes; i++) {
        unsigned char n2;

        // Detect overflow while adding the previous overflow to the second operand.
        overflow = oc_math_add_overflow(&n2, data2[i], overflow);

        // Detect overflow and sum it up for the next byte being processed.
        overflow += oc_math_add_overflow(&out->data[i], data1[i], n2);
    }

    // If there is still an overflow remaining, allocate more space for the
    // number and place the overflow into the new allocated byte.
    if (overflow > 0) {
        bignum_t carry = oc_math_initb(out->num_bytes);
        carry->data[0] = 0x01;
        oc_math_add(out, out, carry);
    }

    // Free the allocated memory.
    free(data1);
    free(data2);
}

/**
 *  This function multiplies two arbitrary large numbers.
 *
 *  Parameters:
 *    out: The target, which will contain the result of the operation
 *    num1: The first operand
 *    num2: The second operand
 */
void oc_math_mul(bignum_t out, bignum_t num1, bignum_t num2) {
    memset(out->data, 0, out->num_bytes);

    for (int i = num2->num_bytes-1; i >= 0; i--) {
        for (int j = num1->num_bytes-1; j >= 0; j--) {
            unsigned char partial;
            unsigned char overflow = oc_math_mul_overflow(&partial, num1->data[j], num2->data[i]);

            bignum_t partial_result = oc_math_initb(2);
            partial_result->data[0] = partial;
            partial_result->data[1] = overflow;

            oc_math_lshift(partial_result, partial_result, j+i);
            oc_math_add(out, out, partial_result);
        }
    }

    oc_math_minify(out);
}

/**
 * Frees all memory allocated for the given arbitrary large number.
 *
 * Parameters:
 *   num: The number, which should be freed
 */
void oc_math_clear(bignum_t num) {
    memset(num->data, 0, num->num_bytes);

    free(num->data);
    num->data = NULL;
    num->num_bytes = 0;

    free(num);
}

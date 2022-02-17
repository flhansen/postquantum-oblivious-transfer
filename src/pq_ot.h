/********************************************************************************
 * Post-Quantum Oblivious Transfer (PQOT): Oblivious Transfer using supersingular
 * isogenies.
 * 
 * Abtstract: API header file for PQOT
********************************************************************************/

#ifndef OPENCRYPTO_PQ_OT_H
#define OPENCRYPTO_PQ_OT_H

#include <gmp.h>

#include "type.h"

// Key size definitions
// -----------------------------------------------------------------------------
#define PQOT_P434 434
#define PQOT_P503 503
#define PQOT_P610 610
#define PQOT_P751 751

// Set the default keysize to 434 bits, if not defined.
#ifndef PQOT_KEYSIZE
    #define PQOT_KEYSIZE PQOT_P434
#endif

// Structures used in PQOT
// -----------------------------------------------------------------------------
struct _OpenCrypto_PQOT_public_parameters {
    unsigned int number_bits;
    mpz_t generator;
    mpz_t modulus;
    gmp_randstate_t rand_state;
};

struct _OpenCrypto_PQOT_sender_keys {

};

struct _OpenCrypto_PQOT_receiver_keys {

};

typedef struct _OpenCrypto_PQOT_public_parameters PQOT_public_parameters;
typedef struct _OpenCrypto_PQOT_sender_keys PQOT_sender_keys;
typedef struct _OpenCrypto_PQOT_receiver_keys PQOT_receiver_keys;


// PQOT API implementation
// -----------------------------------------------------------------------------
void OpenCrypto_PQOT_init_public_params(PQOT_public_parameters *pp);
void OpenCrypto_PQOT_init_sender(PQOT_sender_keys *out_sender, PQOT_public_parameters *pp);
void OpenCrypto_PQOT_init_receiver(PQOT_receiver_keys *out_receiver, PQOT_public_parameters* pp, int c, const mpz_t public_key_sender);
void OpenCrypto_PQOT_clear_public_params(PQOT_public_parameters *pp);
void OpenCrypto_PQOT_clear_sender(PQOT_sender_keys *sender);
void OpenCrypto_PQOT_clear_receiver(PQOT_receiver_keys *receiver);
void OpenCrypto_PQOT_encrypt(byte out_ciphers[][255], PQOT_public_parameters *pp, byte messages[][255], unsigned int number_messages, PQOT_sender_keys *sender, const mpz_t public_key_receiver);
void OpenCrypto_PQOT_decrypt(byte out_messages[][255], PQOT_public_parameters *pp, byte ciphers[][255], unsigned int number_ciphers, PQOT_receiver_keys *reveiver);

#endif
#ifndef PARAMS_H
#define PARAMS_H

#include <stdint.h>

// Approved parameter sets (ML-KEM)
#define ML_KEM_N 256
#define ML_KEM_Q 3329
#define ML_KEM_SSK 32	// shared secret key (bytes)

#define ML_KEM_512_K  2
#define ML_KEM_768_K  3
#define ML_KEM_1024_K 4
#define ML_KEM_K_MAX 4

#define ML_KEM_512_ETA1  3
#define ML_KEM_768_ETA1  2
#define ML_KEM_1024_ETA1 2

#define ML_KEM_512_ETA2  2 
#define ML_KEM_768_ETA2  2
#define ML_KEM_1024_ETA2 2

#define ML_KEM_512_DU  10
#define ML_KEM_768_DU  10
#define ML_KEM_1024_DU 11

#define ML_KEM_512_DV  4
#define ML_KEM_768_DV  4
#define ML_KEM_1024_DV 5

// required bit generator strength (bits)
#define ML_KEM_512_RBG  128
#define ML_KEM_768_RBG  192
#define ML_KEM_1024_RBG 256


// Sizes of keys and ciphertexts (bytes)
#define ML_KEM_512_EK  800
#define ML_KEM_768_EK  1184
#define ML_KEM_1024_EK 1568

#define ML_KEM_512_DK  1632
#define ML_KEM_768_DK  2400
#define ML_KEM_1024_DK 3168

#define ML_KEM_512_CTEXT  768
#define ML_KEM_768_CTEXT  1088
#define ML_KEM_1024_CTEXT 1568

//groupings of parameter sets for parameter passing

typedef struct {
    uint16_t k;
    uint16_t eta1;
    uint16_t eta2;
    uint16_t du;
    uint16_t dv;
    uint16_t rbg;
    uint16_t ek_size;
    uint16_t dk_size;
    uint16_t ctext_size;
} ml_kem_params;

//vec[i] contains i polynomials
typedef int16_t poly[ML_KEM_N];    //coefficient array of len N = 256
typedef poly vec[ML_KEM_K_MAX];
typedef poly matrix[ML_KEM_K_MAX][ML_KEM_K_MAX];

static const ml_kem_params ML_KEM_512 = {
    .k = ML_KEM_512_K,
    .eta1 = ML_KEM_512_ETA1,
    .eta2 = ML_KEM_512_ETA2,
    .du = ML_KEM_512_DU,
    .dv = ML_KEM_512_DV,
    .rbg = ML_KEM_512_RBG,
    .ek_size = ML_KEM_512_EK,
    .dk_size = ML_KEM_512_DK,
    .ctext_size = ML_KEM_512_CTEXT
};

static const ml_kem_params ML_KEM_768 = {
    .k = ML_KEM_768_K,
    .eta1 = ML_KEM_768_ETA1,
    .eta2 = ML_KEM_768_ETA2,
    .du = ML_KEM_768_DU,
    .dv = ML_KEM_768_DV,
    .rbg = ML_KEM_768_RBG,
    .ek_size = ML_KEM_768_EK,
    .dk_size = ML_KEM_768_DK,
    .ctext_size = ML_KEM_768_CTEXT
};

static const ml_kem_params ML_KEM_1024 = {
    .k = ML_KEM_1024_K,
    .eta1 = ML_KEM_1024_ETA1,
    .eta2 = ML_KEM_1024_ETA2,
    .du = ML_KEM_1024_DU,
    .dv = ML_KEM_1024_DV,
    .rbg = ML_KEM_1024_RBG,
    .ek_size = ML_KEM_1024_EK,
    .dk_size = ML_KEM_1024_DK,
    .ctext_size = ML_KEM_1024_CTEXT
};

#endif

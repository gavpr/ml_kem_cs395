#include "ntt.h"
#include <stdint.h>
#include <assert.h>

#define ROOT_OF_UNITY 17;

//precomputed values for zeta operations of ntt 
//each value represents 17^(bit_rev_7(i)) mod ML_KEM_Q
//where i = 0, 1, ... , 127 and ML_KEM_Q = 3329
//used for NTT and NTT^-1 transformations
const int16_t zeta_values[128] = 
    { 1, 1729, 2580, 3289, 2642, 630, 1897, 848,
    1062, 1919, 193, 797, 2786, 3260, 569, 1746,
    296, 2447, 1339, 1476, 3046, 56, 2240, 1333,
    1426, 2094, 535, 2882, 2393, 2879, 1974, 821,
    289, 331, 3253, 1756, 1197, 2304, 2277, 2055,
    650, 1977, 2513, 632, 2865, 33, 1320, 1915,
    2319, 1435, 807, 452, 1438, 2868, 1534, 2402,
    2647, 2617, 1481, 648, 2474, 3110, 1227, 910,
    17, 2761, 583, 2649, 1637, 723, 2288, 1100,
    1409, 2662, 3281, 233, 756, 2156, 3015, 3050,
    1703, 1651, 2789, 1789, 1847, 952, 1461, 2687,
    939, 2308, 2437, 2388, 733, 2337, 268, 641,
    1584, 2298, 2037, 3220, 375, 2549, 2090, 1645,
    1063, 319, 2773, 757, 2099, 561, 2466, 2594,
    2804, 1092, 403, 1026, 1143, 2150, 2775, 886,
    1722, 1212, 1874, 1029, 2110, 2935, 885, 2154 };

//used within multiply_ntts function
//precomputed values for zeta operations
//each value represents 17^(2bit_rev_7(i) + 1) mod ML_KEM_Q
//where i = 0, ... , 127 and ML_KEM_Q = 3329
const int16_t zeta_mul[128] = 
    { 17, -17, 2761, -2761, 583, -583, 2649, -2649,
    1637, -1637, 723, -723, 2288, -2288, 1100, -1100,
    1409, -1409, 2662, -2662, 3281, -3281, 233, -233,
    756, -756, 2156, -2156, 3015, -3015, 3050, -3050,
    1703, -1703, 1651, -1651, 2789, -2789, 1789, -1789,
    1847, -1847, 952, -952, 1461, -1461, 2687, -2687,
    939, -939, 2308, -2308, 2437, -2437, 2388, -2388,
    733, -733, 2337, -2337, 268, -268, 641, -641,
    1584, -1584, 2298, -2298, 2037, -2037, 3220, -3220,
    375, -375, 2549, -2549, 2090, -2090, 1645, -1645,
    1063, -1063, 319, -319, 2773, -2773, 757, -757,
    2099, -2099, 561, -561, 2466, -2466, 2594, -2594,
    2804, -2804, 1092, -1092, 403, -403, 1026, -1026,
    1143, -1143, 2150, -2150, 2775, -2775, 886, -886,
    1722, -1722, 1212, -1212, 1874, -1874, 1029, -1029,
    2110, -2110, 2935, -2935, 885, -885, 2154, -2154 };

//take polynomial f and transform it into its NTT translation
void ntt(int16_t f[ML_KEM_N]){
    uint32_t i = 1;
    int32_t zeta, t;

    for(int32_t len = 128; len >= 2; len = len/2) {
        for(int32_t start = 0; start < 256; start = start + (2 * len)) {
            zeta = zeta_values[i++];

            for(int32_t j = start; j < start + len; j++) {
                t = (zeta * f[j + len]) % ML_KEM_Q;
                f[j + len] = (f[j] - t + ML_KEM_Q) % ML_KEM_Q;
                f[j] = (f[j] + t) % ML_KEM_Q;
            }
        }
    }
}

//inverse of ntt (NTT translation back into regular polynomial)
void ntt_inv(int16_t fhat[ML_KEM_N]){
    uint32_t i = 127;
    int32_t zeta, t;

    for(int32_t len = 2; len <= 128; len = len * 2) {
        for(int32_t start = 0; start < 256; start = start + (2 * len)) {
            zeta = zeta_values[i--];

            for(int32_t j = start; j < start + len; j++) {
                t = fhat[j];
                fhat[j] = (t + fhat[j + len]) % ML_KEM_Q;
                fhat[j + len] = (zeta * (fhat[j + len] - t + ML_KEM_Q)) % ML_KEM_Q;
            }
        }
    }

    for(int32_t k = 0; k < 256; k++) {
        fhat[k] = (fhat[k] * 3303) % ML_KEM_Q;
    }
}

//compute the product of two NTT represented polynomials and return an NTT representation of the product
//input: fhat, ghat
//output: hhat
void multiply_ntts(const int16_t fhat[ML_KEM_N], const int16_t ghat[ML_KEM_N], int16_t hhat[ML_KEM_N]) {
    int16_t c0 = 0, c1 = 0;
    for(int i = 0; i < 128; i++) {
        base_case_multiply(fhat[2*i], fhat[(2*i) + 1], ghat[2*i], ghat[(2*i) + 1], zeta_mul[i], &c0, &c1);
        hhat[2*i] = c0;
        hhat[(2*i) + 1] = c1;
    }
}


//computes the product of two degree-one polynomials using quadratic modulus
//multiply_ntts subroutine
void base_case_multiply(const int16_t a0, const int16_t a1, const int16_t b0, const int16_t b1, const int16_t gamma, int16_t* c0, int16_t* c1) {
    *c0 = (((((int32_t)a0 * b0) + (((int32_t)a1 * b1) % ML_KEM_Q * gamma)) % ML_KEM_Q) + ML_KEM_Q) % ML_KEM_Q; 
    *c1 = (((((int32_t)a0 * b1) + ((int32_t)a1 * b0)) % ML_KEM_Q) + ML_KEM_Q) % ML_KEM_Q;
}


uint8_t bit_rev_7(uint8_t r){
    assert(r <= 127);

    uint8_t result = 0;

    uint8_t current_bit = 0;
    for(int i = 0; i < 7; i++) {
        current_bit = (r >> i) & 1;
        result = result | (current_bit << (6 - i));
    }

    return result;
}
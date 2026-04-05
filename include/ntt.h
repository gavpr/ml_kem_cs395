#ifndef NTT_H
#define NTT_H

#include <stdint.h>
#include "params.h"

//define functions NTT and NTT_inverse

//compute ntt representation fhat in Tq of given polynomial f in Rq
void ntt(int16_t f[ML_KEM_N]);

//compute polynomial f in Rq that corresponds to the given ntt representation fhat in Tq
void ntt_inv(int16_t fhat[ML_KEM_N]);

//compute the product of two NTT represented polynomials and return an NTT representation of the product
void multiply_ntts(const int16_t fhat[ML_KEM_N], const int16_t ghat[ML_KEM_N], int16_t hhat[ML_KEM_N]);

//computes the product of two degree-one polynomials using quadratic modulus
void base_case_multiply(const int16_t a0, const int16_t a1, const int16_t b0, const int16_t b1, const int16_t gamma, int16_t* c0, int16_t* c1);

uint8_t bit_rev_7(uint8_t r);

#endif
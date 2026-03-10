#ifndef NTT_H
#define NTT_H

#include <stdint.h>
#include "params.h"

//define functions NTT and NTT_inverse

//compute ntt representation fhat in Tq of given polynomial f in Rq
void ntt(const uint16_t f[ML_KEM_N], uint16_t fhat[ML_KEM_N]);

//compute polynomial f in Rq that corresponds to the given ntt representation fhat in Tq
void ntt_inv(const uint16_t fhat[ML_KEM_N], uint16_t f[ML_KEM_N]);

uint8_t bit_rev_7(uint8_t r);

#endif
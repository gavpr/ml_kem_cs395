#ifndef SAMPLE_H
#define SAMPLE_H

#include "params.h"
#include <stdint.h>

//Takes a 32-byte seed and two indices as input and output a 
//pseudorandom element of T_q (NTT polynomial)
void sample_ntt(const uint8_t B[], uint16_t a[ML_KEM_N], const uint16_t B_length);

//Takes a seed as input and outputs a pseudorandom sample from the distribution D_n(R_q)
//Byte buffer B size = 64 * eta
void sample_poly_cbd(const uint8_t B[], uint16_t f[ML_KEM_N], const uint8_t eta);

#endif
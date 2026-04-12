#ifndef SAMPLE_H
#define SAMPLE_H

#include "params.h"
#include "fips202.h"
#include <stdint.h>
#include <stddef.h>

//Takes a 32-byte seed and two indices as input and output a 
//pseudorandom element of T_q (NTT polynomial)
void sample_ntt(const uint8_t B[], poly a_hat, const size_t B_length);

//Takes a seed as input and outputs a pseudorandom sample from the distribution D_n(R_q)
//Byte buffer B size = 64 * eta
void sample_poly_cbd(const uint8_t B[], poly f, const uint8_t eta);

#endif
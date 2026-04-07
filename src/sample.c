#include "sample.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>

//void sample_ntt(const uint8_t B[], uint16_t a[ML_KEM_N], const uint16_t B_length);

void sample_poly_cbd(const uint8_t B[], uint16_t f[ML_KEM_N], const uint8_t eta) {
    uint16_t B_size = eta * 64;
    uint8_t b[64 * 8 * eta];
    bytes_to_bits(B, b, B_size);

    uint16_t x;
    uint16_t y;
    for(uint16_t i = 0; i < 256; i++) {
        x = 0;
        y = 0;

        for(uint16_t j = 0; j < eta; j++) {
            x += b[(2*i*eta) + j];
        }

        for(uint16_t j = 0; j < eta; j++) {
            y += b[(2*i*eta) + eta + j];
        }

        f[i] = ((x-y) + ML_KEM_Q) % ML_KEM_Q;
    }
}
#include "sample.h"
#include "util.h"
#include "fips202.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

void sample_ntt(const uint8_t B[], poly a_hat, const size_t B_length) {
    //setup shake128 api
    shake128incctx ctx;
    shake128_inc_init(&ctx);
    shake128_inc_absorb(&ctx, B, B_length);
    shake128_inc_finalize(&ctx);

    uint16_t j = 0;
    uint16_t d1, d2;
    while(j < ML_KEM_N) {
        //void shake128_inc_squeeze(uint8_t *output, size_t outlen, shake128incctx *state);
        uint8_t C[3];
        shake128_inc_squeeze(C, 3, &ctx);
        d1 = C[0] + 256 * (C[1] % 16);
        d2 = (C[1] / 16) + 16 * C[2];
        if(d1 < ML_KEM_Q) {
            a_hat[j] = d1;
            j++;
        }
        if(d2 < ML_KEM_Q && j < 256) {
            a_hat[j] = d2;
            j = j+1;
        }
    }
    
}

void sample_poly_cbd(const uint8_t B[], poly f, const uint8_t eta) {
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
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

void bits_to_bytes(const uint8_t* b, uint8_t* B, int l) {
    //zero out byte buffer
    for(int i = 0; i < l; i++) {
        B[i] = 0;
    }

    for(int i = 0; i < 8*l; i++) {
        B[i/8] += b[i] * (1 << (i % 8));
    }
}

void bytes_to_bits(const uint8_t* B, uint8_t* b, int l) {
    uint8_t* C = (uint8_t*)malloc(l * sizeof(uint8_t));
    memcpy(C, B, l);

    for(int i = 0; i < l; i++) {
        for(int j = 0; j < 8; j++) {
            b[(8*i)+j] = (C[i] % 2);
            C[i] = C[i] / 2;
        }
    }
    free(C);
}

void byte_encode(const poly F, uint8_t* B, uint8_t d) {
    uint16_t m = 0;
    uint16_t a = 0;
    uint8_t b[256*12];
    if(d < 12) {
        m = 1 << d;
    }
    else {
        m = ML_KEM_Q;
    }

    for(int i = 0; i < 256; i++) {
        a = (uint16_t)F[i] % m;
        for(int j = 0; j < d; j++) {
            b[(i*d) + j] = a % 2;
            a = (a - b[(i*d) + j])/2;
        }
    }
    bits_to_bytes(b, B, 32*d); 
}

void byte_decode(const uint8_t* B, poly F, uint8_t d) {
    uint16_t m = 0;
    uint8_t b[32*8*d];
    if(d < 12) {
        m = 1 << d;
    }
    else {
        m = ML_KEM_Q;
    }

    bytes_to_bits(B, b, 32*d);
    for(int i = 0; i < 256; i++) {
        uint16_t res = 0;
        for(int j = 0; j < d; j++) {
            res += (b[(i*d) + j] * (1 << j)) % m;
        }
        F[i] = (int16_t)res;
    }
}


uint16_t compress(const uint16_t x, uint8_t d) {
    if(d > 12) {
        return 1;
    }

    uint16_t mod = (1 << d);
    uint16_t out = ((mod * x) + (ML_KEM_Q / 2)) / ML_KEM_Q;
    out = out % mod;
    return out;
}

uint16_t decompress(const uint16_t y, uint8_t d) {
    if(d > 12) {
        return 1;
    }

    uint16_t mod = (1 << d);
    uint16_t out = ((ML_KEM_Q * y) + (mod / 2)) / mod;
    return out;
}
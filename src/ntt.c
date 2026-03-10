#include "ntt.h"
#include <stdint.h>
#include <assert.h>

void ntt(const uint16_t f[ML_KEM_N], uint16_t fhat[ML_KEM_N]){

}

void ntt_inv(const uint16_t fhat[ML_KEM_N], uint16_t f[ML_KEM_N]){
    
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
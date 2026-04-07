#include "kpke.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>






//uses the decryption key to decrypt a given ciphertext
void k_pke_decrypt(const uint8_t dk[], const uint8_t c[], uint8_t m[], const ml_kem_params *PARAMS) {
    uint8_t *c1 = c; //[0 : 32 * PARAMS->du * PARAMS->k]
    uint8_t *c2 = c + (32 * PARAMS->du * PARAMS->k);    //[32 * PARAMS->du * PARAMS->k : PARAMS->ctext_size]

    vec u_prime[PARAMS->k];    //vector of k polynomials from c
    for(uint16_t i = 0; i < PARAMS->k; i++) {
        uint8_t *current = c1 + (32 * PARAMS->du * i);  //current location of c buffer
        byte_decode(current, &u_prime[i], PARAMS->du);
    }
}


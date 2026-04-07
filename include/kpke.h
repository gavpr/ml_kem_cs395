#ifndef KPKE_H
#define KPKE_H

#include "params.h"
#include <stdint.h>

//uses randomness to generate an encryption key and a corresponding decryption key
//k_pke_keygen()

//uses encryption key to encrypt a plaintext message using randomness r
//k_pke_encrypt()

//uses the decryption key to decrypt a given ciphertext
void k_pke_decrypt(const uint8_t dk[], const uint8_t c[], uint8_t m[], const ml_kem_params *PARAMS);

#endif
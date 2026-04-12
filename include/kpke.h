#ifndef KPKE_H
#define KPKE_H

#include "params.h"
#include <stdint.h>

//uses randomness to generate an encryption key and a corresponding decryption key
//k_pke_keygen()

//uses encryption key to encrypt a plaintext message using randomness r
void k_pke_encrypt(const uint8_t ek[], const uint8_t m[], const uint8_t r[], uint8_t c[], const ml_kem_params *params);

//uses the decryption key to decrypt a given ciphertext
void k_pke_decrypt(const uint8_t dk[], const uint8_t c[], uint8_t m[], const ml_kem_params *params);

#endif
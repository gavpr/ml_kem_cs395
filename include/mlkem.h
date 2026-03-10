#ifndef MLKEM_H
#define MLKEM_H

#include <stdint.h>
#include "params.h"

// ML-KEM (FIPS 203) Key Encapsulation API
// All functions return 0 on success, non-zero otherwise

int ml_kem_512_keygen(uint8_t ek[ML_KEM_512_EK], uint8_t dk[ML_KEM_512_DK]);
int ml_kem_512_encaps(const uint8_t ek[ML_KEM_512_EK], uint8_t k[ML_KEM_SSK], uint8_t c[ML_KEM_512_CTEXT]);
int ml_kem_512_decaps(const uint8_t dk[ML_KEM_512_DK], const uint8_t c[ML_KEM_512_CTEXT], uint8_t k[ML_KEM_SSK]);

int ml_kem_768_keygen(uint8_t ek[ML_KEM_768_EK], uint8_t dk[ML_KEM_768_DK]);
int ml_kem_768_encaps(const uint8_t ek[ML_KEM_768_EK], uint8_t k[ML_KEM_SSK], uint8_t c[ML_KEM_768_CTEXT]);
int ml_kem_768_decaps(const uint8_t dk[ML_KEM_768_DK], const uint8_t c[ML_KEM_768_CTEXT], uint8_t k[ML_KEM_SSK]);

int ml_kem_1024_keygen(uint8_t ek[ML_KEM_1024_EK], uint8_t dk[ML_KEM_1024_DK]);
int ml_kem_1024_encaps(const uint8_t ek[ML_KEM_1024_EK], uint8_t k[ML_KEM_SSK], uint8_t c[ML_KEM_1024_CTEXT]);
int ml_kem_1024_decaps(const uint8_t dk[ML_KEM_1024_DK], const uint8_t c[ML_KEM_1024_CTEXT], uint8_t k[ML_KEM_SSK]);

#endif

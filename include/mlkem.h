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

//These three internal functions are usually hidden but defined here for testing purposes
void ml_kem_keygen_internal(const uint8_t d[], const uint8_t z[], uint8_t ek[], uint8_t dk[], const ml_kem_params *params);
void ml_kem_encaps_internal(const uint8_t ek[], const uint8_t m[], uint8_t ssk[], uint8_t c[], const ml_kem_params *params);
void ml_kem_decaps_internal(const uint8_t dk[], const uint8_t c[], uint8_t ssk[], const ml_kem_params *params);

#endif

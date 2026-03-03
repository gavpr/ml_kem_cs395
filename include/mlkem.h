#ifndef MLKEM_H
#define MLKEM_H

#include "params.h"

// ML-KEM (FIPS 203) Key Encapsulation API
// All functions return 0 on success, non-zero otherwise

int ml_kem_512_keygen(u8 ek[ML_KEM_512_EK], u8 dk[ML_KEM_512_DK]);
int ml_kem_512_encaps(const u8 ek[ML_KEM_512_EK], u8 ssk[ML_KEM_SSK], u8 c[ML_KEM_512_CTEXT]);
int ml_kem_512_decaps(const u8 dk[ML_KEM_512_DK], const u8 c[ML_KEM_512_CTEXT], u8 ssk[ML_KEM_SSK]);

int ml_kem_768_keygen(u8 ek[ML_KEM_768_EK], u8 dk[ML_KEM_768_DK]);
int ml_kem_768_encaps(const u8 ek[ML_KEM_768_EK], u8 ssk[ML_KEM_SSK], u8 c[ML_KEM_768_CTEXT]);
int ml_kem_768_decaps(const u8 dk[ML_KEM_768_DK], const u8 c[ML_KEM_768_CTEXT], u8 ssk[ML_KEM_SSK]);

int ml_kem_1024_keygen(u8 ek[ML_KEM_1024_EK], u8 dk[ML_KEM_1024_DK]);
int ml_kem_1024_encaps(const u8 ek[ML_KEM_1024_EK], u8 ssk[ML_KEM_SSK], u8 c[ML_KEM_1024_CTEXT]);
int ml_kem_1024_decaps(const u8 dk[ML_KEM_1024_DK], const u8 c[ML_KEM_1024_CTEXT], u8 ssk[ML_KEM_SSK]);

#endif

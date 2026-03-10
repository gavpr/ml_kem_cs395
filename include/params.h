#ifndef PARAMS_H
#define PARAMS_H

#include <stdint.h>

// Approved parameter sets (ML-KEM)
#define ML_KEM_N 256
#define ML_KEM_Q 3329
#define ML_KEM_SSK 32	// shared secret key (bytes)

#define ML_KEM_512_K  2
#define ML_KEM_768_K  3
#define ML_KEM_1024_K 4

#define ML_KEM_512_ETA1  3
#define ML_KEM_768_ETA1  2
#define ML_KEM_1024_ETA1 2

#define ML_KEM_512_ETA2  2 
#define ML_KEM_768_ETA2  2
#define ML_KEM_1024_ETA2 2

#define ML_KEM_512_DU  10
#define ML_KEM_768_DU  10
#define ML_KEM_1024_DU 11

#define ML_KEM_512_DV  4
#define ML_KEM_768_DV  4
#define ML_KEM_1024_DV 5

// required bit generator strength (bits)
#define ML_KEM_512_RBG  128
#define ML_KEM_768_RBG  192
#define ML_KEM_1024_RBG 256


// Sizes of keys and ciphertexts (bytes)
#define ML_KEM_512_EK  800
#define ML_KEM_768_EK  1184
#define ML_KEM_1024_EK 1568

#define ML_KEM_512_DK  1632
#define ML_KEM_768_DK  2400
#define ML_KEM_1024_DK 3168

#define ML_KEM_512_CTEXT  768
#define ML_KEM_768_CTEXT  1088
#define ML_KEM_1024_CTEXT 1568

#endif

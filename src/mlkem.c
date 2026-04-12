#include <stdlib.h>
#include <string.h>
#include "mlkem.h"
#include "kpke.h"
#include "fips202.h"
#include "randombytes.h"


//Uses randomness (d,z) to generate an encapsulation key and a corresponding decapsulation key.
static void ml_kem_keygen_internal(const uint8_t d[], const uint8_t z[], uint8_t ek[], uint8_t dk[], const ml_kem_params *params){
    uint8_t ek_pke[384 * params->k + 32];
    uint8_t dk_pke[384 * params->k];

    //generate keys
    k_pke_keygen(d, ek_pke, dk_pke, params);

    //encapsulation key
    memcpy(ek, ek_pke, params->ek_size);

    //32 len hash output
    uint8_t H_out[32];
    sha3_256(H_out, ek, params->ek_size);

    //decapsulation key
    memcpy(dk, dk_pke, 384 * params->k);
    memcpy(dk + (384 * params->k), ek, params->ek_size);
    memcpy(dk + (384 * params->k) + params->ek_size, H_out, 32);
    memcpy(dk + (768 * params->k) + params->ek_size + 32, z, 32);
}

//Uses the encapsulation key and randomness to generate a key and an associated ciphertext.
static void ml_kem_encaps_internal(const uint8_t ek[], const uint8_t m[], uint8_t ssk[], uint8_t c[], const ml_kem_params *params) {
    
    //32 len hash output
    uint8_t H_out[32];
    sha3_256(H_out, ek, params->ek_size);

    uint8_t G_input[64];    // m || H(ek)
    uint8_t G_output[64];
    memcpy(G_input, m, 32);
    memcpy(G_input + 32, H_out, 32);

    sha3_512(G_output, G_input, 64);
    
    //derive the shared secret and randomness r;
    uint8_t r[32];
    memcpy(ssk, G_output, 32);
    memcpy(r, G_output + 32, 32);

    //encrypt the message
    k_pke_encrypt(ek, m, r, c, params);
}

//Uses the decapsulation key to produce a shared secret key from a ciphertext.
static void ml_kem_decaps_internal(const uint8_t dk[], const uint8_t c[], uint8_t ssk[], const ml_kem_params *params){
    //extract dk_pke
    uint8_t dk_pke[384 * params->k];
    memcpy(dk_pke, dk, 384 * params->k);

    //extract ek_pke
    uint8_t ek_pke[params->ek_size];
    memcpy(ek_pke, dk + (384 * params->k), params->ek_size);

    //extract hash of pke_ek key
    uint8_t h[32];
    memcpy(h, dk + (384 * params->k) + params->ek_size, 32);

    //extract implicit rejection value
    uint8_t z[32];
    memcpy(z, dk + (384 * params->k) + params->ek_size + 32, 32);

    uint8_t m[32];
    k_pke_decrypt(dk_pke, c, m, params);

    uint8_t G_input[64];    // decipher ciphertext
    uint8_t G_output[64];
    memcpy(G_input, m, 32);
    memcpy(G_input + 32, h, 32);

    sha3_512(G_output, G_input, 64);

    //derive ssk and randomness r
    uint8_t r[32];
    memcpy(ssk, G_output, 32);
    memcpy(r, G_output + 32, 32);

    //reencrypt ssk using r
    uint8_t K_bar[32];
    uint8_t J_input[32 + params->ctext_size];
    memcpy(J_input, z, 32);
    memcpy(J_input + 32, c, params->ctext_size);
    shake256(K_bar, 32, J_input, 32 + params->ctext_size);

    //reencrypt using derived randomness
    uint8_t c_prime[params->ctext_size];
    k_pke_encrypt(ek_pke, m, r, c_prime, params);

    if(memcmp(c_prime, c, params->ctext_size) != 0) {
        memcpy(ssk, K_bar, 32);
    }
}

int ml_kem_512_keygen(uint8_t ek[ML_KEM_512_EK], uint8_t dk[ML_KEM_512_DK]) {
    //select parameter set
    const ml_kem_params *params = &ML_KEM_512;

    //generate two sets of random bytes (32)
    uint8_t d[32];
    uint8_t z[32];

    if(randombytes(d, 32) != 0) {
        return -1;  //bit generation failure
    }

    if(randombytes(z, 32) != 0) {
        return -1;  //bit generation failure
    }

    ml_kem_keygen_internal(d, z, ek, dk, params);
    return 0;
}

int ml_kem_512_encaps(const uint8_t ek[ML_KEM_512_EK], uint8_t k[ML_KEM_SSK], uint8_t c[ML_KEM_512_CTEXT]) {
    //select parameter set
    const ml_kem_params *params = &ML_KEM_512;

    //m random 32 bytes
    uint8_t m[32];
    if(randombytes(m, 32) != 0) {
        return -1;
    }

    ml_kem_encaps_internal(ek, m, k, c, params);
    return 0;
}

int ml_kem_512_decaps(const uint8_t dk[ML_KEM_512_DK], const uint8_t c[ML_KEM_512_CTEXT], uint8_t k[ML_KEM_SSK]) {
    //select parameter set
    const ml_kem_params *params = &ML_KEM_512;
    ml_kem_decaps_internal(dk, c, k, params);

    return 0;
}

int ml_kem_768_keygen(uint8_t ek[ML_KEM_768_EK], uint8_t dk[ML_KEM_768_DK]) {
    //select parameter set
    const ml_kem_params *params = &ML_KEM_768;

    //generate two sets of random bytes (32)
    uint8_t d[32];
    uint8_t z[32];

    if(randombytes(d, 32) != 0) {
        return -1;  //bit generation failure
    }

    if(randombytes(z, 32) != 0) {
        return -1;  //bit generation failure
    }

    ml_kem_keygen_internal(d, z, ek, dk, params);
    return 0;
}

int ml_kem_768_encaps(const uint8_t ek[ML_KEM_768_EK], uint8_t k[ML_KEM_SSK], uint8_t c[ML_KEM_768_CTEXT]) {
    //select parameter set
    const ml_kem_params *params = &ML_KEM_768;

    //m random 32 bytes
    uint8_t m[32];
    if(randombytes(m, 32) != 0) {
        return -1;
    }

    ml_kem_encaps_internal(ek, m, k, c, params);
    return 0;
}

int ml_kem_768_decaps(const uint8_t dk[ML_KEM_768_DK], const uint8_t c[ML_KEM_768_CTEXT], uint8_t k[ML_KEM_SSK]) {
    //select parameter set
    const ml_kem_params *params = &ML_KEM_768;
    ml_kem_decaps_internal(dk, c, k, params);

    return 0;
}

int ml_kem_1024_keygen(uint8_t ek[ML_KEM_1024_EK], uint8_t dk[ML_KEM_1024_DK]) {
    //select parameter set
    const ml_kem_params *params = &ML_KEM_10242;

    //generate two sets of random bytes (32)
    uint8_t d[32];
    uint8_t z[32];

    if(randombytes(d, 32) != 0) {
        return -1;  //bit generation failure
    }

    if(randombytes(z, 32) != 0) {
        return -1;  //bit generation failure
    }

    ml_kem_keygen_internal(d, z, ek, dk, params);
    return 0;
}

int ml_kem_1024_encaps(const uint8_t ek[ML_KEM_1024_EK], uint8_t k[ML_KEM_SSK], uint8_t c[ML_KEM_1024_CTEXT]) {
    //select parameter set
    const ml_kem_params *params = &ML_KEM_1024;

    //m random 32 bytes
    uint8_t m[32];
    if(randombytes(m, 32) != 0) {
        return -1;
    }

    ml_kem_encaps_internal(ek, m, k, c, params);
    return 0;
}

int ml_kem_1024_decaps(const uint8_t dk[ML_KEM_1024_DK], const uint8_t c[ML_KEM_1024_CTEXT], uint8_t k[ML_KEM_SSK]) {
    //select parameter set
    const ml_kem_params *params = &ML_KEM_1024;
    ml_kem_decaps_internal(dk, c, k, params);

    return 0;
}

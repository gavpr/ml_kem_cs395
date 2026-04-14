#ifndef TEST_H
#define TEST_H

#include "../include/mlkem.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>


void hex_to_bytes(const char *hex, uint8_t *out, size_t len) {
    for (size_t i = 0; i < len; i++) {
        sscanf(hex + 2*i, "%02hhx", &out[i]);
    }
}

void run_keygen_test(const char *d_, const char *z_, const char *ek_, const char *dk_, const char *paramSet_) {

    //get param set
    const ml_kem_params *params;
    if (strcmp(paramSet_, "ML-KEM-512") == 0) {
        params = &ML_KEM_512;
    } else if (strcmp(paramSet_, "ML-KEM-768") == 0) {
        params = &ML_KEM_768;
    } else if (strcmp(paramSet_, "ML-KEM-1024") == 0) {
        params = &ML_KEM_1024;
    } else {
        printf("FAILED: unknown param set %s\n", paramSet_);
        return;
    }

    //function parameters
    uint8_t d[32];
    uint8_t z[32];
    uint8_t ek[params->ek_size];
    uint8_t dk[params->dk_size];

    //expected values
    uint8_t expected_ek[params->ek_size];
    uint8_t expected_dk[params->dk_size];

    //convert hex inputs to bytes
    hex_to_bytes(d_, d, 32);
    hex_to_bytes(z_, z, 32);
    hex_to_bytes(ek_, expected_ek, params->ek_size);
    hex_to_bytes(dk_, expected_dk, params->dk_size);

    //run internal key generation with test vector values
    ml_kem_keygen_internal(d, z, ek, dk, params);

    //print encaps key
    printf("ek got:      ");
    for (int i = 0; i < 8; i++) printf("%02x", ek[i]);
    printf("...");
    printf("\n");
    printf("ek expected: ");
    for (int i = 0; i < 8; i++) printf("%02x", expected_ek[i]);
    printf("...");
    printf("\n");

    //print decaps key
    printf("dk got:      ");
    for (int i = 0; i < 8; i++) printf("%02x", dk[i]);
    printf("...");
    printf("\n");
    printf("dk expected: ");
    for (int i = 0; i < 8; i++) printf("%02x", expected_dk[i]);
    printf("...");
    printf("\n");

    // compare output against expected
    if (memcmp(ek, expected_ek, params->ek_size) == 0 &&
        memcmp(dk, expected_dk, params->dk_size) == 0) {
        printf("PASSED\n\n");
    } else {
        printf("FAILED\n\n");
    }
}

#endif
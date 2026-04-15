#ifndef TEST_H
#define TEST_H

#include "../include/mlkem.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


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
    clock_t start_time = clock();
    ml_kem_keygen_internal(d, z, ek, dk, params);
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC * 1000000.0; //measures microseconds

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
        printf("PASSED [%s] %.2f us\n\n", paramSet_, elapsed_time);
    } else {
        printf("FAILED [%s] %.2f us\n\n", paramSet_, elapsed_time);
    }
}

void run_encap_decap_test(const char *ek_, const char *dk_, const char *m_, const char *k_, const char *c_, const char *paramSet_) {

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
    uint8_t k[32];
    uint8_t k2[32];
    uint8_t m[32];
    uint8_t c[params->ctext_size];
    uint8_t ek[params->ek_size];
    uint8_t dk[params->dk_size];

    //expected values for encapsulation and decaps
    uint8_t expected_k[ML_KEM_SSK];

    //expected values for encapsulation
    uint8_t expected_c[params->ctext_size];

    //convert hex inputs to bytes
    hex_to_bytes(ek_, ek, params->ek_size);
    hex_to_bytes(dk_, dk, params->dk_size);
    //expected values
    hex_to_bytes(m_, m, 32);
    hex_to_bytes(c_, expected_c, params->ctext_size);
    hex_to_bytes(k_, expected_k, 32);
    hex_to_bytes(k_, expected_k, ML_KEM_SSK);

    //run internal key generation with test vector values
    clock_t start_time = clock();
    ml_kem_encaps_internal(ek, m, k, c, params);
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC * 1000000.0; //measures microseconds

    //print shared secret
    printf("k got:      ");
    for (int i = 0; i < 8; i++) printf("%02x", k[i]);
    printf("...");
    printf("\n");
    printf("k expected: ");
    for (int i = 0; i < 8; i++) printf("%02x", expected_k[i]);
    printf("...");
    printf("\n");

    //print ciphertext
    printf("c got:      ");
    for (int i = 0; i < 8; i++) printf("%02x", c[i]);
    printf("...");
    printf("\n");
    printf("c expected: ");
    for (int i = 0; i < 8; i++) printf("%02x", expected_c[i]);
    printf("...");
    printf("\n");

    // compare output against expected
    if (memcmp(c, expected_c, params->ctext_size) == 0 &&
        memcmp(k, expected_k, ML_KEM_SSK) == 0) {
        printf("PASSED ENCAPSULATION[%s] %.2f us\n\n", paramSet_, elapsed_time);
    } else {
        printf("FAILED ENCAPSULATION[%s] %.2f us\n\n", paramSet_, elapsed_time);
    }

    //run internal key generation with test vector values
    clock_t start_time2 = clock();
    ml_kem_decaps_internal(dk, c, k2, params);
    clock_t end_time2 = clock();
    double elapsed_time2 = (double)(end_time2 - start_time2) / CLOCKS_PER_SEC * 1000000.0; //measures microseconds

    //print ciphertext
    printf("k got:      ");
    for (int i = 0; i < 8; i++) printf("%02x", k2[i]);
    printf("...");
    printf("\n");
    printf("k expected: ");
    for (int i = 0; i < 8; i++) printf("%02x", expected_k[i]);
    printf("...");
    printf("\n");

    // compare shared secrets
    if (memcmp(k2, expected_k, ML_KEM_SSK) == 0) {
        printf("PASSED DECAPSULATION[%s] %.2f us\n\n", paramSet_, elapsed_time2);
    } else {
        printf("FAILED DECAPSULATION[%s] %.2f us\n\n", paramSet_, elapsed_time2);
    }
}

#endif
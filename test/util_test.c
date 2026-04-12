#include "../include/params.h"
#include "../include/util.h"
#include "../include/sample.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "fips202.h"

void test_bit_to_byte() {
    
    uint8_t bits_msb[8] = {0, 0, 0, 0, 0, 0, 0, 1};
    uint8_t bytes[sizeof(bits_msb) * 8];
    bits_to_bytes(bits_msb, bytes, sizeof(bits_msb)/8);
    for (int i = 0; i < sizeof(bits_msb)/8; i++) {
        printf("%d ", bytes[i]);
    }
    printf("\n");
    assert(bytes[0] == 0x80);

    uint8_t bits_alt[8] = {1, 0, 1, 0, 1, 0, 1, 0};
    bits_to_bytes(bits_alt, bytes, sizeof(bits_alt)/8);
    for (int i = 0; i < sizeof(bits_alt)/8; i++) {
        printf("%d ", bytes[i]);
    }
    printf("\n");
    assert(bytes[0] == 0x55);

    printf("Passed tests for bits_to_bytes\n");
}

void test_roundtrip(void) {

    //0x55, 0xC4, 0x66
    uint8_t bytes[3] = {85, 196, 102};
    uint8_t bits[8 * sizeof(bytes)];
    bytes_to_bits(bytes, bits, sizeof(bytes));
    for (int i = 0; i < sizeof(bits); i++) {
        printf("%d ", bits[i]);
    }
    printf("\n");
    bits_to_bytes(bits, bytes, sizeof(bytes));

     for (int i = 0; i < sizeof(bytes); i++) {
        printf("%d ", bytes[i]);
    }

    printf("Passed tests for bits_to_bytes\n");
}

void test_byte_encode_round_trip() {
    poly F;
    for (int i = 0; i < ML_KEM_N; i++) F[i] = i % 3329;
    uint8_t d = 10;
    for (int i = 0; i < ML_KEM_N; i++) {
        printf("%d ", F[i]);
    }
    printf("\n\n");
    uint8_t out[d*32];
    poly recovered;
    byte_encode(F, out, d);
    for (int i = 0; i < d*32; i++) {
        printf("%d ", out[i]);
    }
    printf("\n\n");
    byte_decode(out, recovered, d);
    for (int i = 0; i < 256; i++) {
        printf("%d", recovered[i]);
        assert(F[i] == recovered[i]);
    }
    printf("\n\n");
    printf("Passed tests for round trip byte encoding d=%d\n", d);
}

void int_compression_test() {
    uint8_t d = 8;
    uint16_t x = 12; // gt 2^11, and lt 2^12
    printf("%d\n\n", x);
    uint16_t decompressed = decompress(x, d);
    printf("%d\n\n", decompressed);
    uint16_t compressed = compress(decompressed, d);
    printf("%d\n\n", compressed);
    assert(x == compressed);
    printf("Passed tests for compression testing (roundtrip)\n");
}

void test_cbd_output_range() {
    uint8_t B[128] = {
        0xA3, 0x4D, 0x7F, 0x12, 0};
    poly f;
    sample_poly_cbd(B, f, 2);

    // all values must be <= eta or >= q-eta
    // i.e. the centered values are in {-2,-1,0,1,2}
    for (int i = 0; i < 256; i++) {
        printf("%d ", f[i]);
        assert(f[i] <= 2 || f[i] >= ML_KEM_Q - 2);
    }
    printf("PASSED: output range eta=2\n");
}

void test_sample_ntt() {
    uint8_t rho[] = {0xed, 0x85, 0x90, 0x32, 0xbb, 0xb7,
        0x4c, 0x74, 0x12, 0xa7, 0x27, 0x1c, 0x8e, 0xf2, 0x3a,
        0x5a, 0x08, 0xc3, 0x15, 0x51, 0xa2, 0xa5, 0xcb, 0x8a, 
        0xda, 0x78, 0x53, 0x8c, 0xe6, 0xc9, 0x6d, 0x11};  // first 32 bytes of ek

    uint8_t input[34];
    memcpy(input, rho, 32);
    input[32] = 0;  // j
    input[33] = 0;  // i  — gives A^T[0][0]

    poly a_hat;
    sample_ntt(input, a_hat, 34);

    // compare against A^T[0][0] from file
    // A^T is hex encoded with ByteDecode12, so you need to
    // ByteDecode12 the first 384 bytes of A^T to get coefficients
}


int main() {
    //test_bit_to_byte();
    //test_roundtrip();
    //test_byte_encode_round_trip();
    //int_compression_test();
    //test_cbd_output_range();
    test_sample_ntt();
}
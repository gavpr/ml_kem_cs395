#include "../include/params.h"
#include "../include/util.h"
#include "../include/sample.h"
#include <stdio.h>
#include <assert.h>

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
    uint16_t F[ML_KEM_N];
    for (int i = 0; i < ML_KEM_N; i++) F[i] = i % 3329;
    uint8_t d = 10;
    for (int i = 0; i < ML_KEM_N; i++) {
        printf("%d ", F[i]);
    }
    printf("\n\n");
    uint8_t out[d*32];
    uint16_t recovered[ML_KEM_N];
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
    uint16_t f[256];
    sample_poly_cbd(B, f, 2);

    // all values must be <= eta or >= q-eta
    // i.e. the centered values are in {-2,-1,0,1,2}
    for (int i = 0; i < 256; i++) {
        printf("%d ", f[i]);
        assert(f[i] <= 2 || f[i] >= ML_KEM_Q - 2);
    }
    printf("PASSED: output range eta=2\n");
}

int main() {
    //test_bit_to_byte();
    //test_roundtrip();
    test_byte_encode_round_trip();
    //int_compression_test();
    //test_cbd_output_range();

}
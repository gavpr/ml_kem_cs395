#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "../include/params.h"
#include "../include/mlkem.h"

void bytes_to_hex(const uint8_t *bytes, size_t len, char *hex_out) {
    for(size_t i = 0; i < len; i++) {
        sprintf(hex_out + 2*i, "%02x", bytes[i]);
    }
    hex_out[2*len] = '\0';
}

int main() {
    uint8_t ek[ML_KEM_512_EK];
    uint8_t dk[ML_KEM_512_DK];

    //generate keys
    ml_kem_512_keygen(ek, dk);

    //encapsulate
    uint8_t bob_secret[32];
    uint8_t ciphertext[ML_KEM_512_CTEXT];
    ml_kem_512_encaps(ek, bob_secret, ciphertext);
    char bobhex[65];
    bytes_to_hex(bob_secret, 32, bobhex);
    printf("Bob's Secret: ");
    printf("%s ", bobhex);
    printf("\n\n");

    //decapsulate
    uint8_t alice_secret[32];
    ml_kem_512_decaps(dk, ciphertext, alice_secret);
    char alicehex[65];
    bytes_to_hex(alice_secret, 32, alicehex);
    printf("Alice's Secret: ");
    printf("%s ", alicehex);
    

    

    return -1;
}
#include "ntt.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if(argc > 2) {
        return 1;
    }
    uint16_t r = (uint16_t)atoi(argv[1]);

    printf("Input seven bit number: %d\n", r);
    uint16_t reversed_r = bit_rev_7(r);
    printf("Reversed seven bit number: %d\n", reversed_r);

    return 0;
}
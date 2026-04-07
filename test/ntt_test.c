#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "../include/ntt.h"
#include "../include/params.h"

int main(int argc, char* argv[]) {

    poly f = {2619, 102, 1126, 914, 3016, 2771, 2233, 2418, 130, 383, 952, 2465, 2298};
    
    int olength = sizeof(f) / sizeof(f[0]);
    poly g = {456, 3037, 1003, 571, 419, 3033, 356, 1728, 122, 895, 2069, 108, 814};

    for (int i = 0; i < olength; i++) {
        printf("%d ", f[i]);
    }
    printf("\n\n");

    for (int i = 0; i < olength; i++) {
        printf("%d ", g[i]);
    }
    printf("\n\n");

    ntt(f);
    ntt(g);

    for (int i = 0; i < olength; i++) {
        printf("%d ", f[i]);
    }
    printf("\n\n");

    for (int i = 0; i < olength; i++) {
        printf("%d ", g[i]);
    }
    printf("\n\n");

    poly h;
    multiply_ntts(f, g, h);

    for (int i = 0; i < olength; i++) {
        printf("%d ", h[i]);
    }
    printf("\n\n");

    ntt_inv(h);
    for (int i = 0; i < olength; i++) {
        printf("%d ", h[i]);
    }
    printf("\n\n");

    return 0;
}
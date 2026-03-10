#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include "params.h"

//functions to imploement quickly

//byte_encode
//byte decode
//compress
//decompress

// convert a bit array of length l*8 into array of bytes
void bits_to_bytes(const uint8_t* b, uint8_t* B, int l);

//convert a byte array of length l*8 into an array of bits
void bytes_to_bits(const uint8_t* B, uint8_t* b, int l);

#endif
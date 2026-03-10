#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include "params.h"

//functions to imploement quickly

//byte_encode
//byte decode
//compress
//decompress

// convert a bit array into array of bytes
void bits_to_bytes(const uint8_t* b, uint8_t* B, int l);

//convert a byte array into an array of bits
void bytes_to_bits(const uint8_t* B, uint8_t* b, int l);

//encodes array of d-bit integers into a byte array for 1 <= d <= 12
//output B is length 32*d bytes
void byte_encode(const uint16_t F[ML_KEM_N], uint8_t* B, uint8_t d);

//decodes a byte array into an array of d-bit integers for 1 <= d <= 12
//again input byte array (B) should be length 32*d
void byte_decode(const uint8_t* B, uint16_t F[ML_KEM_N], uint8_t d);

#endif
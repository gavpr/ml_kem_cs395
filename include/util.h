#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include "params.h"


// convert a bit array into array of bytes
void bits_to_bytes(const uint8_t* b, uint8_t* B, int l);

//convert a byte array into an array of bits
void bytes_to_bits(const uint8_t* B, uint8_t* b, int l);

//encodes array of d-bit integers into a byte array for 1 <= d <= 12
//output B is length 32*d bytes
void byte_encode(const poly F, uint8_t* B, uint8_t d);

//decodes a byte array into an array of d-bit integers for 1 <= d <= 12
//again input byte array (B) should be length 32*d
void byte_decode(const uint8_t* B, poly F, uint8_t d);

//takes an integer x in Z_q and compresses it into an integer of Z_2^d
//for an d <= 11
uint16_t compress(const uint16_t x, uint8_t d);

//takes an integer x in Z_2^d for any d <= 11 and decompresses it into
//an integer in Z_q
uint16_t decompress(const uint16_t, uint8_t d);


#endif
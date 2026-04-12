# FIPS 203 (ML-KEM) CS 395 Cybersecurity Certificate Capstone
Implementation of FIPS 203 Module-Lattice-Based Key-Encapsulation Mechanism Standard aka Kyber for CS 395 Cybersecurity Capstone

https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.203.pdf 

## Dependencies

This implementation uses the following library:

PQClean: https://github.com/PQClean/PQClean/tree/master/common

  Used For:
  - Random byte generation
  - FIPS 202 hashing functions (SHAKE128, SHAKE256, SHA3-256, SHA3-512)
  - Imported in /include/fips202.h and /include/randombytes.h

#include "kpke.h"
#include "util.h"
#include "ntt.h"
#include "sample.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//uses encryption key to encrypt a plaintext message using randomness r
void k_pke_encrypt(const uint8_t ek[], const uint8_t m[], const uint8_t r[], uint8_t c[], const ml_kem_params *params){
    uint8_t N = 0;
    vec t_hat;

    for(uint16_t i = 0; i < params->k; i++) {
        const uint8_t *current = ek + (384 * i);  //chunk the encryption key
        byte_decode(current, t_hat[i], 12);
    }
    
    //extract the 32-byte seed from the key
    const uint8_t *rho = ek + 384 * params->k;    //[ek * 384k : ek * 384k + 32]
    
    //reconstruct the matrix A
    matrix A_hat;
    for(uint16_t i = 0; i < params->k; i++) {
        for(uint16_t j = 0; j < params->k; j++) {
            uint8_t input_buffer[34];
            memcpy(input_buffer, rho, 32);
            input_buffer[32] = j;   //33 byte
            input_buffer[33] = i;   //34 byte
            sample_ntt(input_buffer, A_hat[i][j], 34);
        }
    }

    vec y;
    for(uint16_t i = 0; i < params->k; i++) {
        uint8_t input_buffer[33];
        memcpy(input_buffer, r, 32);
        input_buffer[32] = N;
        uint8_t output_buffer[64 * params->eta1];
        shake256(output_buffer, sizeof(output_buffer), input_buffer, sizeof(input_buffer));
        sample_poly_cbd(output_buffer, y[i], params->eta1);
        N++;
    }

    vec e1;
    for(uint16_t i = 0; i < params->k; i++) {
        uint8_t input_buffer[33];
        memcpy(input_buffer, r, 32);
        input_buffer[32] = N;
        uint8_t output_buffer[64 * params->eta2];
        shake256(output_buffer, sizeof(output_buffer), input_buffer, sizeof(input_buffer));
        sample_poly_cbd(output_buffer, e1[i], params->eta2);
        N++;
    }

    poly e2;
    uint8_t e2_input[33];
    memcpy(e2_input, r, 32);
    e2_input[32] = N;
    uint8_t e2_output[64 * params->eta2];
    shake256(e2_output, sizeof(e2_output), e2_input, sizeof(e2_input));
    sample_poly_cbd(e2_output, e2, params->eta2);

    //y -> yhat
    vec y_hat;
    for(uint16_t i = 0; i < params->k; i++) {
        memcpy(y_hat[i], y[i], sizeof(poly));
        ntt(y_hat[i]);
    }

    vec u;
    for(uint16_t i = 0; i < params->k; i++) {
        poly tp = {0};
        for(uint16_t j = 0; j < params->k; j++) {
            poly mult_output;
            multiply_ntts(A_hat[j][i], y_hat[j], mult_output);
            for(uint16_t k = 0; k < ML_KEM_N; k++) {
                tp[k] = (tp[k] + mult_output[k]) % ML_KEM_Q;
            }
        }
        ntt_inv(tp);
        for(uint16_t l = 0; l < ML_KEM_N; l++) {
            u[i][l] = (tp[l] + e1[i][l]) % ML_KEM_Q;
        }
    }

    poly mu;
    byte_decode(m, mu, 1);
    for(uint16_t i = 0; i < ML_KEM_N; i++) {
        mu[i] = decompress(mu[i], 1);
    }

    poly v;
    poly tp = {0};
    for(uint16_t j = 0; j < params->k; j++) {
        poly mult_output;
        multiply_ntts(t_hat[j], y_hat[j], mult_output);
        for(uint16_t k = 0; k < ML_KEM_N; k++) {
            tp[k] = (tp[k] + mult_output[k]) % ML_KEM_Q;
        }
    }
    ntt_inv(tp);
    for(uint16_t l = 0; l < ML_KEM_N; l++) {
        v[l] = (tp[l] + e2[l] + mu[l]) % ML_KEM_Q;
    }

    uint8_t *c1 = c;
    for(uint16_t i = 0; i < params->k; i++) {
        poly u_compressed;
        for(uint16_t j = 0; j < ML_KEM_N; j++) {
            u_compressed[j] = compress(u[i][j], params->du);
        }
        byte_encode(u_compressed, c1 + i * 32 * params->du, params->du);
    }


    uint8_t *c2 = c + 32 * params->du * params->k;
    poly v_compressed;
    for(uint16_t j = 0; j < ML_KEM_N; j++) {
        v_compressed[j] = compress(v[j], params->dv);
    }
    byte_encode(v_compressed, c2, params->dv);

}



//uses the decryption key to decrypt a given ciphertext
void k_pke_decrypt(const uint8_t dk[], const uint8_t c[], uint8_t m[], const ml_kem_params *params) {
    const uint8_t *c1 = c; //[0 : 32 * PARAMS->du * PARAMS->k]
    const uint8_t *c2 = c + (32 * params->du * params->k);    //[32 * PARAMS->du * PARAMS->k : PARAMS->ctext_size]

    vec u_prime;    //vector of k polynomials from c
    for(uint16_t i = 0; i < params->k; i++) {
        const uint8_t *current = c1 + (32 * params->du * i);  //current location of c buffer
        byte_decode(current, u_prime[i], params->du);   //run byte decode k times 

        for(uint16_t k = 0; k < ML_KEM_N; k++) {    //run decompress on the decoded bytes
            u_prime[i][k] = decompress(u_prime[i][k], params->du);
        }
    }

    poly v_prime;
    byte_decode(c2, v_prime, params->dv);
    for(uint16_t i = 0; i < ML_KEM_N; i++) {
        v_prime[i] = decompress(v_prime[i], params->dv);
    }

    vec s_hat; //run byte_decode k times on dk_key
    for(uint16_t i = 0; i < params->k; i++) {
        const uint8_t *current = dk + (384 * i);  //chunk the decryption key
        byte_decode(current, s_hat[i], 12);
    }

    //ntt u_prime k times
    for(uint16_t i = 0; i < params->k; i++) {
        ntt(u_prime[i]);    //convert each poly in u_prime to ntt transformation
    }

    poly w = {0};
    poly z_hat;
    for(uint16_t i = 0; i < params->k; i++) {
        multiply_ntts(s_hat[i], u_prime[i], z_hat);
        for(uint16_t j = 0; j < ML_KEM_N; j++) {
            w[j] = (w[j] + z_hat[j]) % ML_KEM_Q;
        }
    }

    ntt_inv(w);

    for(uint16_t j = 0; j < ML_KEM_N; j++) {
        w[j] = (v_prime[j] - w[j] + ML_KEM_Q) % ML_KEM_Q;
    }
    
    //extract message
    for(uint16_t j = 0; j < ML_KEM_N; j++) {
        w[j] = compress(w[j], 1);
    }

    byte_encode(w, m, 1);
}


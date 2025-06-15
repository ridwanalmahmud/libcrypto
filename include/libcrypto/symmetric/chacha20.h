#ifndef _CHACHA20_H
#define _CHACHA20_H

#include <stdint.h>
#include <stddef.h>

#define CHACHA20_KEY_SIZE 32
#define CHACHA20_NONCE_SIZE 12
#define CHACHA20_BLOCK_SIZE 64

typedef struct {
    uint32_t state[16];
    uint8_t keystream[CHACHA20_BLOCK_SIZE];
    size_t position;
} chacha20_ctx;

// Initialize a ChaCha20 context with key and nonce
void chacha20_init(chacha20_ctx *ctx,
                   const uint8_t *key,
                   const uint8_t *nonce,
                   uint32_t counter);

// Generate random key
void chacha20_generate_key(uint8_t *key);

// Generate random nonce
void chacha20_generate_nonce(uint8_t *nonce);

// Encrypt/decrypt data using ChaCha20
void chacha20_crypt(chacha20_ctx *ctx,
                    const uint8_t *input,
                    uint8_t *output,
                    size_t len);

// Encrypt data (convenience function)
void chacha20_encrypt(const uint8_t *key,
                      const uint8_t *nonce,
                      uint32_t counter,
                      const uint8_t *plaintext,
                      uint8_t *ciphertext,
                      size_t len);

// Decrypt data (convenience function)
void chacha20_decrypt(const uint8_t *key,
                      const uint8_t *nonce,
                      uint32_t counter,
                      const uint8_t *ciphertext,
                      uint8_t *plaintext,
                      size_t len);

#endif // !_CHACHA20_H

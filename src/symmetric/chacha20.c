#include "symmetric/chacha20.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Rotate left operation
#define ROTL(a, b) (((a) << (b)) | ((a) >> (32 - (b))))

// ChaCha20 quarter round
#define QR(a, b, c, d) \
    (a += b,           \
     d ^= a,           \
     d = ROTL(d, 16),  \
     c += d,           \
     b ^= c,           \
     b = ROTL(b, 12),  \
     a += b,           \
     d ^= a,           \
     d = ROTL(d, 8),   \
     c += d,           \
     b ^= c,           \
     b = ROTL(b, 7))

void chacha20_block(chacha20_ctx *ctx, uint32_t counter) {
    uint32_t x[16];

    // Copy state to working variables
    for (int i = 0; i < 16; i++) {
        x[i] = ctx->state[i];
    }

    // Set block counter
    x[12] = counter;

    // 10 iterations = 20 rounds
    for (int i = 0; i < 10; i++) {
        // Column rounds
        QR(x[0], x[4], x[8], x[12]);
        QR(x[1], x[5], x[9], x[13]);
        QR(x[2], x[6], x[10], x[14]);
        QR(x[3], x[7], x[11], x[15]);

        // Diagonal rounds
        QR(x[0], x[5], x[10], x[15]);
        QR(x[1], x[6], x[11], x[12]);
        QR(x[2], x[7], x[8], x[13]);
        QR(x[3], x[4], x[9], x[14]);
    }

    // Add working state to original state
    for (int i = 0; i < 16; i++) {
        x[i] += ctx->state[i];
    }

    // Convert to little-endian byte array
    for (int i = 0; i < 16; i++) {
        ctx->keystream[i * 4 + 0] = (uint8_t)(x[i] >> 0);
        ctx->keystream[i * 4 + 1] = (uint8_t)(x[i] >> 8);
        ctx->keystream[i * 4 + 2] = (uint8_t)(x[i] >> 16);
        ctx->keystream[i * 4 + 3] = (uint8_t)(x[i] >> 24);
    }

    ctx->position = 0;
}

void chacha20_init(chacha20_ctx *ctx,
                   const uint8_t *key,
                   const uint8_t *nonce,
                   uint32_t counter) {
    // Constants
    ctx->state[0] = 0x61707865;
    ctx->state[1] = 0x3320646e;
    ctx->state[2] = 0x79622d32;
    ctx->state[3] = 0x6b206574;

    // Key
    for (int i = 0; i < 8; i++) {
        ctx->state[4 + i] =
            ((uint32_t)key[i * 4 + 0] << 0) | ((uint32_t)key[i * 4 + 1] << 8) |
            ((uint32_t)key[i * 4 + 2] << 16) | ((uint32_t)key[i * 4 + 3] << 24);
    }

    // Counter
    ctx->state[12] = counter;

    // Nonce
    for (int i = 0; i < 3; i++) {
        ctx->state[13 + i] = ((uint32_t)nonce[i * 4 + 0] << 0) |
                             ((uint32_t)nonce[i * 4 + 1] << 8) |
                             ((uint32_t)nonce[i * 4 + 2] << 16) |
                             ((uint32_t)nonce[i * 4 + 3] << 24);
    }

    ctx->position = CHACHA20_BLOCK_SIZE; // Force block generation on first use
}

void chacha20_generate_key(uint8_t *key) {
    srand(time(NULL));
    for (size_t i = 0; i < CHACHA20_KEY_SIZE; i++) {
        key[i] = rand() & 0xFF;
    }
}

void chacha20_generate_nonce(uint8_t *nonce) {
    srand(time(NULL));
    for (size_t i = 0; i < CHACHA20_NONCE_SIZE; i++) {
        nonce[i] = rand() & 0xFF;
    }
}

void chacha20_crypt(chacha20_ctx *ctx,
                    const uint8_t *input,
                    uint8_t *output,
                    size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (ctx->position >= CHACHA20_BLOCK_SIZE) {
            chacha20_block(ctx, ctx->state[12]);
            ctx->state[12]++;
            ctx->position = 0;
        }

        output[i] = input[i] ^ ctx->keystream[ctx->position];
        ctx->position++;
    }
}

void chacha20_encrypt(const uint8_t *key,
                      const uint8_t *nonce,
                      uint32_t counter,
                      const uint8_t *plaintext,
                      uint8_t *ciphertext,
                      size_t len) {
    chacha20_ctx ctx;
    chacha20_init(&ctx, key, nonce, counter);
    chacha20_crypt(&ctx, plaintext, ciphertext, len);
}

void chacha20_decrypt(const uint8_t *key,
                      const uint8_t *nonce,
                      uint32_t counter,
                      const uint8_t *ciphertext,
                      uint8_t *plaintext,
                      size_t len) {
    chacha20_ctx ctx;
    chacha20_init(&ctx, key, nonce, counter);
    chacha20_crypt(&ctx, ciphertext, plaintext, len);
}

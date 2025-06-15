#include "symmetric/chacha20.h"
#include "test_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    const uint8_t *key;
    const uint8_t *nonce;
    uint32_t counter;
    const uint8_t *input;
    uint8_t *output;
    size_t len;
} chacha20_test_args;

void test_encrypt_wrapper(void *args) {
    chacha20_test_args *targs = (chacha20_test_args *)args;
    chacha20_encrypt(targs->key,
                     targs->nonce,
                     targs->counter,
                     targs->input,
                     targs->output,
                     targs->len);
}

void test_decrypt_wrapper(void *args) {
    chacha20_test_args *targs = (chacha20_test_args *)args;
    chacha20_decrypt(targs->key,
                     targs->nonce,
                     targs->counter,
                     targs->input,
                     targs->output,
                     targs->len);
}

void test_basic_encryption_decryption(void) {
    printf("=== Basic Encryption/Decryption Test ===\n");

    uint8_t key[CHACHA20_KEY_SIZE] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
        0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
        0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

    uint8_t nonce[CHACHA20_NONCE_SIZE] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x00};

    const char *plaintext = "Hello, ChaCha20! This is a test message.";
    size_t len = strlen(plaintext);

    uint8_t ciphertext[len];
    uint8_t decrypted[len];

    // Test encryption
    chacha20_encrypt(key, nonce, 1, (uint8_t *)plaintext, ciphertext, len);

    // Test decryption
    chacha20_decrypt(key, nonce, 1, ciphertext, decrypted, len);

    printf("Original: %s\n", plaintext);
    printf("Decrypted: %.*s\n", (int)len, decrypted);

    if (compare_bytes((uint8_t *)plaintext, decrypted, len)) {
        printf("Test PASSED\n");
    } else {
        printf("Test FAILED\n");
    }

    printf("\n");
}

void test_random_key_nonce(void) {
    printf("=== Random Key/Nonce Test ===\n");

    uint8_t key[CHACHA20_KEY_SIZE];
    uint8_t nonce[CHACHA20_NONCE_SIZE];

    generate_random_bytes(key, CHACHA20_KEY_SIZE);
    generate_random_bytes(nonce, CHACHA20_NONCE_SIZE);

    const char *plaintext = "Another test message with random key and nonce.";
    size_t len = strlen(plaintext);

    uint8_t ciphertext[len];
    uint8_t decrypted[len];

    chacha20_encrypt(key, nonce, 0, (uint8_t *)plaintext, ciphertext, len);
    chacha20_decrypt(key, nonce, 0, ciphertext, decrypted, len);

    printf("Key: ");
    print_hex(key, CHACHA20_KEY_SIZE);
    printf("Nonce: ");
    print_hex(nonce, CHACHA20_NONCE_SIZE);
    printf("Original: %s\n", plaintext);
    printf("Decrypted: %.*s\n", (int)len, decrypted);

    if (compare_bytes((uint8_t *)plaintext, decrypted, len)) {
        printf("Test PASSED\n");
    } else {
        printf("Test FAILED\n");
    }

    printf("\n");
}

void test_empty_message(void) {
    printf("=== Empty Message Test ===\n");

    uint8_t key[CHACHA20_KEY_SIZE];
    uint8_t nonce[CHACHA20_NONCE_SIZE];

    generate_random_bytes(key, CHACHA20_KEY_SIZE);
    generate_random_bytes(nonce, CHACHA20_NONCE_SIZE);

    const char *plaintext = "";
    size_t len = 0;

    uint8_t ciphertext[1]; // Dummy buffer
    uint8_t decrypted[1]; // Dummy buffer

    chacha20_encrypt(key, nonce, 0, (uint8_t *)plaintext, ciphertext, len);
    chacha20_decrypt(key, nonce, 0, ciphertext, decrypted, len);

    printf("Empty message processed without errors.\n");
    printf("Test PASSED\n");

    printf("\n");
}

void test_large_message_performance(void) {
    printf("=== Large Message Performance Test ===\n");

    size_t len = 1024 * 1024; // 1MB
    uint8_t *key = malloc(CHACHA20_KEY_SIZE);
    uint8_t *nonce = malloc(CHACHA20_NONCE_SIZE);
    uint8_t *plaintext = malloc(len);
    uint8_t *ciphertext = malloc(len);

    if (!key || !nonce || !plaintext || !ciphertext) {
        printf("Memory allocation failed\n");
        free(key);
        free(nonce);
        free(plaintext);
        free(ciphertext);
        return;
    }

    generate_random_bytes(key, CHACHA20_KEY_SIZE);
    generate_random_bytes(nonce, CHACHA20_NONCE_SIZE);

    // Fill with some data
    generate_random_bytes(plaintext, len);

    chacha20_test_args args = {.key = key,
                               .nonce = nonce,
                               .counter = 0,
                               .input = plaintext,
                               .output = ciphertext,
                               .len = len};

    run_performance_tests(test_encrypt_wrapper, &args, 5, len);

    free(key);
    free(nonce);
    free(plaintext);
    free(ciphertext);
}

int main(void) {
    // Initialize random number generator
    srand(time(NULL));

    // Functional tests
    test_basic_encryption_decryption();
    test_random_key_nonce();
    test_empty_message();

    // Performance tests
    test_large_message_performance();

    return 0;
}

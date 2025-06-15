#include "test_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

bool compare_bytes(const uint8_t *a, const uint8_t *b, size_t len) {
    return memcmp(a, b, len) == 0;
}

void print_hex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 4 == 0) {
            printf(" ");
        }
    }
    if (len % 16 != 0) {
        printf("\n");
    }
}

double measure_time(void (*func)(void *), void *args) {
    clock_t start = clock();
    func(args);
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

void generate_random_bytes(uint8_t *buf, size_t len) {
    // Note: For testing only - not cryptographically secure!
    // In production, use a proper CSPRNG
    for (size_t i = 0; i < len; i++) {
        buf[i] = rand() & 0xFF;
    }
}

void run_performance_tests(void (*test_func)(void *),
                           void *args,
                           int iterations,
                           size_t data_size) {
    double total_time = 0.0;
    double min_time = 1e10;
    double max_time = 0.0;

    printf("Running performance tests (%d iterations, %zu bytes):\n",
           iterations,
           data_size);

    for (int i = 0; i < iterations; i++) {
        double time = measure_time(test_func, args);
        total_time += time;
        if (time < min_time)
            min_time = time;
        if (time > max_time)
            max_time = time;
        printf("Run %d: %.6f seconds\n", i + 1, time);
    }

    printf("\nPerformance Summary:\n");
    printf("Total time: %.6f seconds\n", total_time);
    printf("Average time: %.6f seconds\n", total_time / iterations);
    printf("Min time: %.6f seconds\n", min_time);
    printf("Max time: %.6f seconds\n", max_time);

    if (data_size > 0) {
        double avg_time = total_time / iterations;
        printf("Throughput: %.2f MB/s\n",
               (data_size / (1024.0 * 1024.0)) / avg_time);
    }
}

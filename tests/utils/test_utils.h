#ifndef _TEST_UTILS_H
#define _TEST_UTILS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>


// Compare two byte arrays
bool compare_bytes(const uint8_t *a, const uint8_t *b, size_t len);

// Print a byte array in hex
void print_hex(const uint8_t *data, size_t len);

// Measure the time taken to execute a function
// return double Time in seconds
double measure_time(void (*func)(void *), void *args);

// Generate random bytes
void generate_random_bytes(uint8_t *buf, size_t len);

// Run performance tests with a given test function
void run_performance_tests(void (*test_func)(void *),
                           void *args,
                           int iterations,
                           size_t data_size);

#endif // !_TEST_UTILS_H

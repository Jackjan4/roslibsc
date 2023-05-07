#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Calculates the point on the Dino-Dini-Sigmoid curve. x is a float between 0 and 1 (inclusive). k is a float between 0 and 1 (exclusive)
 * 
 * @param x 
 * @param k 
 * @return float 
 */
float calculate_dino_dini_sigmoid(float x, float k);


void ull_to_base32hex_str(unsigned long long int num, size_t buf_size, char* buffer);

#ifdef __cplusplus
}
#endif
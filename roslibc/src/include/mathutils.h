#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Calculates a point on the tunable Sigmoid curve by Dino Dini. x is a float between 0 and 1 (inclusive). k is a float between 0 and 1 (exclusive). Function explanation can be seen here: https://dhemery.github.io/DHE-Modules/technical/sigmoid/ & https://dinodini.wordpress.com/2010/04/05/normalized-tunable-sigmoid-functions/
 * 
 * @param x 
 * @param k 
 * @return
 */
float calculate_dino_dini_sigmoid(float x, float k);



/// @brief Converts an uinsigned long long to a base32hex (hexadeciaml with extended alphabet) string.
/// @param num The input unsigned long long
/// @param buffer The buffer to which to store the result
/// @param buf_size The size of the resulting buffer
void ull_to_base32hex_str(unsigned long long int num, char* buffer, size_t buf_size,);

#ifdef __cplusplus
}
#endif
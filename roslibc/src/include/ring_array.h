#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ring_array {
    uint8_t size;
    uint32_t *items;
    uint8_t current_item_pos;
};



/// @brief Creates a ring array structure that implements an array in a ring formation, thus reverting its pointer back to the beginning when it reaches the end
/// @param r_array The array to be initiated. Pass an unitilized struct here 
/// @param memory_buffer The pointer to the memory buffer which should hold the data
/// @param size The number of elements that the memory buffer, and thus the ring array, can hold
void ring_array_create(struct ring_array* r_array, uint32_t* memory_buffer, uint8_t size);



/// @brief Sets all entries of the ring array to the specified value
/// @param r_array Pointer to the ring array
/// @param value Value which all entries should be set to
void ring_array_set_all(struct ring_array* r_array, uint32_t value);



/// @brief Sets the value of the current ring array entry to the specified value and increases the pointer position by one.
/// @param r_array Pointer to the ring array
/// @param value Value to which the current entry should be set to
void ring_array_put_next(struct ring_array* r_array, uint32_t value);

#ifdef __cplusplus
}
#endif
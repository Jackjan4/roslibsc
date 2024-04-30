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

void ring_array_create(struct ring_array* r_array, uint32_t* memory_buffer, uint8_t size);

void ring_array_set_all(struct ring_array* r_array, uint32_t value);

void ring_array_put_next(struct ring_array* r_array, uint32_t value);

#ifdef __cplusplus
}
#endif
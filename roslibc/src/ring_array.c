#include "ring_array.h"

#include <stdlib.h>

void ring_array_create(struct ring_array* r_array, uint32_t* memory_buffer, uint8_t size) {
    r_array->size = size;
    r_array->current_item_pos = 0;
    r_array->items = memory_buffer;
}

void ring_array_set_all(struct ring_array* r_array, uint32_t value) {
    for(uint8_t i = 0; i < r_array->size; i++) {
        r_array->items[i] = value;
    }
}

void ring_array_put_next(struct ring_array* r_array, uint32_t value) {
    r_array->items[r_array->current_item_pos] = value;
    r_array->current_item_pos++;

    if (r_array->current_item_pos == r_array->size) {
        r_array->current_item_pos = 0;
    }
}
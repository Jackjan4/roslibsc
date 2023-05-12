#pragma once

// C Header
#include <stdint.h>



#ifdef __cplusplus
extern "C" {
#endif

enum display_adapter_bitmode_t {
    DISPLAY_ADAPTER_GRAYSCALE_BITMODE_1_BIT = 0x00,
    DISPLAY_ADAPTER_GRAYSCALE_BITMODE_2_BIT = 0x01,
    DISPLAY_ADAPTER_GRAYSCALE_BITMODE_4_BIT = 0x02,
    DISPLAY_ADAPTER_COLOR_BITMODE_8_BIT = 0x03,
};

enum display_adapter_device_id_t {
    DISPLAY_ADAPTER_DEVICE_ID_SSD1327 = 0x01
};



enum display_adapter_rotation_t {
    DISPLAY_ADAPTER_ROTATION_0 = 0x00,
    DISPLAY_ADAPTER_ROTATION_90 = 0x01,
    DISPLAY_ADAPTER_ROTATION_180 = 0x02,
    DISPLAY_ADAPTER_ROTATION_270 = 0x03,
};



enum display_adapter_buffer_addressing_mode_t {
    DISPLAY_ADAPTER_ADDRESSING_MODE_HORIZONTAL = 0x00,
    DISPLAY_ADAPTER_ADDRESSING_MODE_VERTICAL = 0x01,
    DISPLAY_ADAPTER_ADDRESSING_MODE_VERTICAL_BLOCKS = 0x02,

};

struct display_adapter_descriptor {
    uint8_t* display_buffer;
    const uint16_t buffer_size;
    void (*fn_write_buffer_to_display)(struct display_adapter_descriptor*, uint8_t, uint8_t, void*);
    void* payload;
    const uint8_t width;
    const uint8_t height;
    const enum display_adapter_rotation_t rotation;
    const enum display_adapter_buffer_addressng_mode_t addressing_mode;
    const enum display_adapter_bitmode_t bitmode;
    const enum display_adapter_device_id_t device_id;
};


void display_adapter_set_pixel_in_buffer(const struct display_adapter_descriptor* adapter, uint8_t x, uint8_t y, uint8_t value);

int display_adapter_write_buffer_to_display(const struct display_adapter_descriptor* adapter, uint8_t x, uint8_t y, void* payload);

inline void display_adapter_fill_display_buffer(const struct display_adapter_descriptor* adapter, uint8_t value) {
    for (uint16_t i = 0; i < adapter->buffer_size; i++) {
        adapter->display_buffer[i] = value;
    }
}

#ifdef __cplusplus
}
#endif
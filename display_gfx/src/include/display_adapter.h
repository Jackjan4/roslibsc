#pragma once

// C Header
#include <stdint.h>

// Zephyr Headers
#include <drivers/display.h>

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

struct display_adapter_descriptor {
    uint8_t* display_buffer;
    const uint16_t buffer_size;
    const struct device* display_device;
    const uint8_t width;
    const uint8_t height;
    const enum display_adapter_rotation_t rotation;
    const enum display_adapter_bitmode_t bitmode;
    const enum display_adapter_device_id_t device_id;
};

struct display_adapter_descriptor display_adapter_create_ssd1327(enum display_adapter_bitmode_t bitmode, enum display_adapter_rotation_t rotation, const struct device* display_device);

void display_adapter_set_pixel_in_buffer(const struct display_adapter_descriptor* adapter, uint8_t x, uint8_t y, uint8_t value);

int display_adapter_write_buffer_to_display(const struct display_adapter_descriptor* adapter, uint8_t x, uint8_t y, const struct display_buffer_descriptor* buf_desc);

inline void display_adapter_fill_display_buffer(const struct display_adapter_descriptor* adapter, uint8_t value) {
    for (uint16_t i = 0; i < adapter->buffer_size; i++) {
        adapter->display_buffer[i] = value;
    }
}

#ifdef __cplusplus
}
#endif
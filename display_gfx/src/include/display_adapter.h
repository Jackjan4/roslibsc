#pragma once


// Include tweaks
#if __has_include(<display_gfx.tweaks.h>)
  #include <display_gfx.tweaks.h>
#endif

// C Header
#include <stdint.h>
#include <string.h> // memset



#ifdef __cplusplus
extern "C" {
#endif



enum display_adapter_bitmode_t {
    DISPLAY_ADAPTER_BITMODE_GRAYSCALE_1_BIT = 0x00, // e.g. black and white LCDs/OLEDs
    DISPLAY_ADAPTER_BITMODE_GRAYSCALE_2_BIT = 0x01, // e.g. grayscale OLEDs with 4 colors
    DISPLAY_ADAPTER_BITMODE_GRAYSCALE_4_BIT = 0x02, // e.g. grayscale OLEDs with 16 colors
    DISPLAY_ADAPTER_BITMODE_COLOR_8_BIT = 0x03,
};


/// @brief Definition of the supported devices
enum display_adapter_device_id_t {
    DISPLAY_ADAPTER_DEVICE_ID_ZEPHYR_SSD1327 = 0x01,
    DISPLAY_ADAPTER_DEVICE_ID_ZEPHYR_SSD1306A = 0x02
};



enum display_adapter_rotation_t {
    DISPLAY_ADAPTER_ROTATION_0 = 0x00,
    DISPLAY_ADAPTER_ROTATION_90 = 0x01,
    DISPLAY_ADAPTER_ROTATION_180 = 0x02,
    DISPLAY_ADAPTER_ROTATION_270 = 0x03,
};



/// @brief The addressing mode that the display driver uses. Can be found in the datasheet of the display driver. Many display driver support multiple addressing modes and can be configured.
enum display_adapter_buffer_addressing_mode_t {
    DISPLAY_ADAPTER_BUFFER_ADDRESSING_MODE_HORIZONTAL = 0x00,
    DISPLAY_ADAPTER_BUFFER_ADDRESSING_MODE_VERTICAL = 0x01,
    DISPLAY_ADAPTER_BUFFER_ADDRESSING_MODE_VERTICAL_BLOCKS = 0x02,

};

struct display_adapter_descriptor {
    uint8_t* display_buffer;
    const uint16_t buffer_size;
    void (*fn_write_buffer_to_display)(struct display_adapter_descriptor*, uint8_t, uint8_t, void*);
    void (*fn_set_pixel_in_buffer)(struct display_adapter_descriptor*, uint8_t, uint8_t, uint8_t);
    void* payload;
    const uint8_t width;
    const uint8_t height;
    const enum display_adapter_rotation_t rotation;
    const enum display_adapter_buffer_addressing_mode_t addressing_mode;
    const enum display_adapter_bitmode_t bitmode;
    const enum display_adapter_device_id_t device_id;
};



/// @brief Sets a single pixel in the display buffer to the given value
/// @param adapter The display adapter which should be used
/// @param x The x coordinate of the pixel
/// @param y The x coordinate of the pixel
/// @param value The value to which to set the pixel to
void display_adapter_set_pixel_in_buffer(const struct display_adapter_descriptor* adapter, uint8_t x, uint8_t y, uint8_t value);



/// @brief 
/// @param adapter The display adapter which should be used
/// @param x The X start coordinate of the pixel from which to send the buffer to the physical display. Normally 0 but can be used to only send partial images to the physical display to safe on bandwidth.
/// @param y The Y start coordinate of the pixel from which to send the buffer to the physical display. Normally 0 but can be used to only send partial images to the physical display to safe on bandwidth.
/// @return 
int display_adapter_write_buffer_to_display(const struct display_adapter_descriptor* adapter, uint8_t x, uint8_t y);



/// @brief Fills the whole display buffer with the given value
/// @param adapter The display adapter which should be used
/// @param value The value which should be used for filling all entries in the display buffer
inline void display_adapter_fill_display_buffer(const struct display_adapter_descriptor* adapter, unsigned char value) {
    memset(adapter->display_buffer, value, adapter->buffer_size);
}

#ifdef __cplusplus
}
#endif
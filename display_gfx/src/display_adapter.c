#include "display_adapter.h"


void display_adapter_set_pixel_in_buffer(const struct display_adapter_descriptor* adapter, uint8_t x, uint8_t y, uint8_t value) {
    // Check if value is incorrect for bitmode and thus cannot be represented
#ifdef DISPLAY_GFX_SAFETY_CHECKS

#endif

    // Check if pixel is out of bounds
#ifdef DISPLAY_GFX_SAFETY_CHECKS
    if (x >= display_get || y >= height) return;
#endif

#if defined(DISPLAY_GFX_STATIC_ROTATION_0)

#elif defined(DISPLAY_GFX_STATIC_ROTATION_90)
    // Transpose coordinates
    uint8_t x_temp = x;
    x = y;
    y = x_temp;
#elif defined(DISPLAY_GFX_STATIC_ROTATION_180)

#elif defined(DISPLAY_GFX_STATIC_ROTATION_270)
    // Transpose coordinates
    uint8_t x_temp = x;
    x = y;
    y = x_temp;

    // Reverse y / column
    y = (adapter->height - 1) - y;
#else
    switch (adapter->rotation) {
        case (DISPLAY_ADAPTER_ROTATION_90): {
            // Transpose coordinates
            uint8_t x_temp = x;
            x = y;
            y = x_temp;

            // Reverse x / row
            x = (adapter->width - 1) - x;
            break;
        }
        case (DISPLAY_ADAPTER_ROTATION_270): {
            // Transpose coordinates
            uint8_t x_temp = x;
            x = y;
            y = x_temp;

            // Reverse y / column
            y = (adapter->height - 1) - y;
            break;
        }
    }
#endif


short buffer_position = -1;
#if defined(DISPLAY_GFX_STATIC_ADDRESSING_MODE_HORIZONTAL)
    uint16_t pixel_position = y * adapter->width + x;
    uint8_t is_higher = pixel_position & 0x01;
    buffer_position = pixel_position >> (3 - adapter->bitmode);
#elif defined(DISPLAY_GFX_STATIC_ADDRESSING_MODE_VERTICAL)
    uint8_t x_temp = x;
    x = y;
    y = x_temp;
#elif defined(DISPLAY_GFX_STATIC_ADDRESSING_MODE_VERTICAL_BLOCKS)
    uint8_t buffer_y = y / (8 << adapter->bitmode);
    buffer_position = buffer_y * adapter->width + x;
#else
    switch(adapter->addressing_mode) {
        case(): {

            break;
        }
    }
#endif


    
#if defined(DISPLAY_GFX_STATIC_BITMODE_GRAYSCALE_1_BIT)
    if(value > 1) {
        value = 1;
    }
    // Get current display_buffer byte because there are multiple pixels in that byte and we only need to modify one of them
    uint8_t buffer_value = adapter->display_buffer[buffer_position];

    // Clear bit first
    adapter->display_buffer[buffer_position] &= ~(value << (y & 7));

    // Then set it
    adapter->display_buffer[buffer_position] |= (value << (y & 7));
    
#elif defined(DISPLAY_GFX_STATIC_BITMODE_GRAYSCALE_2_BIT)

#elif defined(DISPLAY_GFX_STATIC_BITMODE_GRAYSCALE_4_BIT)
    // Write pixel to display buffer
    if (!is_higher) {
        adapter->display_buffer[buffer_position] = (buffer_value & 0b00001111) + (value << 4);
    } else {
        adapter->display_buffer[buffer_position] = (buffer_value & 0b11110000) + value;
    }
#elif defined(DISPLAY_GFX_STATIC_BITMODE_COLOR_8_BIT)

#else

#endif
    
}

int display_adapter_write_buffer_to_display(const struct display_adapter_descriptor* adapter, uint8_t x, uint8_t y, void* payload) {
    int err = 0;

    (adapter->fn_write_buffer_to_display)(adapter, x, y, payload);

    return err;
}
#include "display_adapter.h"

struct display_adapter_descriptor display_adapter_create_ssd1327(enum display_adapter_bitmode_t bitmode, enum display_adapter_rotation_t rotation, const struct device* display_device) {
    struct display_capabilities cap;
    display_get_capabilities(display_device, &cap);

    uint16_t buffer_size = 0;
    switch (bitmode) {
        case (DISPLAY_ADAPTER_GRAYSCALE_BITMODE_1_BIT): {
            buffer_size = (128 * 128) >> 3;
            break;
        }
        case (DISPLAY_ADAPTER_GRAYSCALE_BITMODE_2_BIT): {
            buffer_size = (128 * 128) >> 2;
            break;
        }
        case (DISPLAY_ADAPTER_GRAYSCALE_BITMODE_4_BIT): {
            buffer_size = (128 * 128) >> 1;
            break;
        }
        case (DISPLAY_ADAPTER_COLOR_BITMODE_8_BIT): {
            // TODO: Error
            break;
        }
    }

    struct display_adapter_descriptor result = {
        .display_buffer = (uint8_t*)k_calloc(buffer_size, sizeof(uint8_t)),
        .buffer_size = buffer_size,
        .display_device = display_device,
        .width = cap.x_resolution,
        .height = cap.y_resolution,
        .rotation = rotation,
        .bitmode = bitmode,
        .device_id = DISPLAY_ADAPTER_DEVICE_ID_SSD1327,
    };

    return result;
}

void display_adapter_set_pixel_in_buffer(const struct display_adapter_descriptor* adapter, uint8_t x, uint8_t y, uint8_t value) {
    // Check if value is incorrect for bitmode and thus cannot be represented
#ifdef DISPLAY_GFX_SAFETY_CHECKS

#endif

    // Check if pixel is out of bounds
#ifdef DISPLAY_GFX_SAFETY_CHECKS
    if (x >= display_get || y >= height) return;
#endif

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

    uint16_t pixel_position = y * adapter->width + x;
    uint8_t is_higher = pixel_position & 0x01;
    uint16_t buffer_position = pixel_position >> (3 - adapter->bitmode);

    // Get current display_buffer byte because there are multiple pixels in that byte and we only need to modify one of them
    uint8_t buffer_value = adapter->display_buffer[buffer_position];

    // Write pixel to display buffer
    if (!is_higher) {
        adapter->display_buffer[buffer_position] = (buffer_value & 0b00001111) + (value << 4);
    } else {
        adapter->display_buffer[buffer_position] = (buffer_value & 0b11110000) + value;
    }
}

int display_adapter_write_buffer_to_display(const struct display_adapter_descriptor* adapter, uint8_t x, uint8_t y, const struct display_buffer_descriptor* buf_desc) {
    int err = display_write(adapter->display_device, x, y, buf_desc, adapter->display_buffer);

    return err;
}
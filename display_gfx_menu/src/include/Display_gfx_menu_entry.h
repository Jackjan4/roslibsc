#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum display_gfx_menu_entry_type_t {
    DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_BACK = 0x00, // Text with a back symbol in after it
    DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_SUBMENU = 0x01, // text with a submenu symbol after of it
    DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT = 0x02, // A single piece of text that is static at compile time
    DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_CHOOSER = 0x03, // A chooser to pick one text from others
    DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_INTEGER_RANGE = 0x04,
    DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_BUTTON = 0x05,
    DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_SINGLE_VALUE = 0x06,
    DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_BOX = 0x07, // A multiline text with automatic wrapping
    DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_DYNAMIC = 0x08, // A single piece of text that is dynamic during runtime
    DISPLAY_GFX_MENU_ENTRY_TYPE_DIVIDER = 0x09,
    DISPLAY_GFX_MENU_ENTRY_TYPE_DINO_DINI_HALF_SIGMOID = 0x0a,
    DISPLAY_GFX_MENU_ENTRY_TYPE_DINO_DINI_FULL_SIGMOID = 0x0b,
    DISPLAY_GFX_MENU_ENTRY_TYPE_CIRCLE = 0x0c,
    DISPLAY_GFX_MENU_ENTRY_TYPE_GAUGE = 0x0d,
    DISPLAY_GFX_MENU_ENTRY_TYPE_IMAGE = 0x0e,
};

enum display_gfx_menu_entry_state_t {
    DISPLAY_GFX_MENU_ENTRY_STATE_IDLE = 0x00,
    DISPLAY_GFX_MENU_ENTRY_STATE_FULL_REDRAW_NEEDED = 0x01,
    DISPLAY_GFX_MENU_ENTRY_STATE_HEADER_CHANGED = 0x02,
    DISPLAY_GFX_MENU_ENTRY_STATE_VALUE_CHANGED = 0x03
};

enum display_gfx_menu_entry_image_type_t {
    DISPLAY_GFX_MENU_ENTRY_IMAGE_TYPE_MONO_1_PIXEL_PER_BYTE = 0x01,
    DISPLAY_GFX_MENU_ENTRY_IMAGE_TYPE_MONO_8_PIXEL_PER_BYTE = 0x08
};

struct display_gfx_menu_entry {
    display_gfx_menu_entry_type_t entry_type;
    bool selectable;

    uint8_t x_start;
    uint8_t y_start;
    uint8_t x_end;
    uint8_t y_end;

    void (*click_callback)(int16_t);
    void (*left_callback)(int16_t);
    void (*right_callback)(int16_t);

    uint8_t (*handle_frame)(struct display_gfx_menu_entry*);
    void (*receive_new_position)(uint8_t*, uint8_t*);

    union {
        struct {
            const char* header_text;
            short locale_key;

            union {
                struct {
                    char* text;
                    char* old_text;
                    uint8_t buffer_length;
                    void (*value_receive)(char*);
                } string_value;

                struct {
                    uint8_t current_text;
                    const char** text_array;
                    uint8_t array_size;
                    uint8_t (*value_receive)(void);
                    void (*value_changed)(uint8_t new_value);

                    uint8_t old_text_;
                } chooser_value;

                struct {
                    long start;
                    long end;
                    unsigned long stepsize;
                    long current_value;

                    long (*value_receive)(void);
                    void (*value_changed)(long new_value);

                    long old_value_;
                } int_range_value;

                struct {
                } textbox_value;

                struct {
                    uint8_t redirect_menu_id;
                } redirect_value;
            };
        } textual_entry;

        struct {
        } line_entry;

        struct {
            float k;

            float (*value_receive)(void);

            float old_k_;
        } sigmoid_entry;

        struct {
            uint8_t radius;

            uint8_t x_center_old_;
            uint8_t y_center_old_;
        } circle_entry;

        struct {
            uint8_t radius;
            uint16_t gauge_value;
            uint16_t range;

            uint16_t (*value_receive)(void);
        } gauge_entry;

        struct {
            const display_gfx_menu_entry_image_type_t img_format;
            const void* pixel_data;
        } image_entry;
    };

    uint8_t tag;
    display_gfx_menu_entry_state_t state_;
};

#ifdef __cplusplus
}
#endif
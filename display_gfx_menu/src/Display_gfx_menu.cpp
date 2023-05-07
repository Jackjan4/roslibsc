// Headers
#include "Display_gfx_menu.hpp"

// C Header
#include <stdio.h>
#include <string.h>

// Zephyr libs
#ifdef CONFIG_LOG
#include <logging/log.h>
#endif

// Library Header
#include "RobotoLight12p7b.h"

// Application libs
#include "Display_gfx_menu_entry.h"

#ifdef CONFIG_LOG
LOG_MODULE_REGISTER(display_gfx_menu);
#endif

Display_gfx_menu::Display_gfx_menu(uint8_t menu_id, display_gfx_menu_type_t menu_type, display_gfx_menu_entry** entries, uint8_t num_entries, uint8_t current_position, const GFXfont* gfxfont, uint8_t f_scale) : menu_id(menu_id), menu_type(menu_type), entries(entries), num_entries(num_entries), textfont(gfxfont), font_scale(f_scale), current_entry_pos(current_position), menu_state(OLED_GFX_MENU_STATE_FULL_REDRAW_NEEDED){};

Display_gfx_menu::Display_gfx_menu(uint8_t menu_id, display_gfx_menu_type_t menu_type, display_gfx_menu_entry** entries, uint8_t num_entries, const GFXfont* gfxfont, uint8_t f_scale) : menu_id(menu_id), menu_type(menu_type), entries(entries), num_entries(num_entries), textfont(gfxfont), font_scale(f_scale), menu_state(OLED_GFX_MENU_STATE_FULL_REDRAW_NEEDED) {
    for (uint8_t i = 0; i < num_entries; i++) {
        if (entries[i]->selectable) {
            current_entry_pos = i;
            break;
        }
    }
};

Display_gfx_menu::Display_gfx_menu(uint8_t menu_id, display_gfx_menu_type_t menu_type, display_gfx_menu_entry** entries, uint8_t num_entries, uint8_t current_position, const GFXfont* gfxfont, uint8_t f_scale, const char** locale_keys) : menu_id(menu_id), menu_type(menu_type), entries(entries), num_entries(num_entries), textfont(gfxfont), font_scale(f_scale), locale_dict(locale_keys), current_entry_pos(current_position), menu_state(OLED_GFX_MENU_STATE_FULL_REDRAW_NEEDED){};

Display_gfx_menu::Display_gfx_menu(uint8_t menu_id, display_gfx_menu_type_t menu_type, display_gfx_menu_entry** entries, uint8_t num_entries, const GFXfont* gfxfont, uint8_t f_scale, const char** locale_keys) : menu_id(menu_id), menu_type(menu_type), entries(entries), num_entries(num_entries), textfont(gfxfont), font_scale(f_scale), locale_dict(locale_keys), menu_state(OLED_GFX_MENU_STATE_FULL_REDRAW_NEEDED) {
    for (uint8_t i = 0; i < num_entries; i++) {
        if (entries[i]->selectable) {
            current_entry_pos = i;
            break;
        }
    }
};

/**
 * @brief
 *
 * @param display_gfx
 * @return true
 * @return false
 */
unsigned char Display_gfx_menu::draw_menu(const struct display_adapter_descriptor* display_adapter) {
    unsigned char result = 0;

    if (menu_state & OLED_GFX_MENU_STATE_FULL_REDRAW_NEEDED) {
        draw_full_menu(display_adapter);
        result = 1;
    } else {
        if (menu_state & OLED_GFX_MENU_STATE_POSITION_CHANGED) {
            draw_position_cursor(display_adapter);
            result = 1;
        }
        if (menu_state & OLED_GFX_MENU_STATE_ENTRY_CHANGED) {
            for (uint8_t i = 0; i < num_entries; i++) {
                if (entries[i]->state_ != DISPLAY_GFX_MENU_ENTRY_STATE_IDLE) {
                    draw_menu_entry(display_adapter, i, true);
                    result = 1;
                }
            }
        }
    }

    menu_state = OLED_GFX_MENU_STATE_IDLE;
    return result;
}

/**
 * @brief
 *
 * @param display_gfx
 */
void Display_gfx_menu::draw_full_menu(const display_adapter_descriptor* display_adapter) {
    // Clear display buffer -> blank display
    display_adapter_fill_display_buffer(display_adapter, 0x00);

    // Draw menu decoration
    draw_menu_decor(display_adapter);

    for (uint8_t i = 0; i < num_entries; i++) {
        draw_menu_entry(display_adapter, i, false);
    }

    // TODO: Create check if current entry is even selectable
    // Select current position visually
    draw_position_cursor(display_adapter);
};

/**
 * @brief
 *
 */
void Display_gfx_menu::select_entry() {
    display_gfx_menu_entry* cur_entry = entries[current_entry_pos];

    if (cur_entry->click_callback != NULL)
        cur_entry->click_callback(-1);
}

/**
 * @brief
 *
 */
void Display_gfx_menu::select_left() {
    display_gfx_menu_entry* cur_entry = entries[current_entry_pos];

    switch (cur_entry->entry_type) {
        case DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_INTEGER_RANGE: {
            if ((long)(cur_entry->textual_entry.int_range_value.current_value - cur_entry->textual_entry.int_range_value.stepsize) >= cur_entry->textual_entry.int_range_value.start) {
                cur_entry->textual_entry.int_range_value.old_value_ = cur_entry->textual_entry.int_range_value.current_value;
                cur_entry->textual_entry.int_range_value.current_value -= cur_entry->textual_entry.int_range_value.stepsize;
                if (menu_state != OLED_GFX_MENU_STATE_FULL_REDRAW_NEEDED) {
                    menu_state |= OLED_GFX_MENU_STATE_ENTRY_CHANGED;
                }
                cur_entry->state_ = DISPLAY_GFX_MENU_ENTRY_STATE_VALUE_CHANGED;
                if (cur_entry->textual_entry.int_range_value.value_changed) {
                    cur_entry->textual_entry.int_range_value.value_changed(cur_entry->textual_entry.int_range_value.current_value);
                }
            }
            break;
        }
        case DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_CHOOSER: {
            uint8_t current_id = cur_entry->textual_entry.chooser_value.current_text;
            if (current_id > 0) {
                cur_entry->textual_entry.chooser_value.old_text_ = cur_entry->textual_entry.chooser_value.current_text;
                cur_entry->textual_entry.chooser_value.current_text--;
                if (menu_state != OLED_GFX_MENU_STATE_FULL_REDRAW_NEEDED) {
                    menu_state |= OLED_GFX_MENU_STATE_ENTRY_CHANGED;
                }
                cur_entry->state_ = DISPLAY_GFX_MENU_ENTRY_STATE_VALUE_CHANGED;
                if (cur_entry->textual_entry.chooser_value.value_changed) {
                    cur_entry->textual_entry.chooser_value.value_changed(cur_entry->textual_entry.chooser_value.current_text);
                }
            }
            break;
        }
    }

    if (cur_entry->left_callback != NULL) {
        cur_entry->left_callback(0);
    }
}

/**
 * @brief
 *
 */
void Display_gfx_menu::select_right() {
    display_gfx_menu_entry* cur_entry = entries[current_entry_pos];

    switch (cur_entry->entry_type) {
        case DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_INTEGER_RANGE: {
            if ((long)(cur_entry->textual_entry.int_range_value.current_value + cur_entry->textual_entry.int_range_value.stepsize) <= cur_entry->textual_entry.int_range_value.end) {
                cur_entry->textual_entry.int_range_value.old_value_ = cur_entry->textual_entry.int_range_value.current_value;
                cur_entry->textual_entry.int_range_value.current_value += cur_entry->textual_entry.int_range_value.stepsize;
                if (menu_state != OLED_GFX_MENU_STATE_FULL_REDRAW_NEEDED) {
                    menu_state |= OLED_GFX_MENU_STATE_ENTRY_CHANGED;
                }
                cur_entry->state_ = DISPLAY_GFX_MENU_ENTRY_STATE_VALUE_CHANGED;
                if (cur_entry->textual_entry.int_range_value.value_changed) {
                    cur_entry->textual_entry.int_range_value.value_changed(cur_entry->textual_entry.int_range_value.current_value);
                }
            }
            break;
        }
        case DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_CHOOSER: {
            uint8_t current_id = cur_entry->textual_entry.chooser_value.current_text;
            uint8_t arr_size = cur_entry->textual_entry.chooser_value.array_size;
            if (current_id < arr_size - 1) {
                cur_entry->textual_entry.chooser_value.old_text_ = cur_entry->textual_entry.chooser_value.current_text;
                cur_entry->textual_entry.chooser_value.current_text++;
                if (menu_state != OLED_GFX_MENU_STATE_FULL_REDRAW_NEEDED) {
                    menu_state |= OLED_GFX_MENU_STATE_ENTRY_CHANGED;
                }
                cur_entry->state_ = DISPLAY_GFX_MENU_ENTRY_STATE_VALUE_CHANGED;
                if (cur_entry->textual_entry.chooser_value.value_changed) {
                    cur_entry->textual_entry.chooser_value.value_changed(cur_entry->textual_entry.chooser_value.current_text);
                }
            }
            break;
        }
    }

    if (cur_entry->right_callback) {
        cur_entry->right_callback(0);
    }
}

/**
 * @brief
 *
 * @param entry
 * @return true
 * @return false
 */
unsigned char Display_gfx_menu::receive_entry_value(uint8_t entry) {
    unsigned char result = 0;
    display_gfx_menu_entry* cur_entry = entries[entry];

    switch (cur_entry->entry_type) {
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_SINGLE_VALUE): {
            if (cur_entry->textual_entry.string_value.value_receive) {
                // Copy current text to old_text
                strncpy(cur_entry->textual_entry.string_value.old_text, cur_entry->textual_entry.string_value.text, cur_entry->textual_entry.string_value.buffer_length);

                // Receive new value to current_text buffer
                cur_entry->textual_entry.string_value.value_receive(cur_entry->textual_entry.string_value.text);

                if (strcmp(cur_entry->textual_entry.string_value.old_text, cur_entry->textual_entry.string_value.text)) {
                    // Strings are not equal
                    result = 1;
                }
            }
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_GAUGE): {
            if (cur_entry->gauge_entry.value_receive != NULL) {
                uint8_t old_val = cur_entry->gauge_entry.gauge_value;
                uint8_t received_val = cur_entry->gauge_entry.value_receive();
                if (old_val != received_val) {
                    cur_entry->gauge_entry.gauge_value = received_val;
                    result = 1;
                }
            }
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_DYNAMIC): {
            if (cur_entry->textual_entry.string_value.value_receive) {
                // Copy current text to old_text
                strncpy(cur_entry->textual_entry.string_value.old_text, cur_entry->textual_entry.string_value.text, cur_entry->textual_entry.string_value.buffer_length);

                // Receive new value to current_text buffer
                cur_entry->textual_entry.string_value.value_receive(cur_entry->textual_entry.string_value.text);

                if (strcmp(cur_entry->textual_entry.string_value.old_text, cur_entry->textual_entry.string_value.text)) {
                    // Strings are not equal
                    result = 1;
                }
            }
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT): {
            // Plain text cannot receive values
            result = 0;
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_CHOOSER): {
            if (cur_entry->textual_entry.chooser_value.value_receive) {
                uint8_t old_val = cur_entry->textual_entry.chooser_value.current_text;
                uint8_t received_val = cur_entry->textual_entry.chooser_value.value_receive();
                if (old_val != received_val) {
                    cur_entry->textual_entry.chooser_value.current_text = received_val;
                    cur_entry->textual_entry.chooser_value.old_text_ = old_val;
                    result = 1;
                }
            }
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_DINO_DINI_HALF_SIGMOID): {
            if (cur_entry->sigmoid_entry.value_receive != NULL) {
                float old_val = cur_entry->sigmoid_entry.k;
                float received_val = cur_entry->sigmoid_entry.value_receive();
                if (old_val != received_val) {
                    cur_entry->sigmoid_entry.k = received_val;
                    cur_entry->sigmoid_entry.old_k_ = old_val;
                    result = 1;
                }
            }
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_IMAGE): {
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_CIRCLE): {
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_DINO_DINI_FULL_SIGMOID): {
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_DIVIDER): {
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_BUTTON): {
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_INTEGER_RANGE): {
            if (cur_entry->textual_entry.int_range_value.value_receive) {
                // Copy current text to old_text
                long old_val = cur_entry->textual_entry.int_range_value.current_value;
                long received_val = cur_entry->textual_entry.int_range_value.value_receive();
                if (old_val != received_val) {
                    cur_entry->textual_entry.int_range_value.old_value_ = old_val;
                    cur_entry->textual_entry.int_range_value.current_value = received_val;
                    result = 1;
                }
            }
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_SUBMENU): {
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_BACK): {
            break;
        }
    }
    if (result) {
        if (menu_state != OLED_GFX_MENU_STATE_FULL_REDRAW_NEEDED) {
            menu_state |= OLED_GFX_MENU_STATE_ENTRY_CHANGED;
        }
        cur_entry->state_ = DISPLAY_GFX_MENU_ENTRY_STATE_VALUE_CHANGED;
    }
    return result;
}

/**
 * @brief
 *
 * @return The amount of refreshed menu entries
 */
unsigned char Display_gfx_menu::receive_all_entry_values() {
    unsigned char refreshed_values = 0;
    for (int i = 0; i < num_entries; i++) {
        if (receive_entry_value(i)) {
            refreshed_values++;
        }
    }
    return refreshed_values;
}

/**
 * @brief
 *
 * @param position
 */
void Display_gfx_menu::change_cursor_position(uint8_t position) {
    // Do nothing if cursor position didn't change
    if (current_entry_pos == position) {
        return;
    }

    display_gfx_menu_entry* selected_entry;
    int8_t direction = position > current_entry_pos ? 1 : -1;
    uint8_t temp_position = position - direction;
    bool check = true;

    // Iterate (in direction) over menu entries to get the next selectable menu entry
    do {
        if ((check && temp_position == 0 && direction == -1) || (check && temp_position == num_entries - 1 && direction == 1)) {
            check = true;
            break;
        }
        temp_position = temp_position + direction;
        selected_entry = entries[temp_position];
        check = (selected_entry->selectable == false);
    } while (check);

    if (!check) {
        old_entry_pos = current_entry_pos;
        current_entry_pos = temp_position;

        notify_changed_position();

        if (menu_state == OLED_GFX_MENU_STATE_IDLE) {
            menu_state |= OLED_GFX_MENU_STATE_POSITION_CHANGED;
        }
    }
};

/**
 * @brief
 *
 * @param display_gfx
 * @param entry
 * @param clear
 */
void Display_gfx_menu::draw_menu_entry(const struct display_adapter_descriptor* display_adapter, uint8_t entry, bool clear) const {
    // Get current menu entry
    display_gfx_menu_entry* menu_entry = entries[entry];
    display_gfx_menu_entry_type_t entry_type = menu_entry->entry_type;

    // Get font for special menu symbols (like arrows for submenu/back/...)
    GFXfont* font = (GFXfont*)&RobotoLight12p7b;

    // Calculate the positions where the menu item should be drawn to. This is dependent on the child class
    struct display_gfx_menu_coord coords = get_menu_entry_draw_position(entry);

    // Check if the entry is out-of-bounds and should not be drawn
    if (coords.x1 < 0 && coords.y1 < 0) {
        menu_entry->state_ = DISPLAY_GFX_MENU_ENTRY_STATE_IDLE;
        return;
    }

    // Fill header_text and header_text_length if menu_entry is a textual entry
    if (entry_type < 0x07) {
        // Textual Entries have a header text (the label text in the menu) and the actual value/text
        const char* header_text = menu_entry->textual_entry.header_text;
        uint8_t header_text_length = strlen(header_text);

        // Localization of Header Text
        if (locale_dict != NULL && menu_entry->textual_entry.locale_key > 0x00) {
            uint8_t locale_key = menu_entry->textual_entry.locale_key;
            const char* locale_text = locale_dict[locale_key];
            uint8_t locale_text_length = strlen(locale_text);
            header_text = locale_text;
            header_text_length = locale_text_length;
        }

        // Clear Header text
        if (clear) {
            uint8_t header_pixel_width = display_gfx_get_font_x_size(header_text, font_scale, textfont);
            display_gfx_draw_fill_rectangle(display_adapter,
                                            coords.x1,
                                            coords.y_alt,
                                            header_pixel_width,
                                            textfont->maxOverCursor,
                                            0);
        }

        display_gfx_draw_gfx_font(display_adapter,
                                  coords.x1,
                                  coords.y1,
                                  header_text,
                                  15,
                                  1,
                                  textfont);
    }

    switch (menu_entry->entry_type) {
        case DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_BACK: {
            display_gfx_draw_gfx_char(display_adapter,
                                      display_adapter->width - 5,  // This x coord only works with list_menu
                                      coords.y1,
                                      '^',
                                      15,
                                      1,
                                      font);
            break;
        }
        case DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_SUBMENU: {
            display_gfx_draw_gfx_font(display_adapter,
                                      display_adapter->width - 12,  // This x coord only works with list_menu
                                      coords.y1,
                                      "->",
                                      15,
                                      1,
                                      font);
            break;
        }
        case DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_CHOOSER: {
            uint8_t current_text_id = menu_entry->textual_entry.chooser_value.current_text;
            const char* current_text = menu_entry->textual_entry.chooser_value.text_array[current_text_id];
            size_t buffer_length = strlen(current_text) + 3;  // + 3 because two arrow characters and the null character are needed
            char buffer[buffer_length];
            snprintf(buffer, buffer_length, "<%s>", current_text);
            uint8_t text_pixel_width = display_gfx_get_font_x_size(buffer, 1, textfont);

            if (clear) {
                uint8_t old_text_id = menu_entry->textual_entry.chooser_value.old_text_;
                const char* old_text = menu_entry->textual_entry.chooser_value.text_array[old_text_id];
                size_t old_buffer_length = strlen(old_text) + 3;
                char old_buffer[old_buffer_length];
                snprintf(old_buffer, old_buffer_length, "<%s>", old_text);
                uint8_t old_text_pixel_width = display_gfx_get_font_x_size(old_buffer, 1, textfont);
                display_gfx_draw_fill_rectangle(display_adapter,
                                                display_adapter->width - old_text_pixel_width,  // This x coord only works with list_menu
                                                coords.y_alt,
                                                old_text_pixel_width,
                                                textfont->maxOverCursor + textfont->maxUnderCursor + 1,
                                                0);
            }

            display_gfx_draw_gfx_font(display_adapter,
                                      display_adapter->width - text_pixel_width,  // This x coord only works with list_menu
                                      coords.y2,
                                      buffer,
                                      15,
                                      1,
                                      textfont);

            break;
        }
        case DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_INTEGER_RANGE: {
            uint8_t buffer_length = 8;
            char text_buffer[buffer_length];
            sprintf(text_buffer, "[%ld]", menu_entry->textual_entry.int_range_value.current_value);
            uint8_t text_pixel_length = display_gfx_get_font_x_size(text_buffer, 1, textfont);

            if (clear) {
                char old_text[8];
                sprintf(old_text, "[%ld]", menu_entry->textual_entry.int_range_value.old_value_);
                uint8_t old_text_x_length = display_gfx_get_font_x_size(old_text, 1, textfont);
                display_gfx_draw_fill_rectangle(display_adapter,
                                                display_adapter->width - old_text_x_length,  // This x coord only works with list_menu
                                                coords.y_alt,
                                                old_text_x_length,
                                                textfont->maxOverCursor,
                                                0);
            }

            display_gfx_draw_gfx_font(display_adapter,
                                      display_adapter->width - text_pixel_length,  // This x coord only works with list_menu
                                      coords.y2,
                                      text_buffer,
                                      15,
                                      1,
                                      textfont);

            break;
        }
        case DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_SINGLE_VALUE: {
            const char* current_text = menu_entry->textual_entry.string_value.text;
            uint8_t text_x_length = display_gfx_get_font_x_size(current_text, 1, textfont);

            if (clear) {
                const char* old_text = menu_entry->textual_entry.string_value.old_text;
                uint8_t old_text_x_length = display_gfx_get_font_x_size(old_text, 1, textfont);
                display_gfx_draw_fill_rectangle(display_adapter,
                                                display_adapter->width - old_text_x_length,  // This x coord only works with list_menu
                                                coords.y_alt,
                                                old_text_x_length,
                                                textfont->maxOverCursor + textfont->maxUnderCursor + 1,
                                                0);
            }

            display_gfx_draw_gfx_font(display_adapter,
                                      display_adapter->width - text_x_length,  // This x coord only works with list_menu
                                      coords.y2,
                                      current_text,
                                      15,
                                      1,
                                      textfont);

            break;
        }

        case DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_BOX: {
            const char* text = menu_entry->textual_entry.header_text;
            uint8_t text_length = strlen(text);

            // Localization of Header Text
            if (locale_dict != NULL && menu_entry->textual_entry.locale_key > 0x00) {
                uint8_t locale_key = menu_entry->textual_entry.locale_key;
                const char* locale_text = locale_dict[locale_key];
                uint8_t locale_text_length = strlen(locale_text);
                text = locale_text;
                text_length = locale_text_length;
            }

            uint8_t line = 0;
            uint8_t x_delta = menu_entry->x_end - menu_entry->x_start;
            const char* front_ptr = text;
            const char* rear_ptr = text + text_length;

            while (rear_ptr > front_ptr) {
                unsigned short x_len = display_gfx_get_font_x_size_len(front_ptr,
                                                                       rear_ptr - front_ptr,
                                                                       1,
                                                                       textfont);

                if (x_len > x_delta) {
                    // Text is too long to draw into line, move rear ptr left
                    short last_occ = string_last_occurence(front_ptr, ' ', rear_ptr - front_ptr);
                    rear_ptr = front_ptr + last_occ;
                } else {
                    // Text fits into line, draw and move front_ptr_right
                    uint8_t byte_len = rear_ptr - front_ptr;
                    display_gfx_draw_gfx_font_len(display_adapter,
                                                  coords.x1,
                                                  coords.y1 + (line + 1) * textfont->yAdvance,
                                                  front_ptr,
                                                  byte_len,
                                                  15,
                                                  1,
                                                  textfont);

                    front_ptr = rear_ptr + 1;

                    // Reset rear_ptr
                    rear_ptr = text + text_length;
                    line++;
                }
            }

            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_DIVIDER): {
            display_gfx_draw_line(display_adapter,
                                  coords.x1,
                                  coords.y1,
                                  coords.x2,
                                  coords.y2,
                                  15);

            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_DINO_DINI_HALF_SIGMOID): {
            if (clear) {
                display_gfx_draw_half_dino_dini_sigmoid(display_adapter,
                                                        coords.x1,
                                                        coords.y1,
                                                        menu_entry->sigmoid_entry.old_k_,
                                                        coords.x2,
                                                        coords.y2,
                                                        0);
            }

            display_gfx_draw_half_dino_dini_sigmoid(display_adapter,
                                                    coords.x1,
                                                    coords.y1,
                                                    menu_entry->sigmoid_entry.k,
                                                    coords.x2,
                                                    coords.y2,
                                                    15);
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_CIRCLE): {
            if (clear) {
                display_gfx_draw_circle(display_adapter,
                                        coords.x_alt,
                                        coords.y_alt,
                                        menu_entry->circle_entry.radius,
                                        0);
            }
            display_gfx_draw_circle(display_adapter,
                                    coords.x1,
                                    coords.y1,
                                    menu_entry->circle_entry.radius,
                                    15);

            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_GAUGE): {
            if (clear) {
                display_gfx_draw_fill_rectangle(display_adapter,
                                                coords.x1 - menu_entry->gauge_entry.radius,
                                                coords.y1 - menu_entry->gauge_entry.radius,
                                                menu_entry->gauge_entry.radius * 2,
                                                menu_entry->gauge_entry.radius * 2,
                                                0);
            }

            float angle;
            uint16_t gauge_value = menu_entry->gauge_entry.gauge_value;
            if (menu_entry->gauge_entry.range != 270) {
                angle = 225.0F - (270 * ((float)gauge_value / menu_entry->gauge_entry.range));
            } else {
                angle = 225.0F - gauge_value;
            }

            display_gfx_draw_gauge(display_adapter,
                                   coords.x1,
                                   coords.y1,
                                   menu_entry->gauge_entry.radius,
                                   (uint8_t)angle,
                                   15);

            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT): {
            // This is already handled at the very top
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_DYNAMIC): {
            if (clear) {
                uint8_t old_text_pixel_width = display_gfx_get_font_x_size(menu_entry->textual_entry.string_value.old_text, font_scale, textfont);
                display_gfx_draw_fill_rectangle(display_adapter,
                                                coords.x1,
                                                coords.y_alt,
                                                old_text_pixel_width,
                                                textfont->maxOverCursor,
                                                0);
            }

            display_gfx_draw_gfx_font(display_adapter,
                                      coords.x1,
                                      coords.y1,
                                      menu_entry->textual_entry.string_value.text,
                                      15,
                                      1,
                                      textfont);
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_IMAGE): {
            display_gfx_draw_image_grayscale(display_adapter,
                                             coords.x1,
                                             coords.y1,
                                             menu_entry->x_end,
                                             menu_entry->y_end,
                                             (uint8_t*)menu_entry->image_entry.pixel_data,
                                             menu_entry->image_entry.img_format);
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_BUTTON): {
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_DINO_DINI_FULL_SIGMOID): {
            break;
        }
    }

    // Now the entry is drawn -> turn its state back to idle
    menu_entry->state_ = DISPLAY_GFX_MENU_ENTRY_STATE_IDLE;
}

unsigned char Display_gfx_menu::handle_frame() {
    unsigned char result = 0;
    handle_frame_all_entries();

    result = receive_new_position_all_entries();

    result += receive_all_entry_values();
    return result;
}

unsigned char Display_gfx_menu::handle_frame_entry(uint8_t entry) {
    unsigned char result = 0;
    display_gfx_menu_entry* cur_entry = entries[entry];
    if (cur_entry->handle_frame) {
        display_gfx_menu_entry temp = *cur_entry;
        result = cur_entry->handle_frame(&temp);
    }

    return result;
}

unsigned char Display_gfx_menu::handle_frame_all_entries() {
    unsigned char changed_entries = 0;
    for (int i = 0; i < num_entries; i++) {
        if (handle_frame_entry(i)) {
            changed_entries++;
        }
    }
    return changed_entries;
}

unsigned char Display_gfx_menu::receive_new_position_entry(uint8_t entry) {
    unsigned char result = 0;
    display_gfx_menu_entry* cur_entry = entries[entry];

    switch (cur_entry->entry_type) {
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_CIRCLE): {
            if (cur_entry->receive_new_position) {
                uint8_t x_temp = cur_entry->x_start;
                uint8_t y_temp = cur_entry->y_start;
                cur_entry->receive_new_position(&x_temp, &y_temp);

                if (x_temp != cur_entry->x_start || y_temp != cur_entry->y_start) {
                    // Value got changed
                    result = 1;
                    cur_entry->circle_entry.x_center_old_ = cur_entry->x_start;
                    cur_entry->circle_entry.y_center_old_ = cur_entry->y_start;
                    cur_entry->x_start = x_temp;
                    cur_entry->y_start = y_temp;
                }
            }
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_SUBMENU): {
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT): {
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_BUTTON): {
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_DYNAMIC): {
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_CHOOSER): {
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_GAUGE): {
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_SINGLE_VALUE): {
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_INTEGER_RANGE): {
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_IMAGE): {
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_BACK): {
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_DINO_DINI_HALF_SIGMOID): {
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_DINO_DINI_FULL_SIGMOID): {
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_DIVIDER): {
            break;
        }
    }

    if (result) {
        menu_state |= OLED_GFX_MENU_STATE_FULL_REDRAW_NEEDED;
        cur_entry->state_ = DISPLAY_GFX_MENU_ENTRY_STATE_VALUE_CHANGED;
    }
    // TODO: Since we cannot guarantee that the new position intersects with other entries we need to redraw the whole menu; Could be fixed e.g. with dependencies or calculating intersections
    return result;
}

unsigned char Display_gfx_menu::receive_new_position_all_entries() {
    unsigned char changed_entries = 0;
    for (int i = 0; i < num_entries; i++) {
        if (receive_new_position_entry(i)) {
            changed_entries++;
        }
    }
    return changed_entries;
}

short Display_gfx_menu::string_last_occurence(const char* str, char cha, size_t len) const {
    short result = -1;

    size_t str_len = strlen(str);
    if (len < str_len) {
        str_len = len;
    }

    for (unsigned char i = 0; i < len; i++) {
        if (str[i] == cha) {
            result = i;
        }
    }
    return result;
}
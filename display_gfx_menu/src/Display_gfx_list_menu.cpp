#include "Display_gfx_list_menu.hpp"

// Standard libs
#include <stdio.h>
#include <string.h>

Display_gfx_list_menu::Display_gfx_list_menu(uint8_t menu_id, display_gfx_menu_entry **entries, uint8_t num_entries, uint8_t scroll_items_count, const GFXfont *textfont, uint8_t f_scale) : Display_gfx_menu(menu_id, OLED_GFX_MENU_TYPE_LIST, entries, num_entries, textfont, f_scale, 0), scroll_position(0), scroll_items(scroll_items_count){};

Display_gfx_list_menu::Display_gfx_list_menu(uint8_t menu_id, display_gfx_menu_entry **entries, uint8_t num_entries, uint8_t scroll_items_count, const GFXfont *textfont, uint8_t f_scale, const char **locale_dict) : Display_gfx_menu(menu_id, OLED_GFX_MENU_TYPE_LIST, entries, num_entries, textfont, f_scale, locale_dict), scroll_position(0), scroll_items(scroll_items_count){};

void Display_gfx_list_menu::draw_position_cursor(const struct display_adapter_descriptor *display_adapter) {
    if (old_entry_pos != current_entry_pos) {
        display_gfx_draw_gfx_char(display_adapter,
                                  0,
                                  DISPLAY_GFX_LIST_MENU_UPPER_PADDING + (old_entry_pos + 1 - scroll_position) * textfont->yAdvance,
                                  '>',
                                  0,
                                  1,
                                  textfont);
    }

    display_gfx_draw_gfx_char(display_adapter,
                              0,
                              DISPLAY_GFX_LIST_MENU_UPPER_PADDING + (current_entry_pos + 1 - scroll_position) * textfont->yAdvance,
                              '>',
                              15,
                              1,
                              textfont);
};

display_gfx_menu_coord Display_gfx_list_menu::get_menu_entry_draw_position(uint8_t entry) const {
    struct display_gfx_menu_coord result;

    // Get current menu entry
    display_gfx_menu_entry *menu_entry = entries[entry];
    display_gfx_menu_entry_type_t entry_type = menu_entry->entry_type;

    // Check if item is out-of-bounds
    if (entry > scroll_position + scroll_items - 1 || entry < scroll_position) {
        result.x1 = -1;
        result.y1 = -1;
        return result;
    }

    switch (entry_type) {
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_INTEGER_RANGE): {
            // Header Text
            result.x1 = DISPLAY_GFX_LIST_MENU_START_X;
            result.y1 = DISPLAY_GFX_LIST_MENU_UPPER_PADDING + (entry + 1 - scroll_position) * textfont->yAdvance;

            // Value
            // result.x2 = display_gfx->get_adapter()->get_display_width() - text_pixel_length;
            result.y2 = DISPLAY_GFX_LIST_MENU_UPPER_PADDING + (entry + 1 - scroll_position) * textfont->yAdvance;

            // Clear (with Rectangle)
            result.y_alt = DISPLAY_GFX_LIST_MENU_UPPER_PADDING + (entry + 1 - scroll_position) * textfont->yAdvance - textfont->maxOverCursor;
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_SINGLE_VALUE): {
            // Header Text
            result.x1 = DISPLAY_GFX_LIST_MENU_START_X;
            result.y1 = DISPLAY_GFX_LIST_MENU_UPPER_PADDING + (entry + 1 - scroll_position) * textfont->yAdvance;

            // Value
            result.y2 = DISPLAY_GFX_LIST_MENU_UPPER_PADDING + (entry + 1 - scroll_position) * textfont->yAdvance;

            // Clear (with Rectangle)
            result.y_alt = DISPLAY_GFX_LIST_MENU_UPPER_PADDING + (entry + 1 - scroll_position) * textfont->yAdvance - textfont->maxOverCursor;
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_CHOOSER): {
            // Header Text
            result.x1 = DISPLAY_GFX_LIST_MENU_START_X;
            result.y1 = DISPLAY_GFX_LIST_MENU_UPPER_PADDING + (entry + 1 - scroll_position) * textfont->yAdvance;

            // Value
            result.y2 = DISPLAY_GFX_LIST_MENU_UPPER_PADDING + (entry + 1 - scroll_position) * textfont->yAdvance;

            // Clear (with Rectangle)
            result.y_alt = DISPLAY_GFX_LIST_MENU_UPPER_PADDING + (entry + 1 - scroll_position) * textfont->yAdvance - textfont->maxOverCursor;
            break;
        }

        default: {
            result.x1 = DISPLAY_GFX_LIST_MENU_START_X;
            result.y1 = DISPLAY_GFX_LIST_MENU_UPPER_PADDING + (entry + 1 - scroll_position) * textfont->yAdvance;
        }
    }

    return result;
};

void Display_gfx_list_menu::draw_menu_decor(const struct display_adapter_descriptor *display_adapter) const {
    // no op
    return;
};

void Display_gfx_list_menu::notify_changed_position() {
    if (current_entry_pos > scroll_position + scroll_items - 1) {
        scroll_position = current_entry_pos - (scroll_items - 1);
        menu_state = OLED_GFX_MENU_STATE_FULL_REDRAW_NEEDED;
        return;
    };

    if (current_entry_pos < scroll_position) {
        scroll_position = current_entry_pos;
        menu_state = OLED_GFX_MENU_STATE_FULL_REDRAW_NEEDED;
        return;
    }

    return;
};
#include "Display_gfx_free_menu.hpp"

// C/C++ Headers
#include <math.h>
#include <stdio.h>

// Zephyr Header
#include "logging/log.h"

Display_gfx_free_menu::Display_gfx_free_menu(uint8_t menu_id,
                                             display_gfx_menu_entry **entries,
                                             uint8_t num_entries,
                                             const GFXfont *textfont,
                                             uint8_t f_scale)
    : Display_gfx_menu(menu_id, OLED_GFX_MENU_TYPE_FREE, entries, num_entries,
                       textfont, f_scale){};

Display_gfx_free_menu::Display_gfx_free_menu(uint8_t menu_id,
                                             display_gfx_menu_entry **entries,
                                             uint8_t num_entries,
                                             const GFXfont *textfont,
                                             uint8_t f_scale,
                                             const char **locale_dict)
    : Display_gfx_menu(menu_id, OLED_GFX_MENU_TYPE_FREE, entries, num_entries,
                       textfont, f_scale, locale_dict){};

Display_gfx_free_menu::Display_gfx_free_menu(uint8_t menu_id,
                                             display_gfx_menu_entry **entries,
                                             uint8_t num_entries,
                                             uint8_t current_entry,
                                             const GFXfont *textfont,
                                             uint8_t f_scale,
                                             const char **locale_dict)
    : Display_gfx_menu(menu_id, OLED_GFX_MENU_TYPE_FREE, entries, num_entries, current_entry, textfont, f_scale, locale_dict){};

/**
 * @brief
 *
 * @param display_adapter
 */
void Display_gfx_free_menu::draw_position_cursor(const struct display_adapter_descriptor *display_adapter) {
    display_gfx_menu_entry *cur_entry = entries[current_entry_pos];

    if (old_entry_pos != current_entry_pos) {
        display_gfx_menu_entry *old_entry = entries[old_entry_pos];

        uint8_t old_cursor_x = old_entry->x_start - 10;
        uint8_t old_cursor_y = old_entry->y_start;

        display_gfx_draw_gfx_char(display_adapter, old_cursor_x, old_cursor_y,
                                  '>', 0, font_scale, textfont);
    }

    uint8_t cursor_x = cur_entry->x_start - 10;
    uint8_t cursor_y = cur_entry->y_start;

    display_gfx_draw_gfx_char(display_adapter,
                              cursor_x,
                              cursor_y,
                              '>',
                              15,
                              font_scale,
                              textfont);
};

display_gfx_menu_coord Display_gfx_free_menu::get_menu_entry_draw_position(uint8_t entry) const {
    struct display_gfx_menu_coord result;

    // Get current menu entry
    display_gfx_menu_entry *menu_entry = entries[entry];
    display_gfx_menu_entry_type_t entry_type = menu_entry->entry_type;

    switch (entry_type) {
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_DIVIDER): {
            result.x1 = menu_entry->x_start;
            result.y1 = menu_entry->y_start;
            result.x2 = menu_entry->x_end;
            result.y2 = menu_entry->y_end;
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_DINO_DINI_HALF_SIGMOID): {
            result.x1 = menu_entry->x_start;
            result.y1 = menu_entry->y_start;
            result.x2 = menu_entry->x_end;
            result.y2 = menu_entry->y_end;
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_CIRCLE): {
            result.x1 = menu_entry->x_start;
            result.y1 = menu_entry->y_start;

            result.x_alt = menu_entry->circle_entry.x_center_old_;
            result.y_alt = menu_entry->circle_entry.y_center_old_;
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_GAUGE): {
            result.x1 = menu_entry->x_start;
            result.y1 = menu_entry->y_start;
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_INTEGER_RANGE): {
            result.x1 = menu_entry->x_start;
            result.y1 = menu_entry->y_start;
            result.y2 = menu_entry->y_start;

            // Clear
            result.y_alt = menu_entry->y_start - textfont->maxOverCursor;
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT): {
            result.x1 = menu_entry->x_start;
            result.y1 = menu_entry->y_start;

            // Clear (with rectangle)
            result.y_alt = menu_entry->y_start - textfont->maxOverCursor;
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_DYNAMIC): {
            result.x1 = menu_entry->x_start;
            result.y1 = menu_entry->y_start;

            // Clear (with rectangle)
            result.y_alt = menu_entry->y_start - textfont->maxOverCursor;
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_IMAGE): {
            result.x1 = menu_entry->x_start;
            result.y1 = menu_entry->y_start;
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_BACK): {
            result.x1 = menu_entry->x_start;
            result.y1 = menu_entry->y_start;
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_CHOOSER): {
            result.x1 = menu_entry->x_start;
            result.y1 = menu_entry->y_start;

            // Value
            result.y2 = menu_entry->y_start;

            // Clear (with Rectangle)
            result.y_alt = menu_entry->y_start - textfont->maxOverCursor;
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_SINGLE_VALUE): {
            result.x1 = menu_entry->x_start;
            result.y1 = menu_entry->y_start;

            // Value
            result.y2 = menu_entry->y_start;

            // Clear (with Rectangle)
            result.y_alt = menu_entry->y_start - textfont->maxOverCursor;
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_BUTTON): {
            result.x1 = menu_entry->x_start;
            result.y1 = menu_entry->y_start;
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_DINO_DINI_FULL_SIGMOID): {
            break;
        }

        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_SUBMENU): {
            break;
        }
        case DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_BOX: {
            result.x1 = menu_entry->x_start;
            result.y1 = menu_entry->y_start;

            // Clear (with Rectangle)
            result.x_alt = menu_entry->x_end;
            result.y_alt = menu_entry->y_end;
            break;
        }
    }

    return result;
};

void Display_gfx_free_menu::draw_menu_decor(
    const struct display_adapter_descriptor *display_adapter) const {
    // no op
    return;
};

void Display_gfx_free_menu::notify_changed_position() {
    return;
};

#pragma once

#include "Display_gfx_menu.hpp"

#define DISPLAY_GFX_LIST_MENU_START_X 9U
#define DISPLAY_GFX_LIST_MENU_UPPER_PADDING 5U

class Display_gfx_list_menu : public Display_gfx_menu {
   public:
    Display_gfx_list_menu(uint8_t menu_id, display_gfx_menu_entry **entries, uint8_t num_entries, uint8_t scroll_items_count, const GFXfont *textfont, uint8_t f_scale);

    Display_gfx_list_menu(uint8_t menu_id, display_gfx_menu_entry **entries, uint8_t num_entries, uint8_t scroll_items_count, const GFXfont *textfont, uint8_t f_scale, const char **locale_dict);

   private:
    // Current amount of items that have already been scrolled
    uint8_t scroll_position;

    // Amount of items that can be on the display without needing to scroll
    const uint8_t scroll_items;

    void draw_full_menu(const struct display_adapter_descriptor *display_adapter) const;

    void draw_position_cursor(const struct display_adapter_descriptor *display_adapter);

    display_gfx_menu_coord get_menu_entry_draw_position(uint8_t entry) const;

    void draw_menu_decor(const struct display_adapter_descriptor *display_adapter) const;

    void notify_changed_position();

   protected:
};
#pragma once

#include "Display_gfx_menu.hpp"

class Display_gfx_free_menu : public Display_gfx_menu {
   public:
    Display_gfx_free_menu(uint8_t menu_id, display_gfx_menu_entry **entries, uint8_t num_entries, const GFXfont *textfont, uint8_t f_scale);

    Display_gfx_free_menu(uint8_t menu_id, display_gfx_menu_entry **entries, uint8_t num_entries, const GFXfont *textfont, uint8_t f_scale, const char **locale_dict);

    Display_gfx_free_menu(uint8_t menu_id, display_gfx_menu_entry **entries, uint8_t num_entries, uint8_t current_entry, const GFXfont *textfont, uint8_t f_scale, const char **locale_dict);

   private:
    void draw_position_cursor(const struct display_adapter_descriptor *display_adapter);

    display_gfx_menu_coord get_menu_entry_draw_position(uint8_t entry) const;

    void draw_menu_decor(const struct display_adapter_descriptor *display_adapter) const;

    void notify_changed_position();

   protected:
};
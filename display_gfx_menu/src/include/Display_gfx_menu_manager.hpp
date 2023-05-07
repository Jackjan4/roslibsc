#pragma once

#include "Display_gfx_menu.hpp"
#include "display_gfx.h"

class Display_gfx_menu_manager {
   public:
    Display_gfx_menu_manager(const struct display_adapter_descriptor* adapter, Display_gfx_menu** menus, uint8_t menu_count);

    Display_gfx_menu_manager(const struct display_adapter_descriptor* adapter, Display_gfx_menu** menus, uint8_t menu_count, uint8_t current_menu_id);

    unsigned char draw_current_menu();

    void select_entry();

    void select_left();

    void select_right();

    void change_cursor_position(uint8_t position);

    void increment_cursor_position();

    void decrement_cursor_position();

    inline Display_gfx_menu* get_current_menu() {
        return menus[current_menu_pos];
    };

    unsigned char frame_handler();

    void change_menu(uint8_t menu_id);

    void apply_language_to_all_menus(const char** lang);

   private:
    const struct display_adapter_descriptor* display_adapter;
    Display_gfx_menu** menus;
    uint8_t menu_count;

    // The position of the current menu in the 'menus' array (not the menu id)
    uint8_t current_menu_pos;

    /**
     * @brief Get the menu position in the 'menus' array by its menu_id
     *
     * @param menu_id
     * @return Display_gfx_menu*
     */
    int8_t get_menu_pos_by_id(uint8_t menu_id);

   protected:
};
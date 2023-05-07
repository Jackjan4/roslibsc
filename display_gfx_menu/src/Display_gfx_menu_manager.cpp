#include "Display_gfx_menu_manager.hpp"

#include "Display_gfx_menu_entry.h"

Display_gfx_menu_manager::Display_gfx_menu_manager(const struct display_adapter_descriptor* adapter, Display_gfx_menu** menus, uint8_t menu_count) : display_adapter(adapter), menus(menus), menu_count(menu_count), current_menu_pos(0){};

Display_gfx_menu_manager::Display_gfx_menu_manager(const struct display_adapter_descriptor* adapter, Display_gfx_menu** menus, uint8_t menu_count, uint8_t current_menu_id) : display_adapter(adapter), menus(menus), menu_count(menu_count) {
    current_menu_pos = get_menu_pos_by_id(current_menu_id);
};

unsigned char Display_gfx_menu_manager::draw_current_menu() {
    Display_gfx_menu* menu = menus[current_menu_pos];
    return menu->draw_menu(display_adapter);
}

void Display_gfx_menu_manager::select_entry() {
    Display_gfx_menu* menu = menus[current_menu_pos];
    menu->select_entry();
    display_gfx_menu_entry* current_menu_entry = menu->get_current_entry();

    switch (current_menu_entry->entry_type) {
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_SUBMENU): {
            uint8_t redirect_menu_id = current_menu_entry->textual_entry.redirect_value.redirect_menu_id;
            change_menu(redirect_menu_id);
            break;
        }
        case (DISPLAY_GFX_MENU_ENTRY_TYPE_TEXT_BACK): {
            uint8_t redirect_menu_id = current_menu_entry->textual_entry.redirect_value.redirect_menu_id;
            change_menu(redirect_menu_id);
            break;
        }
        default: {
            
        }
    }
}

void Display_gfx_menu_manager::select_left() {
    Display_gfx_menu* menu = menus[current_menu_pos];

    menu->select_left();
}

void Display_gfx_menu_manager::increment_cursor_position() {
    Display_gfx_menu* menu = menus[current_menu_pos];
    menu->inc_cursor_position();
}

void Display_gfx_menu_manager::decrement_cursor_position() {
    Display_gfx_menu* menu = menus[current_menu_pos];
    menu->dec_cursor_position();
}

void Display_gfx_menu_manager::select_right() {
    Display_gfx_menu* menu = menus[current_menu_pos];
    menu->select_right();
}

void Display_gfx_menu_manager::change_cursor_position(uint8_t position) {
    Display_gfx_menu* menu = menus[current_menu_pos];

    menu->change_cursor_position(position);
}

unsigned char Display_gfx_menu_manager::frame_handler() {
    Display_gfx_menu* menu = menus[current_menu_pos];

    return menu->handle_frame();
}

int8_t Display_gfx_menu_manager::get_menu_pos_by_id(uint8_t menu_id) {
    int8_t result = -1;

    for (uint8_t i = 0; i < menu_count; i++) {
        if (menus[i]->get_menu_id() == menu_id) {
            result = i;
            break;
        }
    }
    return result;
};

void Display_gfx_menu_manager::change_menu(uint8_t menu_id) {
    // Change to new menu
    this->current_menu_pos = get_menu_pos_by_id(menu_id);

    // Set the new menu to FULL_REDRAW_NEEDED
    menus[current_menu_pos]->set_menu_state(OLED_GFX_MENU_STATE_FULL_REDRAW_NEEDED);
}

void Display_gfx_menu_manager::apply_language_to_all_menus(const char** lang) {
    for (uint8_t i = 0; i < menu_count; i++) {
        menus[i]->set_locale_dict(lang);
    }
}
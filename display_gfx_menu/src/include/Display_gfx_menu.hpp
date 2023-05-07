#pragma once

// Library Headers
#include "Display_gfx_menu_entry.h"
#include "display_gfx.h"

typedef uint8_t display_gfx_menu_state_t;

const display_gfx_menu_state_t OLED_GFX_MENU_STATE_IDLE = 0x00;
const display_gfx_menu_state_t OLED_GFX_MENU_STATE_POSITION_CHANGED = 0x01;
const display_gfx_menu_state_t OLED_GFX_MENU_STATE_ENTRY_CHANGED = 0x02;
const display_gfx_menu_state_t OLED_GFX_MENU_STATE_FULL_REDRAW_NEEDED = 0x80;

enum display_gfx_menu_type_t {
    OLED_GFX_MENU_TYPE_LIST = 0x00,
    OLED_GFX_MENU_TYPE_FREE = 0x01
};

struct display_gfx_menu_coord {
    // These values are signed because the menus can signal that an entry shouldn't be drawn because it is out-of-bounds by making the coords negative
    int8_t x1;  // a.k.a x_start
    int8_t y1;  // a.k.a y_start
    int8_t x2;  // a.k.a x_end
    int8_t y2;  // a.k.a y_end
    int8_t x_alt;
    int8_t y_alt;
};

class Display_gfx_menu {
   public:
    Display_gfx_menu(uint8_t menu_id, display_gfx_menu_type_t menu_type, display_gfx_menu_entry **entries, uint8_t num_entries, uint8_t current_position, const GFXfont *font, uint8_t f_scale);

    Display_gfx_menu(uint8_t menu_id, display_gfx_menu_type_t menu_type, display_gfx_menu_entry **entries, uint8_t num_entries, const GFXfont *font, uint8_t f_scale);

    Display_gfx_menu(uint8_t menu_id, display_gfx_menu_type_t menu_type, display_gfx_menu_entry **entries, uint8_t num_entries, uint8_t current_position, const GFXfont *font, uint8_t f_scale, const char **locale_dict);

    Display_gfx_menu(uint8_t menu_id, display_gfx_menu_type_t menu_type, display_gfx_menu_entry **entries, uint8_t num_entries, const GFXfont *font, uint8_t f_scale, const char **locale_dict);

    /**
     * @brief Draws the menu to the display buffer. This method checks which parts of the menu need to be redrawn and draws only these accordingly.
     *
     * @param display_gfx
     */
    unsigned char draw_menu(const struct display_adapter_descriptor *display_adapter);

    /**
     * @brief Changes the cursor position to the given value and sets the menu state to redraw the cursor position next time
     *
     * @param position
     */
    void change_cursor_position(uint8_t position);

    /**
     * @brief Increments the cursor position and sets the menu state to redraw the cursor position next time
     *
     */
    inline void inc_cursor_position() {
        // Note: The limit of 255 must be adapted in case current_entry_pos changes its possible size
        change_cursor_position(current_entry_pos < 255 ? current_entry_pos + 1 : 0);
    }

    /**
     * @brief Decrements the cursor position and sets the menu state to redraw the cursor position next time
     *
     */
    inline void dec_cursor_position() {
        change_cursor_position(current_entry_pos > 0 ? current_entry_pos - 1 : 0);
    }

    /**
     * @brief Returns the current menu_entry object
     *
     * @return Display_gfx_menu_entry*
     */
    inline display_gfx_menu_entry *get_current_entry() {
        return entries[current_entry_pos];
    }

    inline void set_menu_state(display_gfx_menu_state_t state) {
        menu_state = state;
    }

    /**
     * @brief
     *
     */
    void select_entry();

    void select_left();

    void select_right();

    unsigned char receive_entry_value(uint8_t entry);

    unsigned char handle_frame_entry(uint8_t entry);

    unsigned char handle_frame_all_entries();

    unsigned char handle_frame();

    unsigned char receive_new_position_entry(uint8_t entry);

    unsigned char receive_new_position_all_entries();

    inline uint8_t get_menu_id() {
        return menu_id;
    }

    inline void set_locale_dict(const char** dict) {
        locale_dict = dict;
    }

   private:
    /**
     * @brief Forces a full redraw of the menu
     *
     * @param display_gfx
     */
    void draw_full_menu(const struct display_adapter_descriptor *display_adapter);

    /**
     * @brief Draws a single menu_entry onto the display_buffer.
     *
     * @param display_gfx
     * @param entry The position of the menu entry inside the array of menu entries
     * @param display_line The display line/rank to which the entry should be drawn onto
     * @param clear States if the former value (if this entry has values) should be cleared
     */
    void draw_menu_entry(const struct display_adapter_descriptor *display_adapter, uint8_t entry, bool clear) const;

    unsigned char receive_all_entry_values();

    short string_last_occurence(const char* str, char cha, size_t len) const;

   protected:
    const uint8_t menu_id;
    const display_gfx_menu_type_t menu_type;
    display_gfx_menu_entry **entries;
    uint8_t num_entries;
    const GFXfont *textfont;
    const uint8_t font_scale;
    const char **locale_dict; // "a normal pointer to a const char**"

    uint8_t current_entry_pos;
    uint8_t old_entry_pos;
    display_gfx_menu_state_t menu_state;

    /**
     * @brief Draws the position cursor infront of the currently selected menu entry
     *
     * @param display_gfx
     */
    virtual void draw_position_cursor(const struct display_adapter_descriptor *display_adapter) = 0;

    virtual display_gfx_menu_coord get_menu_entry_draw_position(uint8_t entry) const = 0;

    /**
     * @brief
     *
     * @param display_adapter
     */
    virtual void draw_menu_decor(const struct display_adapter_descriptor *display_adapter) const = 0;

    virtual void notify_changed_position() = 0;
};
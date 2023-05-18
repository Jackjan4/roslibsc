#pragma once



// Include tweaks
#if __has_include(<display_gfx.tweaks.h>)
  #include <display_gfx.tweaks.h>
#endif



// Standard libs
#include <stddef.h> // size_t

// Application libs
#include "display_adapter.h"

// Adafruit GFX fonts
#include "gfxfont.h"

#ifdef __cplusplus
extern "C" {
#endif

void display_gfx_draw_pixel(const struct display_adapter_descriptor *display_adapter, uint8_t x, uint8_t y, uint8_t value);

void display_gfx_draw_hline(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t length, uint8_t value);

void display_gfx_draw_vline(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t length, uint8_t value);

void display_gfx_draw_line(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end, uint8_t value);

void display_gfx_draw_rectangle(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t x_length, uint8_t y_length, uint8_t value);

void display_gfx_draw_fill_rectangle(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t x_length, uint8_t y_length, uint8_t value);

void display_gfx_draw_circle(const struct display_adapter_descriptor *display_adapter, uint8_t x_center, uint8_t y_center, uint8_t r, uint8_t value);

void display_gfx_draw_gauge(const struct display_adapter_descriptor *display_adapter, uint8_t x_center, uint8_t y_center, uint8_t r, uint16_t angle, uint8_t value);

void display_gfx_draw_gfx_font_len(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, const char *text, size_t len, uint8_t value, uint8_t scale_factor, const GFXfont *font);

void display_gfx_draw_gfx_font(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, const char *text, uint8_t value, uint8_t scale_factor, const GFXfont *font);

void display_gfx_draw_gfx_char(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t cha, uint8_t value, uint8_t scale_factor, const GFXfont *font);

void display_gfx_draw_invert(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end);

void display_gfx_draw_half_dino_dini_sigmoid(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, float k, uint8_t x_end, uint8_t y_end, uint8_t value);

void display_gfx_draw_full_dino_dini_sigmoid(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, float k, uint8_t x_end, uint8_t y_end, uint8_t value);

void display_gfx_draw_image_grayscale(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t width, uint8_t height, const uint8_t *pixel_data, uint8_t pixels_per_byte);

unsigned short display_gfx_get_font_x_size_len(const char *text, size_t len, uint8_t scale_factor, const GFXfont *font);

unsigned short display_gfx_get_font_x_size(const char *text, uint8_t scale_factor, const GFXfont *font);

#ifdef __cplusplus
}
#endif
#include "display_gfx.h"

// C Headers
#include <math.h>
#include <stdlib.h>

// Zephyr Headers
#ifdef __ZEPHYR__
#ifdef CONFIG_LOG
#include <zephyr/logging/log.h>
#endif

#ifdef CONFIG_LOG
LOG_MODULE_REGISTER(oled_gfx);
#endif
#endif



static float display_gfx_calculate_dino_dini_sigmoid(float x, float k) {
    float dividend = x - k * x;

    float divisor = k - 2 * k * fabs(x) + 1;

    float result = dividend / divisor;

    return result;
}



void display_gfx_draw_pixel(const struct display_adapter_descriptor *display_adapter, uint8_t x, uint8_t y, uint8_t value) {
    // TODO: Check for pixel out of bounds

    display_adapter_set_pixel_in_buffer(display_adapter, x, y, value);
}



void display_gfx_draw_hline(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t length, uint8_t value) {
    // TODO: Check for display bounds
    for (int i = 0; i < length; i++) {
        display_gfx_draw_pixel(display_adapter, x_start + i, y_start, value);
    }
}



void display_gfx_draw_vline(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t length, uint8_t value) {
    // TODO: Check for display bounds
    for (int i = 0; i < length; i++) {
        display_gfx_draw_pixel(display_adapter, x_start, y_start + i, value);
    }
}



void display_gfx_draw_line(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end, uint8_t value) {
    if (y_start == y_end) {
        display_gfx_draw_hline(display_adapter, x_start < x_end ? x_start : x_end,
                               y_start < y_end ? y_start : y_end,
                               x_end > x_start ? x_end - x_start : x_start - x_end,
                               value);
        return;
    }

    if (x_start == x_end) {
        display_gfx_draw_vline(display_adapter, x_start < x_end ? x_start : x_end,
                               y_start < y_end ? y_start : y_end,
                               y_end > y_start ? y_end - y_start : y_start - y_end,
                               value);
        return;
    }

    // Bresenham's Algorithm for drawing lines - an explanation can be seen here: https://de.wikipedia.org/wiki/Bresenham-Algorithmus
    int delta_x = abs(x_end - x_start);
    int x_direction = x_start < x_end ? 1 : -1;
    int delta_y = -abs(y_end - y_start);
    int y_direction = y_start < y_end ? 1 : -1;
    int err = delta_x + delta_y;
    int e2; /* error value e_xy */

    while (1) {
        display_gfx_draw_pixel(display_adapter, x_start, y_start, value);

        //
        if (x_start == x_end && y_start == y_end) break;

        // err * 2
        e2 = err << 1;

        /* e_xy + e_x > 0 --> */
        if (e2 > delta_y) {
            err = err + delta_y;
            x_start += x_direction;
        }

        /* e_xy + e_y < 0 --> */
        if (e2 < delta_x) {
            err = err + delta_x;
            y_start += y_direction;
        }
    }
}



void display_gfx_draw_rectangle(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t x_length, uint8_t y_length, uint8_t value) {
    // Horizontal Lines
    display_gfx_draw_hline(display_adapter, x_start, y_start, x_length, value);
    display_gfx_draw_hline(display_adapter, x_start, y_start + y_length, x_length, value);

    // Vertical lines
    display_gfx_draw_vline(display_adapter, x_start, y_start, y_length, value);
    display_gfx_draw_vline(display_adapter, x_start + x_length, y_start, y_length, value);
}



void display_gfx_draw_fill_rectangle(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t x_length, uint8_t y_length, uint8_t value) {
    for (int y = 0; y < y_length; y++) {
        for (int x = 0; x < x_length; x++) {
            display_gfx_draw_pixel(display_adapter, x_start + x, y_start + y, value);
        }
    }
}



void display_gfx_draw_circle(const struct display_adapter_descriptor *display_adapter, uint8_t x_center, uint8_t y_center, uint8_t r, uint8_t value) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    // Draw the 4 horizontal/vertical points
    display_gfx_draw_pixel(display_adapter, x_center, y_center + r, value);
    display_gfx_draw_pixel(display_adapter, x_center, y_center - r, value);
    display_gfx_draw_pixel(display_adapter, x_center + r, y_center, value);
    display_gfx_draw_pixel(display_adapter, x_center - r, y_center, value);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        display_gfx_draw_pixel(display_adapter, x_center + x, y_center + y, value);
        display_gfx_draw_pixel(display_adapter, x_center - x, y_center + y, value);
        display_gfx_draw_pixel(display_adapter, x_center + x, y_center - y, value);
        display_gfx_draw_pixel(display_adapter, x_center - x, y_center - y, value);
        display_gfx_draw_pixel(display_adapter, x_center + y, y_center + x, value);
        display_gfx_draw_pixel(display_adapter, x_center - y, y_center + x, value);
        display_gfx_draw_pixel(display_adapter, x_center + y, y_center - x, value);
        display_gfx_draw_pixel(display_adapter, x_center - y, y_center - x, value);
    }
}



void display_gfx_draw_gauge(const struct display_adapter_descriptor *display_adapter, uint8_t x_center, uint8_t y_center, uint8_t r, uint16_t angle, uint8_t value) {
    // Draw outer circle
    display_gfx_draw_circle(display_adapter, x_center,
                            y_center,
                            r,
                            value);

    float angle_rad = (float)angle * ((float)M_PI / 180.0F);

    float cosi = cosf(angle_rad) * r;
    float sini = sinf(angle_rad) * r;
    float x_end = (float)x_center + cosi;
    float y_end = y_center - sini;

    display_gfx_draw_line(display_adapter, x_center,
                          y_center,
                          (uint8_t)x_end,
                          (uint8_t)y_end,
                          value);
}



void display_gfx_draw_gfx_char(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t cha, uint8_t value, uint8_t scale_factor, const GFXfont *font) {
    cha = cha - font->first;
    GFXglyph *glyph = &(font->glyph)[cha];
    uint8_t *bitmap = font->bitmap;

    uint16_t bitmap_offset = glyph->bitmapOffset;
    uint8_t glyph_width = glyph->width;
    uint8_t glyph_height = glyph->height;
    int8_t x_offset = glyph->xOffset;
    int8_t y_offset = glyph->yOffset;
    uint8_t bits = 0;
    uint8_t bit = 0;

    for (uint8_t yy = 0; yy < glyph_height; yy++) {
        for (uint8_t xx = 0; xx < glyph_width; xx++) {
            // This is a counter that increments 'bits' to the next glyph bits when needed after 8 rounds
            if (!(bit++ & 7)) {
                bits = bitmap[bitmap_offset++];
            }

            if (bits & 0x80) {
                if (scale_factor == 1) {
                    display_gfx_draw_pixel(display_adapter, x_start + x_offset + xx, y_start + y_offset + yy, value);
                } else {
                    // Error: Char is drawn above the y_start line
                    display_gfx_draw_fill_rectangle(display_adapter, x_start + (x_offset + xx) * scale_factor, y_start + (y_offset + yy) * scale_factor, scale_factor, scale_factor, value);
                }
            }
            bits <<= 1;
        }
    }
}



void display_gfx_draw_gfx_font_len(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, const char *text, size_t len, uint8_t value, uint8_t scale_factor, const GFXfont *font) {
    uint8_t x_nextChar = x_start;
    size_t text_length = len;
    for (uint8_t i = 0; i < text_length; i++) {
        uint8_t cha = text[i];
        GFXglyph *glyph = &(font->glyph)[cha - font->first];
        display_gfx_draw_gfx_char(display_adapter, x_nextChar, y_start, cha, value, scale_factor, font);
        x_nextChar += glyph->xAdvance * scale_factor;
    }
}



void display_gfx_draw_gfx_font(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, const char *text, uint8_t value, uint8_t scale_factor, const GFXfont *font) {
    size_t len = strlen(text);
    return display_gfx_draw_gfx_font_len(display_adapter, x_start, y_start, text, len, value, scale_factor, font);
}



void display_gfx_draw_invert(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end) {
    for (int y = y_start; y < y_end; y++) {
        for (int x = x_start; x < x_end; x++) {
            //        oled_object->get_display_buffer
        }
    }
}



void display_gfx_draw_half_dino_dini_sigmoid(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, float k, uint8_t x_end, uint8_t y_end, uint8_t value) {
    uint8_t delta_x = x_end - x_start;
    uint8_t delta_y = y_end - y_start;
    float stepsize = 1 / (float)delta_x;
    float current_x = 0;

    for (uint16_t i = 0; i < delta_x; i++) {
        float res = display_gfx_calculate_dino_dini_sigmoid(current_x, k);
        current_x += stepsize;
        // res is a value between 0 and 1 so we have to scale it according to our y space
        res = res * delta_y;
        uint8_t int_res = (uint8_t)(res + 0.5f);

        display_gfx_draw_pixel(display_adapter, x_start + i, y_end - int_res, value);
    }
}



void display_gfx_draw_full_dino_dini_sigmoid(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, float k, uint8_t x_end, uint8_t y_end, uint8_t value) {
    // We only need to calculate half of the points because we are going to mirror the negative half

    uint8_t delta_x_half = (x_end - x_start) >> 1;
    uint8_t delta_y_half = (y_end - y_start) >> 1;

    float stepsize = 1 / (float)delta_x_half;
    float current_x = 0;

    for (uint16_t i = 0; i < delta_x_half; i++) {
        float res = display_gfx_calculate_dino_dini_sigmoid(current_x, k);
        current_x += stepsize;
        // res is a value between 0 and 1 so we have to scale it according to our y space
        res = res * delta_y_half;
        uint8_t int_res = (uint8_t)(res + 0.5);

        display_gfx_draw_pixel(display_adapter, x_start + delta_x_half + i, y_end - delta_y_half - int_res, value);
        display_gfx_draw_pixel(display_adapter, x_start + delta_x_half - i, y_start + delta_y_half + int_res, value);
    }
}



unsigned short display_gfx_get_font_x_size_len(const char *text, size_t len, uint8_t scale_factor, const GFXfont *font) {
    size_t text_length = len;
    unsigned short x_size = 0;

    if (text_length < 1) {
        return x_size;
    }

    GFXglyph *glyph_0 = &(font->glyph)[text[0] - font->first];
    if (glyph_0->xOffset < 0) {
        x_size -= glyph_0->xOffset;
    }

    for (uint8_t i = 0; i < text_length - 1; i++) {
        GFXglyph *glyph = &(font->glyph)[text[i] - font->first];
        x_size += glyph->xAdvance;
    }

    GFXglyph *glyph_last = &(font->glyph)[text[text_length - 1] - font->first];
    x_size += glyph_last->xOffset + glyph_last->width;

    return x_size;
}



unsigned short display_gfx_get_font_x_size(const char *text, uint8_t scale_factor, const GFXfont *font) {
    size_t len = strlen(text);
    return display_gfx_get_font_x_size_len(text, len, scale_factor, font);
}



void display_gfx_draw_image_grayscale(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t width, uint8_t height, const uint8_t *pixel_data, uint8_t pixels_per_byte) {
#if defined(DISPLAY_GFX_STATIC_GRAYSCALE_IMG_FORMAT_1_PIXEL_PER_BYTE)
    uint32_t i = 0;
    for (uint8_t y = 0; y < height; y++) {
        for (uint8_t x = 0; x < width; x++) {
            uint8_t pixel_value = pixel_data[i];

            if (pixel_value) {
                display_gfx_draw_pixel(display_adapter, x + x_start, y + y_start, pixel_value);
            }
            i++;
        }
    }
#elif defined(DISPLAY_GFX_STATIC_GRAYSCALE_IMG_FORMAT_2_PIXEL_PER_BYTE)

#elif defined(DISPLAY_GFX_STATIC_GRAYSCALE_IMG_FORMAT_4_PIXEL_PER_BYTE)


#elif defined(DISPLAY_GFX_STATIC_GRAYSCALE_IMG_FORMAT_8_PIXEL_PER_BYTE)

#else

#endif
}
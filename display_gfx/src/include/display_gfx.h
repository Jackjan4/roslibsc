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



/// @brief Draws a single pixel. 
/// @param display_adapter The display adapter on which to draw
/// @param x The X coordinate of the pixel on the display
/// @param y The Y coordinate of the pixel on the display
/// @param value The value (color/brightness, depending on the display) of the pixel
void display_gfx_draw_pixel(const struct display_adapter_descriptor *display_adapter, uint8_t x, uint8_t y, uint8_t value);



/// @brief Draws a horizontal line from the start coordinates with the given length
/// @param display_adapter The display adapter on which to draw
/// @param x_start The X start coordinate on the display
/// @param y_start The Y start coordinate on the display
/// @param length The length ofm the line
/// @param value The value (color/brightness, depending on the display) of the pixel
void display_gfx_draw_hline(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t length, uint8_t value);



/// @brief Draws a vertical line from the start coordinates with the given length
/// @param display_adapter The display adapter on which to draw
/// @param x_start The X start coordinate on the display
/// @param y_start The Y start coordinate on the display
/// @param length The length ofm the line
/// @param value The value (color/brightness, depending on the display) of the pixel
void display_gfx_draw_vline(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t length, uint8_t value);



/// @brief Draws a  line from the start coordinates to the end coordinates
/// @param display_adapter The display adapter on which to draw
/// @param x_start The X start coordinate on the display
/// @param y_start The Y start coordinate on the display
/// @param x_end The X start coordinate on the display
/// @param y_end The Y end coordinate on the display
/// @param value The value (color/brightness, depending on the display) of the pixel
void display_gfx_draw_line(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end, uint8_t value);



/// @brief Draws a rectangle according to the given start and end points. The start point is the upper left corner and the enpoint is the lower right corner. 
/// @param display_adapter The display adapter on which to draw
/// @param x_start The X start coordinate on the display
/// @param y_start The Y start coordinate on the display
/// @param x_length The width of the rectangle
/// @param y_length The height of the rectangle
/// @param value The value (color/brightness, depending on the display) of the pixel
void display_gfx_draw_rectangle(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t x_length, uint8_t y_length, uint8_t value);



/// @brief Draws a filled rectangle according to the given start and end points. The start point is the upper left corner and the enpoint is the lower right corner. 
/// @param display_adapter The display adapter on which to draw
/// @param x_start The X start coordinate on the display
/// @param y_start The Y start coordinate on the display
/// @param x_length The width of the rectangle
/// @param y_length The height of the rectangle
/// @param value The value (color/brightness, depending on the display) of the pixel
void display_gfx_draw_fill_rectangle(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t x_length, uint8_t y_length, uint8_t value);



/// @brief Draws a circle with the specified center point and radius.
/// @param display_adapter The display adapter on which to draw
/// @param x_center The X center coordinate on the display
/// @param y_center The Y center coordinate on the display
/// @param r The radius of the circle
/// @param value The value (color/brightness, depending on the display) of the pixel
void display_gfx_draw_circle(const struct display_adapter_descriptor *display_adapter, uint8_t x_center, uint8_t y_center, uint8_t r, uint8_t value);



/// @brief Draws a gauge (a circle with an arrow) with the specified center point, radius and arrow angle
/// @param display_adapter The display adapter on which to draw
/// @param x_center The X center coordinate on the display
/// @param y_center The Y center coordinate on the display
/// @param r The radius of the circle
/// @param angle The angle of the arrow. 0 degrees equals to an arrow point up. Rotation is clockwise.
/// @param value The value (color/brightness, depending on the display) of the pixel
void display_gfx_draw_gauge(const struct display_adapter_descriptor *display_adapter, uint8_t x_center, uint8_t y_center, uint8_t r, uint16_t angle, uint8_t value);



/// @brief Draws text by using an gfxfont with the specified length and an applied scale factor to resize the text
/// @param display_adapter The display adapter on which to draw
/// @param x_start The X start coordinate for the text on the display
/// @param y_start The Y start coordinate for the text on the display
/// @param text The text to be drawn. Needs to be a C-string.
/// @param len  The length of the text
/// @param value The value (color/brightness, depending on the display) of the pixels
/// @param scale_factor Applied scale factor for resizing the text. 1 equals the normal font size. 2 equals a text with doubled size and so on.
/// @param font A pointer to the font which should be used for the text
void display_gfx_draw_gfx_font_len(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, const char *text, size_t len, uint8_t value, uint8_t scale_factor, const GFXfont *font);



/// @brief Draws text by using an gfxfont and an applied scale factor to resize the text.
/// @param display_adapter The display adapter on which to draw
/// @param x_start The X start coordinate for the text on the display
/// @param y_start The Y start coordinate for the text on the display
/// @param text The text to be drawn. Needs to be a C-String. The text is drawn in full length.
/// @param value The value (color/brightness, depending on the display) of the pixels
/// @param scale_factor Applied scale factor for resizing the text. 1 equals the normal font size. 2 equals a text with doubled size and so on.
/// @param font A pointer to the font which should be used for the text
void display_gfx_draw_gfx_font(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, const char *text, uint8_t value, uint8_t scale_factor, const GFXfont *font);


/// @brief Draws a single char by using an gfxfont and an applied scale factor to resize it.
/// @param display_adapter The display adapter on which to draw
/// @param x_start The X start coordinate for the char on the display
/// @param y_start The Y start coordinate for the char on the display
/// @param cha The character to be drawn
/// @param value The value (color/brightness, depending on the display) of the pixels
/// @param scale_factor Applied scale factor for resizing the text. 1 equals the normal font size. 2 equals a text with doubled size and so on.
/// @param font A pointer to the font which should be used for the text
void display_gfx_draw_gfx_char(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t cha, uint8_t value, uint8_t scale_factor, const GFXfont *font);



/// @brief Inverts the pixels in the given rectangle. The applied inversion method is dependent on the bitmode of the display_adapter.
/// @param display_adapter The display adapter on which to draw
/// @param x_start The X start coordinate for the rectangle where the inversion should occur on the display
/// @param y_start The Y start coordinate for the rectangle where the inversion should occur on the display
/// @param x_end The X end coordinate for the rectangle where the inversion should occur on the display
/// @param y_end The Y end coordinate for the rectangle where the inversion should occur on the display
void display_gfx_draw_invert(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end);



/// @brief Draws the positive (x >= 0) half of a tunable Dino-Dini-Sigmoid curve. Explanation of the curve can be found here: https://dhemery.github.io/DHE-Modules/technical/sigmoid/
/// @param display_adapter The display adapter on which to draw
/// @param x_start The X start coordinate (lower left corner) where the curve should be drawed.
/// @param y_start The Y start coordinate (lower left corner) where the curve should be drawed.
/// @param k 
/// @param x_end The X end coordinate (upper right corner) where the curve should be drawed.
/// @param y_end The Y end coordinate (upper right corner) where the curve should be drawed.
/// @param value The value (color/brightness, depending on the display) of the pixels
void display_gfx_draw_half_dino_dini_sigmoid(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, float k, uint8_t x_end, uint8_t y_end, uint8_t value);



/// @brief Draws a a tunable Dino-Dini-Sigmoid curve. Explanation of the curve can be found here: https://dhemery.github.io/DHE-Modules/technical/sigmoid/
/// @param display_adapter The display adapter on which to draw
/// @param x_start The X start coordinate (lower left corner) where the curve should be drawed.
/// @param y_start The Y start coordinate (lower left corner) where the curve should be drawed.
/// @param k 
/// @param x_end The X end coordinate (upper right corner) where the curve should be drawed.
/// @param y_end The Y end coordinate (upper right corner) where the curve should be drawed.
/// @param value The value (color/brightness, depending on the display) of the pixels
void display_gfx_draw_full_dino_dini_sigmoid(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, float k, uint8_t x_end, uint8_t y_end, uint8_t value);



/// @brief Draws a grayscale image.
/// @param display_adapter The display adapter on which to draw
/// @param x_start The X start coordinate where the image should begin
/// @param y_start The Y start coordinate where the image should begin
/// @param width The width of the image
/// @param height The height of the image
/// @param pixel_data A pointer to the pixel buffer where the image is stored
/// @param pixels_per_byte How many pixels are stored in a single byte of the pixel_data
void display_gfx_draw_image_grayscale(const struct display_adapter_descriptor *display_adapter, uint8_t x_start, uint8_t y_start, uint8_t width, uint8_t height, const uint8_t *pixel_data, uint8_t pixels_per_byte);



/// @brief Returns the width (horizontal pixel length) of the given text as if it would be drawn with the given length and scale factor on the display.
/// @param text The C-String text whose draw width should be calculated
/// @param len The length of the string
/// @param scale_factor Applied scale factor for resizing the text. 1 equals the normal font size. 2 equals a text with doubled size and so on.
/// @param font A pointer to the font which should be used for the text
/// @return 
unsigned short display_gfx_get_font_x_size_len(const char *text, size_t len, uint8_t scale_factor, const GFXfont *font);



/// @brief Returns the width (horizontal pixel length) of the given text as if it would be drawn with the given scale factor on the display.
/// @param text The C-String text whose draw width should be calculated
/// @param scale_factor Applied scale factor for resizing the text. 1 equals the normal font size. 2 equals a text with doubled size and so on.
/// @param font A pointer to the font which should be used for the text
/// @return 
unsigned short display_gfx_get_font_x_size(const char *text, uint8_t scale_factor, const GFXfont *font);

#ifdef __cplusplus
}
#endif
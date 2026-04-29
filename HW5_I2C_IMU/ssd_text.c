#include "ssd_text.h"

void draw_char(uint8_t x, uint8_t y, char c) {
    // draws a character c at position x, y
    
    // characters in the font.h file are stored as an array of 5 bytes
    // each bit in the byte is the row, and each byte is the column, so
    // each character is 8 rows by 5 columns

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 8; j++) {
            ssd1306_drawPixel(x + i, y + j, (ASCII[c - 32][i] & (1 << j)) != 0);
        }
    }
    ssd1306_update();
}

void draw_string(uint8_t x, uint8_t y, char* s) {
    for (int i = 0; i < 50; i++) {
        // break at null termination
        if (s[i] == 0) {
            return;
        }
        // assume text is all drawn on one row and we're not auto wrapping text for now
        draw_char(x + (5 * i), y, s[i]);
    }
}
#ifndef SSD_TEXT_H
#define SSD_TEXT_H

#include <stdint.h>
#include "ssd1306.h"
#include "font.h"

void draw_char(uint8_t x, uint8_t y, char c);
void draw_string(uint8_t x, uint8_t y, char* s);

#endif
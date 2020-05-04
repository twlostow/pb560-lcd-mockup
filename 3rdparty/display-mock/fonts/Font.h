#ifndef __FONT_H
#define __FONT_H

struct Font 
{
    uint16_t *offsets;
    uint8_t *data;
};

#include "font_Arial_8.c"
static const Font font_arial_8 = { font_offsets_Arial_8, font_data_Arial_8 };
#include "font_Arial_9.c"
static const Font font_arial_9 = { font_offsets_Arial_9, font_data_Arial_9 };
#include "font_Arial_29.c"
static const Font font_arial_29 = { font_offsets_Arial_29, font_data_Arial_29 };
#include "font_Arial_Black_11.c"
static const Font font_arial_black_11 = { font_offsets_Arial_Black_11, font_data_Arial_Black_11 };

#endif
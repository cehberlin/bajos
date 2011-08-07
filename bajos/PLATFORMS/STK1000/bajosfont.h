/*
    created with FontEditor written by H. Reddmann
    HaReddmann at t-online dot de

File Name           : bajosfont.h
Date                : 09.10.2008
Font size in bytes  : 0x0176, 374
Font width          : 17
Font height         : 25
Font first char     : 0x41
Font last char      : 0x53
Font bits per pixel : 4
Font is compressed  : true

The font data are defined as

struct _FONT_ {
// common shared fields
uint16_t   font_Size_in_Bytes_over_all_included_Size_it_self;
uint8_t    font_Width_in_Pixel_for_fixed_drawing;
uint8_t    font_Height_in_Pixel_for_all_Characters;
uint8_t    font_Bits_per_Pixels;
// if MSB are set then font is a compressed font
uint8_t    font_First_Char;
uint8_t    font_Last_Char;
uint8_t    font_Char_Widths[font_Last_Char - font_First_Char +1];
// for each character the separate width in pixels,
// characters < 128 have an implicit virtual right empty row
// characters with font_Char_Widths[] == 0 are undefined

// if compressed font then additional fields
uint8_t    font_Byte_Padding;
// each Char in the table are aligned in size to this value
uint8_t    font_RLE_Table[3];
// Run Length Encoding Table for compression
uint8_t    font_Char_Size_in_Bytes[font_Last_Char - font_First_Char +1];
// for each char the size in (bytes / font_Byte_Padding) are stored,
// this get us the table to seek to the right beginning of each char
// in the font_data[].

// for compressed and uncompressed fonts
uint8_t    font_data[];
// bit field of all characters
}
*/

//#include <inttypes.h>
#include "../../JVM/typedefinitions.h"

#ifndef bajosfont_H
#define bajosfont_H

#define bajosfont_WIDTH 17
#define bajosfont_HEIGHT 25

static u2 bajosfont[] =
{
    0x01, 0x76, 0x11, 0x19, 0x84, 0x41, 0x53,
    0x0E, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00,
    0x00, 0x00, 0x0B,
    0x01, 0x02, 0x05, 0x0B,
    0x42, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x00,
    0x00, 0x00, 0x35,
    0x03, 0x60, 0x10, 0x83, 0x00, 0x18, 0xC6, 0x10, 0x14, 0x85, 0x50, 0x10, 0x03, 0x50, 0x10, 0x42,
    0x00, 0x14, 0x43, 0x21, 0x08, 0xC5, 0x40, 0x08, 0x42, 0x31, 0x14, 0x42, 0x10, 0x14, 0x03, 0x40,
    0x08, 0x81, 0x41, 0x0C, 0x44, 0x10, 0x18, 0x86, 0x20, 0x10, 0x81, 0x21, 0x14, 0x82, 0x40, 0x00,
    0x05, 0x21, 0x04, 0x40, 0x21, 0x08, 0x05, 0x21, 0x08, 0xC4, 0x50, 0x08, 0x42, 0x31, 0x0C, 0x85,
    0x10, 0x00, 0x83, 0x00, 0x64, 0xC3, 0x90, 0x65, 0x43, 0x00, 0x64, 0x98, 0x10, 0x60, 0x43, 0xA0,
    0x09, 0x00, 0x36, 0x00, 0x59, 0x90, 0x09, 0xD9, 0x90, 0x09, 0x41, 0x90, 0x0D, 0x00, 0x26, 0x68,
    0xD9, 0x80, 0x05, 0x80, 0x96, 0x65, 0x03, 0x80, 0x05, 0x5A, 0x80, 0x05, 0x99, 0x20, 0x60, 0x40,
    0x26, 0x04, 0x40, 0x86, 0x09, 0x02, 0x06, 0x60, 0x03, 0x26, 0x08, 0xD8, 0x10, 0x60, 0x82, 0x90,
    0x09, 0x58, 0x10, 0x64, 0x03, 0x80, 0x05, 0x81, 0x96, 0x61, 0x03, 0xA0, 0x61, 0x41, 0x96, 0x0D,
    0x01, 0x90, 0x65, 0x43, 0x00, 0x00, 0xC3, 0x90, 0xA2, 0x83, 0x20, 0xA0, 0x01, 0x2A, 0x04, 0x40,
    0x8A, 0x0A, 0x41, 0x80, 0x06, 0xA8, 0x10, 0xA8, 0x42, 0x10, 0xA4, 0xA9, 0x10, 0xA4, 0x42, 0x10,
    0xA8, 0x42, 0x00, 0xA4, 0x02, 0xA0, 0xA2, 0x43, 0x1A, 0x00, 0x2A, 0x1A, 0xA0, 0x43, 0x8A, 0x02,
    0x29, 0x2A, 0x00, 0xE8, 0x00, 0xA4, 0xA8, 0x10, 0x00, 0xE8, 0x10, 0xA0, 0x82, 0x80, 0x0E, 0x00,
    0x2A, 0x08, 0xA8, 0x10, 0x00, 0x03, 0xE0, 0x71, 0x83, 0x00, 0x78, 0x5D, 0x37, 0x04, 0x40, 0xD7,
    0x09, 0x1D, 0x37, 0x00, 0x9D, 0x10, 0x04, 0xDD, 0xD0, 0x09, 0x42, 0x27, 0x08, 0xDD, 0xC0, 0x0D,
    0x5C, 0xD0, 0x75, 0x42, 0x37, 0x00, 0x1E, 0x17, 0x04, 0xDD, 0x00, 0x74, 0x01, 0xC0, 0x05, 0x41,
    0x37, 0x04, 0x9C, 0x10, 0x74, 0x43, 0x10, 0x70, 0x42, 0xC7, 0x0D, 0x02, 0xE0, 0x0D, 0x01, 0x00,
    0x83, 0x50, 0x0D, 0x43, 0x55, 0x0D, 0x43, 0x50, 0x0D, 0x40, 0x55, 0x09, 0x01, 0x40, 0x0D, 0x16,
    0x25, 0x04, 0x94, 0x20, 0x54, 0x41, 0x50, 0x09, 0x00, 0x25, 0x08, 0x94, 0x50, 0x51, 0x02, 0x25,
    0x08, 0x94, 0x00, 0x54, 0x41, 0x40, 0x0D, 0x95, 0x00, 0x58, 0x03, 0x50, 0x55, 0x42, 0x45, 0x0D,
    0x01, 0x50, 0x0D, 0x02, 0x00
};
#endif

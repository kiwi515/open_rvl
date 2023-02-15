#ifndef RVL_SDK_OS_FONT_H
#define RVL_SDK_OS_FONT_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Documentation from:
 * https://www.gc-forever.com/yagcd/chap16.html#sec16.1.3
 */

typedef enum {
    OS_FONT_ENCODE_ANSI,
    OS_FONT_ENCODE_SJIS,
    OS_FONT_ENCODE_2,
    OS_FONT_ENCODE_UTF8,
    OS_FONT_ENCODE_UTF16,
    OS_FONT_ENCODE_UTF32,
    OS_FONT_ENCODE_MAX
} OSFontEncode;

typedef struct OSFontHeader {
    u16 type;          // at 0x0
    u16 firstChar;     // at 0x2
    u16 lastChar;      // at 0x4
    u16 invalidChar;   // at 0x6
    u16 ascent;        // at 0x8
    u16 descent;       // at 0xA
    u16 width;         // at 0xC
    u16 leading;       // at 0xE
    u16 cellWidth;     // at 0x10
    u16 cellHeight;    // at 0x12
    u32 sheetSize;     // at 0x14
    u16 sheetFormat;   // at 0x18
    u16 sheetNumCol;   // at 0x1A
    u16 sheetNumRow;   // at 0x1C
    u16 sheetWidth;    // at 0x1E
    u16 sheetHeight;   // at 0x20
    u16 widthTableOfs; // at 0x22
    u32 sheetImageOfs; // at 0x24
    u32 sheetFullSize; // at 0x28
} OSFontHeader;

u16 OSGetFontEncode(void);
u16 OSSetFontEncode(u16);
u32 OSLoadFont(OSFontHeader*, void*);
const char* OSGetFontTexel(const char*, void*, s32, s32, u32*);
BOOL OSInitFont(OSFontHeader*);
const char* OSGetFontTexture(const char*, void**, u32*, u32*, u32*);
const char* OSGetFontWidth(const char*, u32*);

#ifdef __cplusplus
}
#endif
#endif

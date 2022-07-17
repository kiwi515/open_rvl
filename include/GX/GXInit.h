#ifndef RVL_SDK_GX_INIT_H
#define RVL_SDK_GX_INIT_H
#include <GX.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GXData {
    union {
        UNKWORD WORD_0x0;
        u16 SHORTS_0x0[2];
    };
    char UNK_0x4[0x8 - 0x4];
    UNKWORD WORD_0x8;
    char UNK_0xC[0xA8 - 0xC];
    GXColor ambColors[2]; // at 0xA8
    GXColor matColors[2]; // at 0xB0
    u32 WORDS_0xB8[2];
    u32 WORD_0xC0;
    u32 WORD_0xC4;
    char UNK_0xC8[0x254 - 0xC8];
    u32 WORD_0x254;
    char UNK_0x258[0x5F8 - 0x258];
    u8 dlistBegan; // at 0x5F8
    u8 BYTE_0x5F9;
    u32 dirtyFlags; // at 0x5FC
} GXData;

extern GXData* const __GXData;

#ifdef __cplusplus
}
#endif
#endif

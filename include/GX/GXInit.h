#ifndef RVL_SDK_GX_INIT_H
#define RVL_SDK_GX_INIT_H
#include <GX.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GXData {
    UNKWORD WORD_0x0;
    char UNK_0x4[0x8 - 0x4];
    UNKWORD WORD_0x8;
    char UNK_0xC[0x5F8 - 0xC];
    u8 dlistBegan; // at 0x5F8
    u8 BYTE_0x5F9;
    u32 dirtyFlags; // at 0x5FC
} GXData;

extern GXData* __GXData;

#ifdef __cplusplus
}
#endif
#endif

#ifndef RVL_SDK_OS_STATE_FLAGS_H
#define RVL_SDK_OS_STATE_FLAGS_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSStateFlags {
    u32 checksum; // at 0x0
    u32 WORD_0x4;
    u32 WORD_0x8;
    u32 WORD_0xC;
    u32 WORD_0x10;
    u32 WORD_0x14;
    u32 WORD_0x18;
    u32 WORD_0x1C;
} OSStateFlags;

BOOL __OSWriteStateFlags(const OSStateFlags*);
BOOL __OSReadStateFlags(OSStateFlags*);

#ifdef __cplusplus
}
#endif
#endif

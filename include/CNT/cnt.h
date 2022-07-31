#ifndef RVL_SDK_CNT_H
#define RVL_SDK_CNT_H
#include <ARC/arc.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct CNTHandle {
    ARCHandle arcHandle; // at 0x0
    UNKWORD WORD_0x1C;
} CNTHandle;

typedef struct CNTFileInfo {
    CNTHandle* handle; // at 0x0
    u32 offset;        // at 0x4
    u32 size;          // at 0x8
    u32 position;      // at 0xC
} CNTFileInfo;

s32 contentFastOpenNAND(CNTHandle*, s32, CNTFileInfo*);
s32 contentConvertPathToEntrynumNAND(CNTHandle*, const char*);
u32 contentGetLengthNAND(CNTFileInfo*);
s32 contentReadNAND(CNTFileInfo*, void*, s32, s32);
s32 contentCloseNAND(CNTFileInfo*);

#ifdef __cplusplus
}
#endif
#endif

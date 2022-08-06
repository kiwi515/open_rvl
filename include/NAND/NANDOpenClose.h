#ifndef RVL_SDK_NAND_OPEN_CLOSE_H
#define RVL_SDK_NAND_OPEN_CLOSE_H
#include "nand.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

s32 NANDOpen(const char*, NANDFileInfo*, u8);
s32 NANDPrivateOpen(const char*, NANDFileInfo*, u8);

s32 NANDClose(NANDFileInfo*);

#ifdef __cplusplus
}
#endif
#endif

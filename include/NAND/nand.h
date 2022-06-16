#ifndef RVL_SDK_NAND_H
#define RVL_SDK_NAND_H
#include "NANDOpenClose.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

int NANDDelete(const char*);
int NANDRead(NANDFileInfo*, void*, u32);
int NANDWrite(NANDFileInfo*, const void*, u32);

#ifdef __cplusplus
}
#endif
#endif

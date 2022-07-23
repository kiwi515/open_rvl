#ifndef RVL_SDK_NAND_H
#define RVL_SDK_NAND_H
#include "NANDOpenClose.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NAND_SEEK_BEGIN,
    NAND_SEEK_0, //!< SEEK_CUR?
    NAND_SEEK_1, //!< SEEK_END?
} NANDSeekOrigin;

typedef struct NANDCommandBlock {
    u8 temp;
} NANDCommandBlock;

typedef void (*NANDAsyncCallback)(s32, NANDCommandBlock*);

s32 NANDDelete(const char*);
s32 NANDRead(NANDFileInfo*, void*, u32);
s32 NANDWrite(NANDFileInfo*, const void*, u32);
s32 NANDSeek(NANDFileInfo*, s32, NANDSeekOrigin);

#ifdef __cplusplus
}
#endif
#endif

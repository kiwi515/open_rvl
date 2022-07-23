#ifndef RVL_SDK_NAND_OPEN_CLOSE_H
#define RVL_SDK_NAND_OPEN_CLOSE_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct NANDFileInfo {
    char UNK_0x0[0x8C];
} NANDFileInfo;

typedef enum {
    NAND_ACCESS_0,
    NAND_READ,
    NAND_WRITE,
    NAND_ACCESS_3 //!< RW?
} NANDAccessType;

s32 NANDOpen(const char*, NANDFileInfo*, u8);
s32 NANDPrivateOpen(const char*, NANDFileInfo*, u8);

s32 NANDClose(NANDFileInfo*);

#ifdef __cplusplus
}
#endif
#endif

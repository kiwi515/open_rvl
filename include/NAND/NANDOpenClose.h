#ifndef RVL_SDK_NAND_OPEN_CLOSE_H
#define RVL_SDK_NAND_OPEN_CLOSE_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char UNK_0x0[0x8C];
} NANDFileInfo;

typedef enum {
    NAND_ACCESS_0,
    NAND_READ,
    NAND_WRITE,
    NAND_ACCESS_3 //!< RW?
} NANDAccessType;

int NANDOpen(const char*, NANDFileInfo*, NANDAccessType);
int NANDClose(NANDFileInfo*);

#ifdef __cplusplus
}
#endif
#endif

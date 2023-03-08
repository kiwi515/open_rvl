#ifndef RVL_SDK_NAND_OPEN_CLOSE_H
#define RVL_SDK_NAND_OPEN_CLOSE_H
#include <revolution/NAND/nand.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

NANDResult NANDOpen(const char*, NANDFileInfo*, u8);
NANDResult NANDPrivateOpen(const char*, NANDFileInfo*, u8);
NANDResult NANDOpenAsync(const char*, NANDFileInfo*, u8, NANDAsyncCallback,
                         NANDCommandBlock*);
NANDResult NANDPrivateOpenAsync(const char*, NANDFileInfo*, u8,
                                NANDAsyncCallback, NANDCommandBlock*);
NANDResult NANDClose(NANDFileInfo*);
NANDResult NANDCloseAsync(NANDFileInfo*, NANDAsyncCallback, NANDCommandBlock*);
NANDResult NANDPrivateSafeOpenAsync(const char*, NANDFileInfo*, u8, void*, u32,
                                    NANDAsyncCallback, NANDCommandBlock*);
NANDResult NANDSafeCloseAsync(NANDFileInfo*, NANDAsyncCallback,
                              NANDCommandBlock*);

#ifdef __cplusplus
}
#endif
#endif

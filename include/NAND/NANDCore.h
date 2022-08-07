#ifndef RVL_SDK_NAND_CORE_H
#define RVL_SDK_NAND_CORE_H
#include "nand.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void nandGetRelativeName(char*, const char*);

BOOL nandIsPrivatePath(const char*);
BOOL nandIsUnderPrivatePath(const char*);

BOOL nandIsInitialized(void);

NANDResult nandConvertErrorCode(s32);
void nandGenerateAbsPath(char*, const char*);
void nandGetParentDirectory(char*, const char*);

void nandCallback(IPCResult, void*);

const char* nandGetHomeDir(void);

#ifdef __cplusplus
}
#endif
#endif

#ifndef RVL_SDK_NAND_CORE_H
#define RVL_SDK_NAND_CORE_H
#include <revolution/NAND/nand.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NAND_LIB_UNINITIALIZED,
    NAND_LIB_INITIALIZING,
    NAND_LIB_INITIALIZED
} NANDLibState;

void nandRemoveTailToken(char*, const char*);
void nandGetHeadToken(char*, char*, const char*);
void nandGetRelativeName(char*, const char*);
void nandConvertPath(char*, const char*, const char*);
BOOL nandIsPrivatePath(const char*);
BOOL nandIsUnderPrivatePath(const char*);
BOOL nandIsInitialized(void);
void nandReportErrorCode(IPCResult) __attribute__((never_inline));
NANDResult nandConvertErrorCode(IPCResult);
void nandGenerateAbsPath(char*, const char*);
void nandGetParentDirectory(char*, const char*);
NANDResult NANDInit(void);
NANDResult NANDGetCurrentDir(char*);
NANDResult NANDGetHomeDir(char*);
void nandCallback(s32, void*);
NANDResult NANDGetType(const char*, u8*);
NANDResult NANDPrivateGetTypeAsync(const char*, u8*, NANDAsyncCallback,
                                   NANDCommandBlock*);
const char* nandGetHomeDir(void);
void NANDInitBanner(NANDBanner*, u32, const wchar_t*, const wchar_t*);

#ifdef __cplusplus
}
#endif
#endif

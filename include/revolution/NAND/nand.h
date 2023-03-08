#ifndef RVL_SDK_NAND_H
#define RVL_SDK_NAND_H
#include <revolution/FS.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define NAND_BANNER_TITLE_MAX 32

typedef enum {
    NAND_RESULT_FATAL_ERROR = -128,
    NAND_RESULT_UNKNOWN = -64,

    NAND_RESULT_MAXDEPTH = -16,
    NAND_RESULT_AUTHENTICATION,
    NAND_RESULT_OPENFD,
    NAND_RESULT_NOTEMPTY,
    NAND_RESULT_NOEXISTS,
    NAND_RESULT_MAXFILES,
    NAND_RESULT_MAXFD,
    NAND_RESULT_MAXBLOCKS,
    NAND_RESULT_INVALID,

    NAND_RESULT_EXISTS = -6,
    NAND_RESULT_ECC_CRIT,
    NAND_RESULT_CORRUPT,
    NAND_RESULT_BUSY,
    NAND_RESULT_ALLOC_FAILED,
    NAND_RESULT_ACCESS,

    NAND_RESULT_OK,
} NANDResult;

typedef enum {
    NAND_SEEK_BEG,
    NAND_SEEK_CUR,
    NAND_SEEK_END,
} NANDSeekMode;

typedef enum {
    NAND_ACCESS_NONE,
    NAND_ACCESS_READ,
    NAND_ACCESS_WRITE,
    NAND_ACCESS_RW
} NANDAccessType;

typedef enum {
    NAND_FILE_TYPE_NONE,
    NAND_FILE_TYPE_FILE,
    NAND_FILE_TYPE_DIR,
} NANDFileType;

typedef enum {
    // Read/write by owner
    NAND_PERM_RUSR = (NAND_ACCESS_READ << 4),
    NAND_PERM_WUSR = (NAND_ACCESS_WRITE << 4),
    // Read/write by group
    NAND_PERM_RGRP = (NAND_ACCESS_READ << 2),
    NAND_PERM_WGRP = (NAND_ACCESS_WRITE << 2),
    // Read/write by other
    NAND_PERM_ROTH = (NAND_ACCESS_READ << 0),
    NAND_PERM_WOTH = (NAND_ACCESS_WRITE << 0),
    // Read/write by all
    NAND_PERM_RALL = NAND_PERM_RUSR | NAND_PERM_RGRP | NAND_PERM_ROTH,
    NAND_PERM_WALL = NAND_PERM_WUSR | NAND_PERM_WGRP | NAND_PERM_WOTH,
} NANDPermission;

typedef struct NANDCommandBlock;
typedef void (*NANDAsyncCallback)(s32, struct NANDCommandBlock*);

typedef struct NANDFileInfo {
    s32 fd; // at 0x0
    s32 WORD_0x4;
    char openPath[FS_MAX_PATH];   // at 0x8
    char renamePath[FS_MAX_PATH]; // at 0x48
    u8 access;                    // at 0x88
    u8 BYTE_0x89;
    u8 state; // at 0x8A
} NANDFileInfo;

typedef struct NANDCommandBlock {
    void* userData;             // at 0x0
    NANDAsyncCallback callback; // at 0x4
    NANDFileInfo* info;         // at 0x8
    char UNK_0xC[0x14 - 0xC];
    FSFileAttr* fileAttr;   // at 0x14
    FSFileAttr fileAttrObj; // at 0x18
    u32 attr;               // at 0x20
    u32 ownerPerm;          // at 0x24
    u32 groupPerm;          // at 0x28
    u32 otherPerm;          // at 0x2C
    u32 dirFileCount;       // at 0x30
    char path[FS_MAX_PATH]; // at 0x34
    u32* lengthOut;         // at 0x74
    u32* positionOut;       // at 0x8
    s32 WORD_0x7C;
    void* buffer;   // at 0x80
    u32 bufferSize; // at 0x84
    u8* typeOut;    // at 0x88
    u32 tempDirId;  // at 0x8C
    char UNK_0x90[0xB8 - 0x90];
} NANDCommandBlock;

typedef struct NANDBanner {
    u32 magic; // at 0x0
    u32 flags; // at 0x4
    char UNK_0x8[0x20 - 0x8];
    wchar_t title[NAND_BANNER_TITLE_MAX];    // at 0x20
    wchar_t subtitle[NAND_BANNER_TITLE_MAX]; // at 0x60
    char UNK_0xA0[0xF0A0 - 0xA0];
} NANDBanner;

NANDResult NANDCreate(const char*, u8, u8);
NANDResult NANDPrivateCreate(const char*, u8, u8);
NANDResult NANDPrivateCreateAsync(const char*, u8, u8, NANDAsyncCallback,
                                  NANDCommandBlock*);
NANDResult NANDDelete(const char*);
NANDResult NANDPrivateDelete(const char*);
NANDResult NANDPrivateDeleteAsync(const char*, NANDAsyncCallback,
                                  NANDCommandBlock*);
NANDResult NANDRead(NANDFileInfo*, void*, u32);
NANDResult NANDReadAsync(NANDFileInfo*, void*, u32, NANDAsyncCallback,
                         NANDCommandBlock*);
NANDResult NANDWrite(NANDFileInfo*, const void*, u32);
NANDResult NANDWriteAsync(NANDFileInfo*, const void*, u32, NANDAsyncCallback,
                          NANDCommandBlock*);
NANDResult NANDSeek(NANDFileInfo*, s32, NANDSeekMode);
NANDResult NANDSeekAsync(NANDFileInfo*, s32, NANDSeekMode, NANDAsyncCallback,
                         NANDCommandBlock*);
NANDResult NANDPrivateCreateDir(const char*, u8, u8);
NANDResult NANDPrivateCreateDirAsync(const char*, u8, u8, NANDAsyncCallback,
                                     NANDCommandBlock*);
NANDResult NANDMove(const char*, const char*);
NANDResult NANDGetLength(NANDFileInfo*, u32*);
NANDResult NANDGetLengthAsync(NANDFileInfo*, u32*, NANDAsyncCallback,
                              NANDCommandBlock*);
NANDResult NANDGetStatus(const char*, FSFileAttr*);
NANDResult NANDPrivateGetStatusAsync(const char*, FSFileAttr*,
                                     NANDAsyncCallback, NANDCommandBlock*);
void NANDSetUserData(NANDCommandBlock*, void*);
void* NANDGetUserData(NANDCommandBlock*);

#ifdef __cplusplus
}
#endif
#endif

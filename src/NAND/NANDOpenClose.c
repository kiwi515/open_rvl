#include "NANDOpenClose.h"
#include "NANDCore.h"

#include <OS/OSInterrupt.h>
#include <stdio.h>

static void nandOpenCallback(IPCResult, void*);
static NANDResult nandSafeOpenAsync(const char*, NANDFileInfo*, u8, void*, u32,
                                    NANDAsyncCallback, NANDCommandBlock*, BOOL);
static void nandSafeOpenCallback(IPCResult, void*);
static void nandReadOpenCallback(IPCResult, void*);
static NANDResult nandSafeCloseAsync(NANDFileInfo*, NANDAsyncCallback,
                                     NANDCommandBlock*);
static void nandSafeCloseCallback(IPCResult, void*);
static void nandReadCloseCallback(IPCResult, void*);
static void nandCloseCallback(IPCResult, void*);
static u32 nandGetUniqueNumber(void);

static IPCResult nandOpen(const char* path, u8 mode, NANDCommandBlock* block,
                          BOOL async, BOOL priv) {
    IPCOpenMode ipcMode;
    char absPath[64];

    __memclr(absPath, sizeof(absPath));
    ipcMode = IPC_OPEN_NONE;

    nandGenerateAbsPath(absPath, path);

    if (!priv && nandIsPrivatePath(absPath)) {
        return IPC_RESULT_ACCESS;
    }

    switch (mode) {
    case NAND_ACCESS_RW:
        ipcMode = IPC_OPEN_RW;
        break;
    case NAND_ACCESS_READ:
        ipcMode = IPC_OPEN_READ;
        break;
    case NAND_ACCESS_WRITE:
        ipcMode = IPC_OPEN_WRITE;
        break;
    }

    if (async) {
        return ISFS_OpenAsync(absPath, ipcMode, nandOpenCallback, block);
    } else {
        return ISFS_Open(absPath, ipcMode);
    }
}

NANDResult NANDOpen(const char* path, NANDFileInfo* info, u8 mode) {
    IPCResult result;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    result = nandOpen(path, mode, NULL, FALSE, FALSE);
    if (result >= 0) {
        info->fd = result;
        info->state = 1;
        return NAND_RESULT_OK;
    }

    return nandConvertErrorCode(result);
}

NANDResult NANDPrivateOpen(const char* path, NANDFileInfo* info, u8 mode) {
    IPCResult result;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    result = nandOpen(path, mode, NULL, FALSE, TRUE);
    if (result >= 0) {
        info->fd = result;
        info->state = 1;
        return NAND_RESULT_OK;
    }

    return nandConvertErrorCode(result);
}

NANDResult NANDOpenAsync(const char* path, NANDFileInfo* info, u8 mode,
                         NANDAsyncCallback callback, NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    block->info = info;
    return nandConvertErrorCode(nandOpen(path, mode, block, TRUE, FALSE));
}

NANDResult NANDPrivateOpenAsync(const char* path, NANDFileInfo* info, u8 mode,
                                NANDAsyncCallback callback,
                                NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    block->info = info;
    return nandConvertErrorCode(nandOpen(path, mode, block, TRUE, TRUE));
}

static void nandOpenCallback(IPCResult result, void* arg) {
    NANDCommandBlock* block = (NANDCommandBlock*)arg;

    if (result >= 0) {
        block->info->fd = result;
        block->info->BYTE_0x89 = 2;
        block->info->state = 1;
        block->callback(NAND_RESULT_OK, block);
    } else {
        block->callback(nandConvertErrorCode(result), block);
    }
}

NANDResult NANDClose(NANDFileInfo* info) {
    IPCResult result;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    if (info->state != 1) {
        return NAND_RESULT_INVALID;
    }

    result = ISFS_Close(info->fd);
    if (result == IPC_RESULT_OK) {
        info->state = 2;
    }

    return nandConvertErrorCode(result);
}

NANDResult NANDCloseAsync(NANDFileInfo* info, NANDAsyncCallback callback,
                          NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    if (info->state != 1) {
        return NAND_RESULT_INVALID;
    }

    block->callback = callback;
    block->info = info;
    return nandConvertErrorCode(
        ISFS_CloseAsync(info->fd, nandCloseCallback, block));
}

NANDResult NANDPrivateSafeOpenAsync(const char* path, NANDFileInfo* info,
                                    u8 access, void* buffer, u32 bufferSize,
                                    NANDAsyncCallback callback,
                                    NANDCommandBlock* block) {
    return nandSafeOpenAsync(path, info, access, buffer, bufferSize, callback,
                             block, TRUE);
}

static NANDResult nandSafeOpenAsync(const char* path, NANDFileInfo* info,
                                    u8 access, void* buffer, u32 bufferSize,
                                    NANDAsyncCallback callback,
                                    NANDCommandBlock* block, BOOL priv) {
    IPCResult result;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    info->access = access;
    info->BYTE_0x89 = 0;

    nandGenerateAbsPath(info->openPath, path);

    if (!priv && nandIsPrivatePath(info->openPath)) {
        return NAND_RESULT_ACCESS;
    }

    if (access == NAND_ACCESS_READ) {
        block->info = info;
        block->callback = callback;
        result = ISFS_OpenAsync(info->openPath, IPC_OPEN_READ,
                                nandReadOpenCallback, block);
        return (result == IPC_RESULT_OK) ? NAND_RESULT_OK
                                         : nandConvertErrorCode(result);
    }

    if (access == NAND_ACCESS_WRITE || access == NAND_ACCESS_RW) {
        block->info = info;
        block->callback = callback;
        block->WORD_0x7C = 0;
        block->buffer = buffer;
        block->bufferSize = bufferSize;
        result = ISFS_CreateDirAsync("/tmp/sys", 0, IPC_OPEN_RW, IPC_OPEN_RW,
                                     IPC_OPEN_RW, nandSafeOpenCallback, block);
        return (result == IPC_RESULT_OK) ? NAND_RESULT_OK
                                         : nandConvertErrorCode(result);
    }

    return NAND_RESULT_INVALID;
}

static void nandSafeOpenCallback(IPCResult result, void* arg) {
// Why???
#define block ((NANDCommandBlock*)arg)

    IPCResult myResult;
    char tempPath[64];
    char relativeName[12];

    if (result >= 0 || (result == IPC_RESULT_EXISTS && block->WORD_0x7C == 0)) {
        NANDFileInfo* info = block->info;
        myResult = (IPCResult)-117;

        if (block->WORD_0x7C == 0) {
            info->BYTE_0x89 = 1;
        }

        block->WORD_0x7C++;

        if (block->WORD_0x7C == 1) {
            myResult = ISFS_GetAttrAsync(
                info->openPath, &block->fileAttrObj.ownerId,
                &block->fileAttrObj.groupId, &block->attr, &block->ownerPerm,
                &block->groupPerm, &block->otherPerm, nandSafeOpenCallback,
                block);
        } else if (block->WORD_0x7C == 2) {
            myResult = ISFS_OpenAsync(info->openPath, IPC_OPEN_READ,
                                      nandSafeOpenCallback, block);
        } else if (block->WORD_0x7C == 3) {
            info->BYTE_0x89 = 2;
            block->tempDirId = nandGetUniqueNumber();
            info->WORD_0x4 = result;
            sprintf(tempPath, "%s/%08x", "/tmp/sys", block->tempDirId);
            myResult =
                ISFS_CreateDirAsync(tempPath, 0, IPC_OPEN_RW, IPC_OPEN_NONE,
                                    IPC_OPEN_NONE, nandSafeOpenCallback, block);
        } else if (block->WORD_0x7C == 4) {
            info->BYTE_0x89 = 3;
            nandGetRelativeName(relativeName, info->openPath);
            sprintf(info->renamePath, "%s/%08x/%s", "/tmp/sys",
                    block->tempDirId, relativeName);
            myResult = ISFS_CreateFileAsync(info->renamePath, block->attr,
                                            block->ownerPerm, block->groupPerm,
                                            block->otherPerm,
                                            nandSafeOpenCallback, block);
        } else if (block->WORD_0x7C == 5) {
            info->BYTE_0x89 = 4;
            if (info->access == NAND_ACCESS_WRITE) {
                myResult = ISFS_OpenAsync(info->renamePath, IPC_OPEN_WRITE,
                                          nandSafeOpenCallback, block);
            } else if (info->access == NAND_ACCESS_RW) {
                myResult = ISFS_OpenAsync(info->renamePath, IPC_OPEN_RW,
                                          nandSafeOpenCallback, block);
            } else {
                myResult = (IPCResult)-117;
            }
        } else if (block->WORD_0x7C == 6) {
            info->fd = result;
            info->BYTE_0x89 = 5;
            block->WORD_0x7C = 7;
            myResult =
                ISFS_ReadAsync(info->WORD_0x4, block->buffer, block->bufferSize,
                               nandSafeOpenCallback, block);
        } else if (block->WORD_0x7C == 7) {
            myResult =
                ISFS_ReadAsync(info->WORD_0x4, block->buffer, block->bufferSize,
                               nandSafeOpenCallback, block);
        } else if (block->WORD_0x7C == 8) {
            if (result > 0) {
                block->WORD_0x7C = 6;
                myResult = ISFS_WriteAsync(info->fd, block->buffer, result,
                                           nandSafeOpenCallback, block);
            } else if (result == IPC_RESULT_OK) {
                myResult = ISFS_SeekAsync(info->fd, 0, IPC_SEEK_BEG,
                                          nandSafeOpenCallback, block);
            }
        } else if (block->WORD_0x7C == 9) {
            if (result == IPC_RESULT_OK) {
                info->state = 3;
                block->callback(nandConvertErrorCode(IPC_RESULT_OK), block);
            } else {
                block->callback(nandConvertErrorCode(result), block);
            }
            return;
        }

        if (myResult != IPC_RESULT_OK) {
            block->callback(nandConvertErrorCode(myResult), block);
        }
        return;
    }

    block->callback(nandConvertErrorCode(result), block);

#undef block
}

static void nandReadOpenCallback(IPCResult result, void* arg) {
    NANDCommandBlock* block = (NANDCommandBlock*)arg;

    if (result >= 0) {
        block->info->fd = result;
        block->info->BYTE_0x89 = 2;
        block->info->state = 3;
        block->callback(NAND_RESULT_OK, block);
    } else {
        block->callback(nandConvertErrorCode(result), block);
    }
}

NANDResult NANDSafeCloseAsync(NANDFileInfo* info, NANDAsyncCallback callback,
                              NANDCommandBlock* block) {
    return nandSafeCloseAsync(info, callback, block);
}

// Not actually weak/inline, but IPA seems to
// not be aggressive enough to auto inline this.
static inline NANDResult nandSafeCloseAsync(NANDFileInfo* info,
                                            NANDAsyncCallback callback,
                                            NANDCommandBlock* block) {
    IPCResult result;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    if (info->state != 3) {
        return NAND_RESULT_INVALID;
    }

    if (info->access == NAND_ACCESS_READ) {
        block->info = info;
        block->callback = callback;
        result = ISFS_CloseAsync(info->fd, nandReadCloseCallback, block);
    } else if (info->access == NAND_ACCESS_WRITE ||
               info->access == NAND_ACCESS_RW) {
        block->info = info;
        block->callback = callback;
        block->WORD_0x7C = 10;
        result = ISFS_CloseAsync(info->fd, nandSafeCloseCallback, block);
    } else {
        result = IPC_RESULT_INVALID;
    }

    return nandConvertErrorCode(result);
}

static void nandSafeCloseCallback(IPCResult result, void* arg) {
    IPCResult myResult;
    NANDCommandBlock* block = (NANDCommandBlock*)arg;

    if (result == IPC_RESULT_OK) {
        NANDFileInfo* info = block->info;
        myResult = (IPCResult)-117;

        block->WORD_0x7C++;

        if (block->WORD_0x7C == 11) {
            info->BYTE_0x89 = 6;
            myResult =
                ISFS_CloseAsync(info->WORD_0x4, nandSafeCloseCallback, block);
        } else if (block->WORD_0x7C == 12) {
            info->BYTE_0x89 = 7;
            myResult = ISFS_RenameAsync(info->renamePath, info->openPath,
                                        nandSafeCloseCallback, block);
        } else if (block->WORD_0x7C == 13) {
            char parentDir[64];
            __memclr(parentDir, sizeof(parentDir));
            info->BYTE_0x89 = 8;
            nandGetParentDirectory(parentDir, info->renamePath);
            myResult =
                ISFS_DeleteAsync(parentDir, nandSafeCloseCallback, block);
        } else if (block->WORD_0x7C == 14) {
            info->BYTE_0x89 = 9;
            info->state = 4;
            block->callback(nandConvertErrorCode(result), block);
            return;
        }

        if (myResult != IPC_RESULT_OK) {
            block->callback(nandConvertErrorCode(myResult), block);
        }
        return;
    }

    block->callback(nandConvertErrorCode(result), block);
}

static void nandReadCloseCallback(IPCResult result, void* arg) {
    NANDCommandBlock* block = (NANDCommandBlock*)arg;

    if (result == IPC_RESULT_OK) {
        block->info->BYTE_0x89 = 7;
        block->info->state = 4;
    }

    block->callback(nandConvertErrorCode(result), block);
}

static void nandCloseCallback(IPCResult result, void* arg) {
    NANDCommandBlock* block = (NANDCommandBlock*)arg;

    if (result == IPC_RESULT_OK) {
        block->info->BYTE_0x89 = 7;
        block->info->state = 2;
    }

    block->callback(nandConvertErrorCode(result), block);
}

static u32 nandGetUniqueNumber(void) {
    static u32 s_counter = 0;

    BOOL enabled = OSDisableInterrupts();
    u32 num = s_counter++;
    OSRestoreInterrupts(enabled);

    return num;
}

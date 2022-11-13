#include "nand.h"
#include "NANDCore.h"

#include <FS/fs.h>
#include <stdio.h>
#include <string.h>

static void nandSplitPerm(u8, u32*, u32*, u32*);
static void nandGetStatusCallback(IPCResult, void*);
static void nandGetFileStatusAsyncCallback(IPCResult, void*);
static BOOL nandInspectPermission(u8);

static IPCResult nandCreate(const char* path, u8 perm, u8 attr,
                            NANDCommandBlock* block, BOOL async, BOOL priv) {
    char absPath[64];
    u32 perm0, perm1, perm2;

    __memclr(absPath, sizeof(absPath));

    perm0 = 0;
    perm1 = 0;
    perm2 = 0;

    nandGenerateAbsPath(absPath, path);

    if (!priv && nandIsPrivatePath(absPath)) {
        return IPC_RESULT_NO_PRIVATE_AUTH;
    }

    if (!nandInspectPermission(perm)) {
        return -0x65;
    }

    nandSplitPerm(perm, &perm0, &perm1, &perm2);

    if (async) {
        return ISFS_CreateFileAsync(absPath, attr, perm0, perm1, perm2,
                                    nandCallback, block);
    } else {
        return ISFS_CreateFile(absPath, attr, perm0, perm1, perm2);
    }
}

NANDResult NANDCreate(const char* path, u8 perm, u8 attr) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    return nandConvertErrorCode(
        nandCreate(path, perm, attr, NULL, FALSE, FALSE));
}

NANDResult NANDPrivateCreate(const char* path, u8 perm, u8 attr) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    return nandConvertErrorCode(
        nandCreate(path, perm, attr, NULL, FALSE, TRUE));
}

NANDResult NANDPrivateCreateAsync(const char* path, u8 perm, u8 attr,
                                  NANDAsyncCallback callback,
                                  NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    return nandConvertErrorCode(
        nandCreate(path, perm, attr, block, TRUE, TRUE));
}

static IPCResult nandDelete(const char* path, NANDCommandBlock* block,
                            BOOL async, BOOL priv) {
    char absPath[64];

    __memclr(absPath, sizeof(absPath));

    nandGenerateAbsPath(absPath, path);

    if (!priv && nandIsPrivatePath(absPath)) {
        return IPC_RESULT_NO_PRIVATE_AUTH;
    }

    if (async) {
        return ISFS_DeleteAsync(absPath, nandCallback, block);
    } else {
        return ISFS_Delete(absPath);
    }
}

NANDResult NANDDelete(const char* path) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    return nandConvertErrorCode(nandDelete(path, NULL, FALSE, FALSE));
}

NANDResult NANDPrivateDelete(const char* path) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    return nandConvertErrorCode(nandDelete(path, NULL, FALSE, TRUE));
}

NANDResult NANDPrivateDeleteAsync(const char* path, NANDAsyncCallback callback,
                                  NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    return nandConvertErrorCode(nandDelete(path, block, TRUE, TRUE));
}

NANDResult NANDRead(NANDFileInfo* info, void* buf, u32 len) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    return nandConvertErrorCode(ISFS_Read(info->fd, buf, len));
}

NANDResult NANDReadAsync(NANDFileInfo* info, void* buf, u32 len,
                         NANDAsyncCallback callback, NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    return nandConvertErrorCode(
        ISFS_ReadAsync(info->fd, buf, len, nandCallback, block));
}

NANDResult NANDWrite(NANDFileInfo* info, const void* buf, u32 len) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    return nandConvertErrorCode(ISFS_Write(info->fd, buf, len));
}

NANDResult NANDWriteAsync(NANDFileInfo* info, const void* buf, u32 len,
                          NANDAsyncCallback callback, NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    return nandConvertErrorCode(
        ISFS_WriteAsync(info->fd, buf, len, nandCallback, block));
}

static IPCResult nandSeek(s32 fd, s32 offset, NANDSeekMode whence,
                          NANDCommandBlock* block, BOOL async) {
    IPCSeekMode mode = (IPCSeekMode)-1;

    switch (whence) {
    case NAND_SEEK_BEG:
        mode = IPC_SEEK_BEG;
        break;
    case NAND_SEEK_CUR:
        mode = IPC_SEEK_CUR;
        break;
    case NAND_SEEK_END:
        mode = IPC_SEEK_END;
        break;
    }

    if (async) {
        return ISFS_SeekAsync(fd, offset, mode, nandCallback, block);
    } else {
        return ISFS_Seek(fd, offset, mode);
    }
}

NANDResult NANDSeek(NANDFileInfo* info, s32 offset, NANDSeekMode whence) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    return nandConvertErrorCode(
        nandSeek(info->fd, offset, whence, NULL, FALSE));
}

NANDResult NANDSeekAsync(NANDFileInfo* info, s32 offset, NANDSeekMode whence,
                         NANDAsyncCallback callback, NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    return nandConvertErrorCode(
        nandSeek(info->fd, offset, whence, block, TRUE));
}

static IPCResult nandCreateDir(const char* path, u8 perm, u8 attr,
                               NANDCommandBlock* block, BOOL async, BOOL priv) {
    char absPath[64];
    u32 perm0, perm1, perm2;

    __memclr(absPath, sizeof(absPath));

    nandGenerateAbsPath(absPath, path);

    if (!priv && nandIsPrivatePath(absPath)) {
        return IPC_RESULT_NO_PRIVATE_AUTH;
    }

    if (!nandInspectPermission(perm)) {
        return -0x65;
    }

    perm0 = 0;
    perm1 = 0;
    perm2 = 0;
    nandSplitPerm(perm, &perm0, &perm1, &perm2);

    if (async) {
        return ISFS_CreateDirAsync(absPath, attr, perm0, perm1, perm2,
                                   nandCallback, block);
    } else {
        return ISFS_CreateDir(absPath, attr, perm0, perm1, perm2);
    }
}

NANDResult NANDPrivateCreateDir(const char* path, u8 perm, u8 attr) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    return nandConvertErrorCode(
        nandCreateDir(path, perm, attr, NULL, FALSE, TRUE));
}

NANDResult NANDPrivateCreateDirAsync(const char* path, u8 perm, u8 attr,
                                     NANDAsyncCallback callback,
                                     NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    return nandConvertErrorCode(
        nandCreateDir(path, perm, attr, block, TRUE, TRUE));
}

static IPCResult nandMove(const char* from, const char* to,
                          NANDCommandBlock* block, BOOL async, BOOL priv) {
    char absPathFrom[64];
    char absPathTo[64];
    char relativeName[13];

    __memclr(absPathFrom, sizeof(absPathFrom));
    __memclr(absPathTo, sizeof(absPathTo));
    __memclr(relativeName, sizeof(relativeName));

    relativeName[12] = '\0';

    nandGenerateAbsPath(absPathFrom, from);
    nandGetRelativeName(relativeName, absPathFrom);
    nandGenerateAbsPath(absPathTo, to);

    if (strcmp(absPathTo, "\\") == 0) {
        sprintf(absPathTo, "\\%s", relativeName);
    } else {
        strcat(absPathTo, "\\");
        strcat(absPathTo, relativeName);
    }

    if (!priv &&
        (nandIsPrivatePath(absPathFrom) || nandIsPrivatePath(absPathTo))) {
        return IPC_RESULT_NO_PRIVATE_AUTH;
    }

    if (async) {
        return ISFS_RenameAsync(absPathFrom, absPathTo, nandCallback, block);
    } else {
        return ISFS_Rename(absPathFrom, absPathTo);
    }
}

NANDResult NANDMove(const char* from, const char* to) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    return nandConvertErrorCode(nandMove(from, to, NULL, FALSE, FALSE));
}

static IPCResult nandGetFileStatus(s32 fd, u32* pLength, u32* r5) {
    FSFileStats stats;
    IPCResult result;

    result = ISFS_GetFileStats(fd, &stats);
    if (result == IPC_RESULT_OK) {
        if (pLength != NULL) {
            *pLength = stats.length;
        }

        if (r5 != NULL) {
            *r5 = stats.WORD_0x4;
        }
    }

    return result;
}

NANDResult NANDGetLength(NANDFileInfo* info, u32* length) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    return nandConvertErrorCode(nandGetFileStatus(info->fd, length, NULL));
}

static IPCResult nandGetFileStatusAsync(s32 fd, NANDCommandBlock* block) {
    // Work buffer???
    return ISFS_GetFileStatsAsync(fd,
                                  (FSFileStats*)ROUND_UP_PTR(block->path, 32),
                                  nandGetFileStatusAsyncCallback, block);
}

static void nandGetFileStatusAsyncCallback(IPCResult result, void* arg) {
    // Implicit cast required
    NANDCommandBlock* block = arg;
    // Work buffer???
    FSFileStats* stats = (FSFileStats*)ROUND_UP_PTR(block->path, 32);

    if (result == IPC_RESULT_OK) {
        if (block->pLength != NULL) {
            *block->pLength = stats->length;
        }

        if (block->PTR_0x78 != NULL) {
            *block->PTR_0x78 = stats->WORD_0x4;
        }
    }

    block->callback(nandConvertErrorCode(result), arg);
}

NANDResult NANDGetLengthAsync(NANDFileInfo* info, u32* length,
                              NANDAsyncCallback callback,
                              NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    block->pLength = length;
    block->PTR_0x78 = NULL;
    return nandConvertErrorCode(nandGetFileStatusAsync(info->fd, block));
}

static void nandComposePerm(u8* perm, u32 perm0, u32 perm1, u32 perm2)
    __attribute__((never_inline)) {
    u32 _perm = 0;

    if (perm0 & 0x1) {
        _perm |= 0x10;
    }

    if (perm0 & 0x2) {
        _perm |= 0x20;
    }

    if (perm1 & 0x1) {
        _perm |= 0x4;
    }

    if (perm1 & 0x2) {
        _perm |= 0x8;
    }

    if (perm2 & 0x1) {
        _perm |= 0x1;
    }

    if (perm2 & 0x2) {
        _perm |= 0x2;
    }

    *perm = _perm;
}

static void nandSplitPerm(u8 perm, u32* perm0, u32* perm1, u32* perm2)
    __attribute__((never_inline)) {
    *perm0 = 0;
    *perm1 = 0;
    *perm2 = 0;

    if (perm & 0x10) {
        *perm0 |= 0x1;
    }

    if (perm & 0x20) {
        *perm0 |= 0x2;
    }

    if (perm & 0x4) {
        *perm1 |= 0x1;
    }

    if (perm & 0x8) {
        *perm1 |= 0x2;
    }

    if (perm & 0x1) {
        *perm2 |= 0x1;
    }

    if (perm & 0x2) {
        *perm2 |= 0x2;
    }
}

static IPCResult nandGetStatus(const char* path, FSFileAttr* attr,
                               NANDCommandBlock* block, BOOL async, BOOL priv) {
    IPCResult result;
    u32 sp1C;
    u32 perm0, perm1, perm2;
    char absPath[64];
    __memclr(absPath, sizeof(absPath));

    nandGenerateAbsPath(absPath, path);

    if (!priv && nandIsUnderPrivatePath(absPath)) {
        return IPC_RESULT_NO_PRIVATE_AUTH;
    }

    if (async) {
        return ISFS_GetAttrAsync(
            absPath, attr, &attr->BYTE_0x4, &block->WORD_0x20, &block->perm0,
            &block->perm1, &block->perm2, nandGetStatusCallback, block);
    } else {
        sp1C = 0;
        perm0 = 0;
        perm1 = 0;
        perm2 = 0;

        result = ISFS_GetAttr(absPath, attr, &attr->BYTE_0x4, &sp1C, &perm0,
                              &perm1, &perm2);

        if (result == IPC_RESULT_OK) {
            nandComposePerm(&attr->perm, perm0, perm1, perm2);
            attr->BYTE_0x6 = sp1C;
        }

        return result;
    }
}

static void nandGetStatusCallback(IPCResult result, void* arg) {
    NANDCommandBlock* block = (NANDCommandBlock*)arg;
    FSFileAttr* attr;

    if (result == IPC_RESULT_OK) {
        attr = block->attr;
        attr->BYTE_0x6 = block->WORD_0x20;
        nandComposePerm(&attr->perm, block->perm0, block->perm1, block->perm2);
    }

    block->callback(nandConvertErrorCode(result), block);
}

NANDResult NANDGetStatus(const char* path, FSFileAttr* attr) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    return nandConvertErrorCode(nandGetStatus(path, attr, NULL, FALSE, FALSE));
}

NANDResult NANDPrivateGetStatusAsync(const char* path, FSFileAttr* attr,
                                     NANDAsyncCallback callback,
                                     NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    block->attr = attr;
    return nandConvertErrorCode(nandGetStatus(path, attr, block, TRUE, TRUE));
}

void NANDSetUserData(NANDCommandBlock* block, void* data) {
    block->userData = data;
}

void* NANDGetUserData(NANDCommandBlock* block) { return block->userData; }

static BOOL nandInspectPermission(u8 perm) { return perm & 0x10; }

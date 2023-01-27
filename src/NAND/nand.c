#include <FS.h>
#include <NAND.h>

#include <stdio.h>
#include <string.h>

static void nandSplitPerm(u8, u32*, u32*, u32*);
static void nandGetStatusCallback(s32, void*);
static void nandGetFileStatusAsyncCallback(s32, void*);
static BOOL nandInspectPermission(u8);

static IPCResult nandCreate(const char* path, u8 perm, u8 attr,
                            NANDCommandBlock* block, BOOL async, BOOL priv) {
    char absPath[64];
    u32 ownerPerm, groupPerm, otherPerm;

    __memclr(absPath, sizeof(absPath));

    ownerPerm = 0;
    groupPerm = 0;
    otherPerm = 0;

    nandGenerateAbsPath(absPath, path);

    if (!priv && nandIsPrivatePath(absPath)) {
        return IPC_RESULT_ACCESS;
    }

    if (!nandInspectPermission(perm)) {
        return IPC_RESULT_INVALID;
    }

    nandSplitPerm(perm, &ownerPerm, &groupPerm, &otherPerm);

    if (async) {
        return ISFS_CreateFileAsync(absPath, attr, ownerPerm, groupPerm,
                                    otherPerm, nandCallback, block);
    } else {
        return ISFS_CreateFile(absPath, attr, ownerPerm, groupPerm, otherPerm);
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
        return IPC_RESULT_ACCESS;
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
    u32 ownerPerm, groupPerm, otherPerm;

    __memclr(absPath, sizeof(absPath));

    nandGenerateAbsPath(absPath, path);

    if (!priv && nandIsPrivatePath(absPath)) {
        return IPC_RESULT_ACCESS;
    }

    if (!nandInspectPermission(perm)) {
        return IPC_RESULT_INVALID;
    }

    ownerPerm = 0;
    groupPerm = 0;
    otherPerm = 0;
    nandSplitPerm(perm, &ownerPerm, &groupPerm, &otherPerm);

    if (async) {
        return ISFS_CreateDirAsync(absPath, attr, ownerPerm, groupPerm,
                                   otherPerm, nandCallback, block);
    } else {
        return ISFS_CreateDir(absPath, attr, ownerPerm, groupPerm, otherPerm);
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
        return IPC_RESULT_ACCESS;
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

static IPCResult nandGetFileStatus(s32 fd, u32* lengthOut, u32* positionOut) {
    FSFileStats stats;
    IPCResult result;

    result = ISFS_GetFileStats(fd, &stats);
    if (result == IPC_RESULT_OK) {
        if (lengthOut != NULL) {
            *lengthOut = stats.length;
        }

        if (positionOut != NULL) {
            *positionOut = stats.position;
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

static void nandGetFileStatusAsyncCallback(s32 result, void* arg) {
    // Implicit cast required
    NANDCommandBlock* block = arg;
    // Work buffer???
    FSFileStats* stats = (FSFileStats*)ROUND_UP_PTR(block->path, 32);

    if (result == IPC_RESULT_OK) {
        if (block->lengthOut != NULL) {
            *block->lengthOut = stats->length;
        }

        if (block->positionOut != NULL) {
            *block->positionOut = stats->position;
        }
    }

    block->callback(nandConvertErrorCode(result), arg);
}

NANDResult NANDGetLengthAsync(NANDFileInfo* info, u32* lengthOut,
                              NANDAsyncCallback callback,
                              NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    block->lengthOut = lengthOut;
    block->positionOut = NULL;
    return nandConvertErrorCode(nandGetFileStatusAsync(info->fd, block));
}

static void nandComposePerm(u8* out, u32 ownerPerm, u32 groupPerm,
                            u32 otherPerm) __attribute__((never_inline)) {
    u32 perm = 0;

    if (ownerPerm & NAND_ACCESS_READ) {
        perm |= NAND_PERM_RUSR;
    }

    if (ownerPerm & NAND_ACCESS_WRITE) {
        perm |= NAND_PERM_WUSR;
    }

    if (groupPerm & NAND_ACCESS_READ) {
        perm |= NAND_PERM_RGRP;
    }

    if (groupPerm & NAND_ACCESS_WRITE) {
        perm |= NAND_PERM_WGRP;
    }

    if (otherPerm & NAND_ACCESS_READ) {
        perm |= NAND_PERM_ROTH;
    }

    if (otherPerm & NAND_ACCESS_WRITE) {
        perm |= NAND_PERM_WOTH;
    }

    *out = perm;
}

static void nandSplitPerm(u8 perm, u32* ownerPerm, u32* groupPerm,
                          u32* otherPerm) __attribute__((never_inline)) {
    *ownerPerm = 0;
    *groupPerm = 0;
    *otherPerm = 0;

    if (perm & NAND_PERM_RUSR) {
        *ownerPerm |= NAND_ACCESS_READ;
    }

    if (perm & NAND_PERM_WUSR) {
        *ownerPerm |= NAND_ACCESS_WRITE;
    }

    if (perm & NAND_PERM_RGRP) {
        *groupPerm |= NAND_ACCESS_READ;
    }

    if (perm & NAND_PERM_WGRP) {
        *groupPerm |= NAND_ACCESS_WRITE;
    }

    if (perm & NAND_PERM_ROTH) {
        *otherPerm |= NAND_ACCESS_READ;
    }

    if (perm & NAND_PERM_WOTH) {
        *otherPerm |= NAND_ACCESS_WRITE;
    }
}

static IPCResult nandGetStatus(const char* path, FSFileAttr* fileAttr,
                               NANDCommandBlock* block, BOOL async, BOOL priv) {
    IPCResult result;
    u32 attr;
    u32 ownerPerm, groupPerm, otherPerm;
    char absPath[64];
    __memclr(absPath, sizeof(absPath));

    nandGenerateAbsPath(absPath, path);

    if (!priv && nandIsUnderPrivatePath(absPath)) {
        return IPC_RESULT_ACCESS;
    }

    if (async) {
        return ISFS_GetAttrAsync(
            absPath, &fileAttr->ownerId, &fileAttr->groupId, &block->attr,
            &block->ownerPerm, &block->groupPerm, &block->otherPerm,
            nandGetStatusCallback, block);
    } else {
        attr = 0;
        ownerPerm = 0;
        groupPerm = 0;
        otherPerm = 0;

        result = ISFS_GetAttr(absPath, &fileAttr->ownerId, &fileAttr->groupId,
                              &attr, &ownerPerm, &groupPerm, &otherPerm);

        if (result == IPC_RESULT_OK) {
            nandComposePerm(&fileAttr->perm, ownerPerm, groupPerm, otherPerm);
            fileAttr->attr = attr;
        }

        return result;
    }
}

static void nandGetStatusCallback(s32 result, void* arg) {
    NANDCommandBlock* block = (NANDCommandBlock*)arg;
    FSFileAttr* fileAttr;

    if (result == IPC_RESULT_OK) {
        fileAttr = block->fileAttr;
        fileAttr->attr = block->attr;
        nandComposePerm(&fileAttr->perm, block->ownerPerm, block->groupPerm,
                        block->otherPerm);
    }

    block->callback(nandConvertErrorCode(result), block);
}

NANDResult NANDGetStatus(const char* path, FSFileAttr* fileAttr) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    return nandConvertErrorCode(
        nandGetStatus(path, fileAttr, NULL, FALSE, FALSE));
}

NANDResult NANDPrivateGetStatusAsync(const char* path, FSFileAttr* fileAttr,
                                     NANDAsyncCallback callback,
                                     NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    block->fileAttr = fileAttr;
    return nandConvertErrorCode(
        nandGetStatus(path, fileAttr, block, TRUE, TRUE));
}

void NANDSetUserData(NANDCommandBlock* block, void* data) {
    block->userData = data;
}

void* NANDGetUserData(NANDCommandBlock* block) { return block->userData; }

static BOOL nandInspectPermission(u8 perm) { return perm & NAND_PERM_RUSR; }

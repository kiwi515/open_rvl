#define RFL_ACC_SAFE_BUFFER_SIZE 0x2000

#include "RFL_NANDAccess.h"

#include <string.h>

#include <MEM.h>
#include <OS.h>
#include <TRK/__mem.h>

static const char* scFileNames[RFL_ACCESS_MAX] = {
    "/shared2/menu/FaceLib/RFL_DB.dat", //!< RFL_ACCESS_DB
    "/shared2/menu/FaceLib/RFL_Res.dat" //!< RFL_ACCESS_RES
};

static const char* scFirstDirectory = "/shared2/menu";
static const char* scSecondDirectory = "/shared2/menu/FaceLib";

static const u8 scFilePermissions[RFL_ACCESS_MAX] = {
    0x3F, //!< RFL_ACCESS_DB
    0x15  //!< RFL_ACCESS_RES
};

static const u8 scFileAttributes[RFL_ACCESS_MAX] = {
    0x0, //!< RFL_ACCESS_DB
    0x0  //!< RFL_ACCESS_RES
};

static void opencallback_(NANDResult, NANDCommandBlock*);
static void createcallback_(NANDResult, NANDCommandBlock*);
static void close2opencallback_(NANDResult, NANDCommandBlock*);
static void readcallback_(NANDResult, NANDCommandBlock*);
static void writecallback_(NANDResult, NANDCommandBlock*);
static void readseekcallback_(NANDResult, NANDCommandBlock*);
static void writecallback_(NANDResult, NANDCommandBlock*);
static void writeseekcallback_(NANDResult, NANDCommandBlock*);
static void closecallback_(NANDResult, NANDCommandBlock*);
static void getlengthcallback_(NANDResult, NANDCommandBlock*);
static void deletecallback_(NANDResult, NANDCommandBlock*);
static void createdircallback1_(NANDResult, NANDCommandBlock*);
static void createdircallback2_(NANDResult, NANDCommandBlock*)
    __attribute__((never_inline));

void RFLiInitAccessInfo(MEMiHeapHead* heap) {
    u16 i;

    for (i = 0; i < RFL_ACCESS_MAX; i++) {
        RFLAccessInfo* info = RFLiGetAccInfo((RFLAccessType)i);
        memset(info, 0, sizeof(RFLAccessInfo));
        info->safeBuffer =
            MEMAllocFromExpHeapEx(heap, RFL_ACC_SAFE_BUFFER_SIZE, 32);
        OSCreateAlarm(&info->alarm);
    }
}

void RFLiExitAccessInfo(void) {
    u16 i;

    for (i = 0; i < RFL_ACCESS_MAX; i++) {
        RFLAccessInfo* info = RFLiGetAccInfo((RFLAccessType)i);
        OSCancelAlarm(&info->alarm);
    }
}

BOOL RFLiIsWorking(void) { return RFLiGetWorking(); }

void RFLiStartWorking(void) {
    const BOOL enabled = OSDisableInterrupts();

    RFLiSetWorking(TRUE);
    RFLiGetManager()->status = RFL_RESULT_BUSY;

    OSRestoreInterrupts(enabled);
}

static void startWorkingClose_(void) {
    const BOOL enabled = OSDisableInterrupts();

    RFLiSetWorking(TRUE);
    RFLiGetManager()->lastStatus = RFLiGetManager()->status;
    RFLiGetManager()->lastReason = RFLiGetManager()->reason;
    RFLiGetManager()->status = RFL_RESULT_BUSY;

    OSRestoreInterrupts(enabled);
}

static void endWorkingCloseReason_(RFLResult result, NANDResult reason) {
    const BOOL enabled = OSDisableInterrupts();

    RFLiSetWorking(FALSE);
    if (result == RFL_RESULT_OK) {
        RFLiGetManager()->status = RFLiGetManager()->lastStatus;
        // Typo? Reason set to old status
        RFLiGetManager()->reason = RFLiGetManager()->lastStatus;
    } else {
        RFLiGetManager()->status = result;
        RFLiGetManager()->reason = reason;
    }

    OSRestoreInterrupts(enabled);
}

static void endWorkingClose_(RFLResult result) {
    endWorkingCloseReason_(result, NAND_RESULT_OK);
}

void RFLiEndWorkingReason(RFLResult result, NANDResult reason) {
    BOOL enabled;

    switch (RFLiGetManager()->status) {
    case RFL_RESULT_BUSY:
    case RFL_RESULT_OK:
        enabled = OSDisableInterrupts();
        RFLiSetWorking(FALSE);
        RFLiGetManager()->status = result;
        RFLiGetManager()->reason = reason;
        OSRestoreInterrupts(enabled);
        break;
    default:
        break;
    }
}

void RFLiEndWorking(RFLResult result) {
    RFLiEndWorkingReason(result, NAND_RESULT_OK);
}

NANDCommandBlock* RFLiSetCommandBlock(RFLAccessType type, UNKWORD tag) {
    NANDCommandBlock* block = &RFLiGetAccInfo(type)->block;
    RFLAccessInfo* info = RFLiGetAccInfo(type);
    RFLAccessUserData* data = &info->userData;

    data->tag = tag;
    data->type = type;
    NANDSetUserData(block, data);
    return block;
}

RFLAccessType RFLiGetType(NANDCommandBlock* block) {
    RFLAccessUserData* data = (RFLAccessUserData*)NANDGetUserData(block);
    return data->type;
}

static void alarmCallback_(OSAlarm* alarm, OSContext* ctx) {
#pragma unused(ctx)

    RFLAlarmUserData* data = (RFLAlarmUserData*)OSGetAlarmUserData(alarm);
    if (RFLAvailable()) {
        RFLAlarmCallback alarmCallback =
            RFLiGetAccInfo(data->type)->alarmData.callback;
        alarmCallback(data->type);
    }
}

static void retry_(RFLAccessType type, u8 val, RFLAlarmCallback callback) {
    if (val < 30) {
        RFLAccessInfo* info = RFLiGetAccInfo(type);
        RFLAlarmUserData* alarmData = &info->alarmData;
        alarmData->callback = callback;
        alarmData->type = type;
        alarmData->BYTE_0x8 = val;
        OSCancelAlarm(&info->alarm);
        OSSetAlarmUserData(&info->alarm, &info->alarmData);
        OSSetAlarm(&info->alarm, OS_MSEC_TO_TICKS(50), alarmCallback_);
    } else {
        RFLiEndWorkingReason(RFL_RESULT_2, NAND_RESULT_BUSY);
    }
}

static void opencallback_(NANDResult result, NANDCommandBlock* block) {
    BOOL doCallback = TRUE;
    RFLAccessType type = RFLiGetType(block);
    RFLAccessInfo* info = RFLiGetAccInfo(type);

    switch (result) {
    case NAND_RESULT_OK:
        info->BYTE_0x1D8 = 1;
        RFLiEndWorking(RFL_RESULT_OK);
        break;
    case NAND_RESULT_MAXFILES:
    case NAND_RESULT_MAXFD:
    case NAND_RESULT_MAXBLOCKS:
    case NAND_RESULT_AUTHENTICATION:
    case NAND_RESULT_ECC_CRIT:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ACCESS:
        info->BYTE_0x1D8 = 0;
        RFLiEndWorkingReason(RFL_RESULT_2, result);
        break;
    case NAND_RESULT_NOEXISTS:
        info->BYTE_0x1D8 = 0;
        if (info->access == NAND_ACCESS_READ) {
            RFLiEndWorkingReason(RFL_RESULT_2, result);
        } else {
            result = NANDPrivateCreateAsync(info->path, info->perm, info->attr,
                                            createcallback_,
                                            RFLiSetCommandBlock(type, 10));
            switch (result) {
            case NAND_RESULT_OK:
                doCallback = FALSE;
                break;
            default:
                RFLiEndWorkingReason(RFL_RESULT_2, result);
            }
        }
        break;
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_ALLOC_FAILED:
    case NAND_RESULT_INVALID:
    default:
        info->BYTE_0x1D8 = 0;
        RFLiEndWorkingReason(RFL_RESULT_CRITICAL, result);
        break;
    }

    if (doCallback && info->callback != NULL) {
        info->callback();
    }
}

static void createcallback_(NANDResult result, NANDCommandBlock* block) {
    NANDResult reason;
    NANDCommandBlock* openBlock;
    BOOL doCallback = TRUE;
    RFLAccessType type = RFLiGetType(block);
    RFLAccessInfo* info = RFLiGetAccInfo(type);

    switch (result) {
    case NAND_RESULT_OK:
    case NAND_RESULT_EXISTS:
        openBlock = RFLiSetCommandBlock(type, 11);
        memset(info->safeBuffer, 0, RFL_ACC_SAFE_BUFFER_SIZE);
        reason = NANDPrivateSafeOpenAsync(
            info->path, (RFLAvailable() ? &RFLiGetAccInfo(type)->file : NULL),
            info->access, info->safeBuffer, RFL_ACC_SAFE_BUFFER_SIZE,
            opencallback_, openBlock);
        switch (reason) {
        case NAND_RESULT_OK:
            doCallback = FALSE;
            break;
        case NAND_RESULT_NOEXISTS:
        case NAND_RESULT_MAXFD:
        case NAND_RESULT_BUSY:
        case NAND_RESULT_ALLOC_FAILED:
        case NAND_RESULT_ACCESS:
            RFLiEndWorkingReason(RFL_RESULT_2, reason);
            break;
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_INVALID:
        default:
            RFLiEndWorkingReason(RFL_RESULT_CRITICAL, result);
            break;
        }
        break;
    case NAND_RESULT_MAXFILES:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFL_RESULT_2, result);
        break;
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_INVALID:
    case NAND_RESULT_ECC_CRIT:
    default:
        RFLiEndWorkingReason(RFL_RESULT_CRITICAL, result);
        break;
    }

    if (doCallback && info->callback != NULL) {
        info->callback();
    }
}

static void close2opencallback_(NANDResult result, NANDCommandBlock* block) {
    NANDResult reason;
    NANDCommandBlock* openBlock;
    BOOL doCallback = TRUE;
    RFLAccessType type = RFLiGetType(block);
    RFLAccessInfo* info = RFLiGetAccInfo(type);

    switch (result) {
    case NAND_RESULT_OK:
        openBlock = RFLiSetCommandBlock(type, 11);
        memset(info->safeBuffer, 0, RFL_ACC_SAFE_BUFFER_SIZE);
        reason = NANDPrivateSafeOpenAsync(
            info->path, (RFLAvailable() ? &RFLiGetAccInfo(type)->file : NULL),
            info->access, info->safeBuffer, RFL_ACC_SAFE_BUFFER_SIZE,
            opencallback_, openBlock);
        switch (reason) {
        case NAND_RESULT_OK:
            doCallback = FALSE;
            break;
        case NAND_RESULT_NOEXISTS:
        case NAND_RESULT_MAXFD:
        case NAND_RESULT_BUSY:
        case NAND_RESULT_ALLOC_FAILED:
        case NAND_RESULT_ACCESS:
            RFLiEndWorkingReason(RFL_RESULT_2, reason);
            break;
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_INVALID:
        default:
            RFLiEndWorkingReason(RFL_RESULT_CRITICAL, result);
            break;
        }
        break;
    case NAND_RESULT_MAXFILES:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFL_RESULT_2, result);
        break;
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFL_RESULT_CRITICAL, result);
        break;
    }

    if (doCallback && info->callback != NULL) {
        info->callback();
    }
}

RFLResult RFLiOpenAsync(RFLAccessType rflAccess, u8 nandAccess,
                        RFLAccessCallback callback) {
    NANDFileInfo* nandInfo;
    NANDCommandBlock* block;
    NANDResult reason;
    RFLAccessInfo* rflInfo;
    const char* filename;

    nandInfo = RFLAvailable() ? &RFLiGetAccInfo(rflAccess)->file : NULL;
    RFLiStartWorking();
    rflInfo = RFLiGetAccInfo(rflAccess);
    filename = scFileNames[rflAccess];

    rflInfo->callback = callback;
    strncpy(rflInfo->path, filename, FS_MAX_PATH);
    rflInfo->BYTE_0x44 = 0;
    rflInfo->access = nandAccess;
    rflInfo->perm = scFilePermissions[rflAccess];
    rflInfo->attr = scFileAttributes[rflAccess];

    block = RFLiSetCommandBlock(rflAccess, 11);
    if (rflInfo->BYTE_0x1D8 != 0) {
        reason = NANDSafeCloseAsync(nandInfo, close2opencallback_, block);
        switch (reason) {
        case NAND_RESULT_OK:
            break;
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_INVALID:
        case NAND_RESULT_CORRUPT:
        case NAND_RESULT_ACCESS:
        default:
            RFLiEndWorkingReason(RFL_RESULT_2, reason);
            break;
        }
    } else {
        memset(rflInfo->safeBuffer, 0, RFL_ACC_SAFE_BUFFER_SIZE);
        reason = NANDPrivateSafeOpenAsync(
            filename, nandInfo, nandAccess, rflInfo->safeBuffer,
            RFL_ACC_SAFE_BUFFER_SIZE, opencallback_, block);
        switch (reason) {
        case NAND_RESULT_OK:
            break;
        case NAND_RESULT_NOEXISTS:
            if (nandAccess == NAND_ACCESS_READ) {
                RFLiEndWorkingReason(RFL_RESULT_2, reason);
            } else {
                block = RFLiSetCommandBlock(rflAccess, 10);
                reason = NANDPrivateCreateAsync(filename, rflInfo->perm,
                                                rflInfo->attr, createcallback_,
                                                block);
                if (reason != NAND_RESULT_OK) {
                    RFLiEndWorkingReason(RFL_RESULT_2, reason);
                }
            }
            break;
        case NAND_RESULT_MAXFD:
        case NAND_RESULT_BUSY:
        case NAND_RESULT_ALLOC_FAILED:
        case NAND_RESULT_ACCESS:
            RFLiEndWorkingReason(RFL_RESULT_2, reason);
            break;
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_INVALID:
        default:
            RFLiEndWorkingReason(RFL_RESULT_CRITICAL, reason);
            break;
        }
    }

    return RFLGetAsyncStatus();
}

static void readcallback_(NANDResult result, NANDCommandBlock* block) {
    RFLAccessType type = RFLiGetType(block);
    RFLAccessInfo* info = RFLiGetAccInfo(type);
    BOOL error = FALSE;

    // Non-error result = buffer size
    if (result >= NAND_RESULT_OK) {
        if (result > info->rwSize) {
            error = TRUE;
            RFLiEndWorking(RFL_RESULT_3);
        } else if (result == info->rwSize) {
            RFLiEndWorking(RFL_RESULT_OK);
        } else {
            error = TRUE;
            RFLiEndWorking(RFL_RESULT_3);
        }
    } else {
        error = TRUE;
        switch (result) {
        case NAND_RESULT_ACCESS:
        case NAND_RESULT_ALLOC_FAILED:
        case NAND_RESULT_BUSY:
        case NAND_RESULT_CORRUPT:
        case NAND_RESULT_ECC_CRIT:
        case NAND_RESULT_AUTHENTICATION:
            RFLiEndWorkingReason(RFL_RESULT_2, result);
            break;
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_INVALID:
        default:
            RFLiEndWorkingReason(RFL_RESULT_CRITICAL, result);
            break;
        }
    }

    if (error) {
        memset(info->buffer, 0, info->rwSize);
    }

    if (info->callback != NULL) {
        info->callback();
    }
}

static void readseekcallback_(NANDResult result, NANDCommandBlock* block) {
    RFLAccessType type = RFLiGetType(block);
    RFLAccessInfo* info = RFLiGetAccInfo(type);
    BOOL doCallback = TRUE;

    // Non-error result = Seek offset
    if (result == info->offset) {
        NANDCommandBlock* block = RFLiSetCommandBlock(type, 13);
        NANDFileInfo* nandInfo =
            RFLAvailable() ? &RFLiGetAccInfo(type)->file : NULL;
        NANDResult reason =
            NANDReadAsync(nandInfo, info->buffer, ROUND_UP(info->rwSize, 32),
                          readcallback_, block);
        switch (reason) {
        case NAND_RESULT_OK:
            doCallback = FALSE;
            break;
        case NAND_RESULT_ACCESS:
        case NAND_RESULT_ALLOC_FAILED:
        case NAND_RESULT_BUSY:
        case NAND_RESULT_CORRUPT:
        case NAND_RESULT_ECC_CRIT:
            RFLiEndWorkingReason(RFL_RESULT_2, reason);
            break;
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_INVALID:
        default:
            RFLiEndWorkingReason(RFL_RESULT_CRITICAL, reason);
            break;
        }
    } else if (result >= NAND_RESULT_OK) {
        RFLiEndWorking(RFL_RESULT_3);
    } else {
        switch (result) {
        case NAND_RESULT_BUSY:
        case NAND_RESULT_ALLOC_FAILED:
        case NAND_RESULT_ACCESS:
            RFLiEndWorkingReason(RFL_RESULT_2, result);
            break;
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_INVALID:
        default:
            RFLiEndWorkingReason(RFL_RESULT_CRITICAL, result);
            break;
        }
    }

    if (doCallback && info->callback != NULL) {
        info->callback();
    }
}

RFLResult RFLiReadAsync(RFLAccessType type, void* buffer, u32 readSize,
                        RFLAccessCallback callback, s32 offset) {
    NANDCommandBlock* block;
    NANDResult reason;
    NANDFileInfo* nandInfo =
        RFLAvailable() ? &RFLiGetAccInfo(type)->file : NULL;
    RFLAccessInfo* info = RFLiGetAccInfo(type);
    RFLiStartWorking();

    info->callback = callback;
    info->buffer = buffer;
    info->rwSize = readSize;
    info->offset = offset;

    block = RFLiSetCommandBlock(type, 12);
    reason = NANDSeekAsync(nandInfo, offset, NAND_SEEK_BEG, readseekcallback_,
                           block);
    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFL_RESULT_2, reason);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFL_RESULT_CRITICAL, reason);
        break;
    }

    return RFLGetAsyncStatus();
}

static void retryWrite_(RFLAccessType type) {
    NANDResult reason;
    RFLAccessInfo* rflInfo = RFLiGetAccInfo(type);
    NANDCommandBlock* block = RFLiSetCommandBlock(type, 14);
    NANDFileInfo* nandInfo =
        RFLAvailable() ? &RFLiGetAccInfo(type)->file : NULL;

    reason = NANDWriteAsync(nandInfo, rflInfo->buffer, rflInfo->rwSize,
                            writecallback_, block);
    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        retry_(type, rflInfo->alarmData.BYTE_0x8 + 1, retryWrite_);
        break;
    case NAND_RESULT_MAXBLOCKS:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFL_RESULT_2, reason);
        break;
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFL_RESULT_CRITICAL, reason);
        break;
    }

    if (!RFLiIsWorking() && rflInfo->callback != NULL) {
        rflInfo->callback();
    }
}

static void writecallback_(NANDResult result, NANDCommandBlock* block) {
    RFLAccessType type = RFLiGetType(block);
    RFLAccessInfo* info = RFLiGetAccInfo(type);

    if (result == info->rwSize) {
        RFLiEndWorking(RFL_RESULT_OK);
    } else if (result >= NAND_RESULT_OK) {
        RFLiEndWorking(RFL_RESULT_4);
    } else {
        switch (result) {
        case NAND_RESULT_MAXBLOCKS:
        case NAND_RESULT_CORRUPT:
        case NAND_RESULT_ACCESS:
            RFLiEndWorkingReason(RFL_RESULT_2, result);
            break;
        case NAND_RESULT_BUSY:
        case NAND_RESULT_ALLOC_FAILED:
            retry_(type, info->alarmData.BYTE_0x8 + 1, retryWrite_);
            break;
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_INVALID:
        default:
            RFLiEndWorkingReason(RFL_RESULT_CRITICAL, result);
            break;
        }
    }

    if (info->callback != NULL) {
        info->callback();
    }
}

static void retryWriteSeek_(RFLAccessType type) {
    NANDResult reason;
    RFLAccessInfo* rflInfo = RFLiGetAccInfo(type);
    NANDCommandBlock* block = RFLiSetCommandBlock(type, 12);
    NANDFileInfo* nandInfo =
        RFLAvailable() ? &RFLiGetAccInfo(type)->file : NULL;

    reason = NANDSeekAsync(nandInfo, rflInfo->offset, NAND_SEEK_BEG,
                           writeseekcallback_, block);
    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        retry_(type, rflInfo->alarmData.BYTE_0x8 + 1, retryWriteSeek_);
        break;
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFL_RESULT_2, reason);
        break;
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFL_RESULT_CRITICAL, reason);
        break;
    }
}

static void writeseekcallback_inline(RFLAccessType type) {
    NANDCommandBlock* block = RFLiSetCommandBlock(type, 14);
    RFLAccessInfo* rflInfo = RFLiGetAccInfo(type);
    NANDFileInfo* nandInfo =
        RFLAvailable() ? &RFLiGetAccInfo(type)->file : NULL;
    NANDResult reason = NANDWriteAsync(nandInfo, rflInfo->buffer,
                                       rflInfo->rwSize, writecallback_, block);
    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        retry_(type, 0, retryWrite_);
        break;
    case NAND_RESULT_MAXBLOCKS:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFL_RESULT_2, reason);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFL_RESULT_CRITICAL, reason);
        break;
    }
}

static void writeseekcallback_(NANDResult result, NANDCommandBlock* block) {
    RFLAccessType type = RFLiGetType(block);
    RFLAccessInfo* rflInfo = RFLiGetAccInfo(type);
    BOOL doCallback = TRUE;

    if (result == rflInfo->offset) {
        writeseekcallback_inline(type);
        if (RFLiIsWorking()) {
            doCallback = FALSE;
        }
    } else if (result >= NAND_RESULT_OK) {
        RFLiEndWorking(RFL_RESULT_4);
    } else {
        switch (result) {
        case NAND_RESULT_BUSY:
        case NAND_RESULT_ALLOC_FAILED:
            retry_(type, 0, retryWriteSeek_);
            break;
        case NAND_RESULT_ACCESS:
            RFLiEndWorkingReason(RFL_RESULT_2, result);
            break;
        case NAND_RESULT_FATAL_ERROR:
        case NAND_RESULT_UNKNOWN:
        case NAND_RESULT_INVALID:
        default:
            RFLiEndWorkingReason(RFL_RESULT_CRITICAL, result);
            break;
        }
    }

    if (doCallback && rflInfo->callback != NULL) {
        rflInfo->callback();
    }
}

RFLResult RFLiWriteAsync(RFLAccessType type, void* buffer, u32 writeSize,
                         RFLAccessCallback callback, s32 offset) {
    NANDResult reason;
    NANDCommandBlock* block;
    RFLAccessInfo* rflInfo = RFLiGetAccInfo(type);
    NANDFileInfo* nandInfo =
        RFLAvailable() ? &RFLiGetAccInfo(type)->file : NULL;

    RFLiStartWorking();

    rflInfo->callback = callback;
    rflInfo->buffer = buffer;
    rflInfo->rwSize = writeSize;
    rflInfo->offset = offset;

    block = RFLiSetCommandBlock(type, 12);
    reason = NANDSeekAsync(nandInfo, offset, NAND_SEEK_BEG, writeseekcallback_,
                           block);

    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        retry_(type, 0, retryWriteSeek_);
        break;
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFL_RESULT_2, reason);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFL_RESULT_CRITICAL, reason);
        break;
    }

    return RFLGetAsyncStatus();
}

static NANDResult closecore_(RFLAccessType type) {
    NANDCommandBlock* block = RFLiSetCommandBlock(type, 15);
    NANDFileInfo* info = RFLAvailable() ? &RFLiGetAccInfo(type)->file : NULL;
    return NANDSafeCloseAsync(info, closecallback_, block);
}

static void closecallback_(NANDResult result, NANDCommandBlock* block) {
    RFLAccessType type = RFLiGetType(block);
    RFLAccessInfo* info = RFLiGetAccInfo(type);

    switch (result) {
    case NAND_RESULT_OK:
        info->BYTE_0x1D8 = 0;
        endWorkingClose_(RFL_RESULT_OK);
        break;
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        RFLiEndWorkingReason(RFL_RESULT_2, result);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    case NAND_RESULT_ACCESS:
    default:
        RFLiEndWorkingReason(RFL_RESULT_CRITICAL, result);
        break;
    }

    if (!RFLiIsWorking() && info->callback != NULL) {
        info->callback();
    }
}

// TO-DO: Fakematch
RFLResult RFLiCloseAsync(RFLAccessType type, RFLAccessCallback callback) {
    int new_var;
    RFLAccessInfo* info = RFLiGetAccInfo(type);
    NANDResult reason;

    startWorkingClose_();
    info->callback = callback;

    new_var = 0xFFFFFFFF;
    reason = (closecore_(type) & new_var) & 0xFFFFFFFFFFFFFFFFu;

    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        RFLiEndWorkingReason(RFL_RESULT_2, reason);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    case NAND_RESULT_ACCESS:
    default:
        RFLiEndWorkingReason(RFL_RESULT_CRITICAL, reason);
        break;
    }

    return RFLGetAsyncStatus();
}

static void getlengthcallback_(NANDResult result, NANDCommandBlock* block) {
    RFLAccessType type = RFLiGetType(block);
    RFLAccessInfo* info = RFLiGetAccInfo(type);

    switch (result) {
    case NAND_RESULT_OK:
        RFLiEndWorking(RFL_RESULT_OK);
        break;
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFL_RESULT_2, result);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFL_RESULT_CRITICAL, result);
        break;
    }

    if (info->callback != NULL) {
        info->callback();
    }
}

RFLResult RFLiGetLengthAsync(RFLAccessType type, u32* out,
                             RFLAccessCallback callback) {
    NANDResult reason;
    NANDCommandBlock* block;
    NANDFileInfo* nandInfo;
    RFLAccessInfo* rflInfo = RFLiGetAccInfo(type);

    RFLiStartWorking();

    rflInfo->callback = callback;
    rflInfo->buffer = out;

    block = RFLiSetCommandBlock(type, 16);
    nandInfo = RFLAvailable() ? &RFLiGetAccInfo(type)->file : NULL;
    reason = NANDGetLengthAsync(nandInfo, out, getlengthcallback_, block);

    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFL_RESULT_2, reason);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFL_RESULT_CRITICAL, reason);
        break;
    }

    return RFLGetAsyncStatus();
}

static void deletecallback_(NANDResult result, NANDCommandBlock* block) {
    RFLAccessType type = RFLiGetType(block);
    RFLAccessInfo* info = RFLiGetAccInfo(type);

    switch (result) {
    case NAND_RESULT_OK:
    case NAND_RESULT_NOEXISTS:
        RFLiEndWorking(RFL_RESULT_OK);
        break;
    case NAND_RESULT_OPENFD:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFL_RESULT_2, result);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFL_RESULT_CRITICAL, result);
        break;
    }

    if (info->callback != NULL) {
        info->callback();
    }
}

RFLResult RFLiDeleteAsync(RFLAccessType type, RFLAccessCallback callback) {
    NANDCommandBlock* block;
    NANDResult reason;
    RFLAccessInfo* info = RFLiGetAccInfo(type);

    RFLiStartWorking();
    info->callback = callback;

    block = RFLiSetCommandBlock(type, 17);
    reason = NANDPrivateDeleteAsync(scFileNames[type], deletecallback_, block);

    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_NOEXISTS:
        RFLiEndWorking(RFL_RESULT_OK);
        break;
    case NAND_RESULT_OPENFD:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
    case NAND_RESULT_ACCESS:
        RFLiEndWorkingReason(RFL_RESULT_2, reason);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    default:
        RFLiEndWorkingReason(RFL_RESULT_CRITICAL, reason);
        break;
    }

    return RFLGetAsyncStatus();
}

static void createDirCommon_(const char* dir, NANDAsyncCallback callback) {
    NANDCommandBlock* block = RFLiSetCommandBlock(RFL_ACCESS_DB, 18);
    NANDResult reason = NANDPrivateCreateDirAsync(
        dir, NAND_PERM_RALL | NAND_PERM_WALL, 0, callback, block);
    switch (reason) {
    case NAND_RESULT_OK:
        break;
    case NAND_RESULT_EXISTS:
        callback(NAND_RESULT_EXISTS, block);
        break;
    case NAND_RESULT_MAXFILES:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        RFLiEndWorkingReason(RFL_RESULT_2, reason);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    case NAND_RESULT_ECC_CRIT:
    case NAND_RESULT_ACCESS:
    default:
        RFLiEndWorkingReason(RFL_RESULT_CRITICAL, reason);
        break;
    }
}

static void createdircallback2_(NANDResult result, NANDCommandBlock* block) {
    RFLAccessType type = RFLiGetType(block);
    RFLAccessInfo* info = RFLiGetAccInfo(type);

    switch (result) {
    case NAND_RESULT_OK:
    case NAND_RESULT_EXISTS:
        RFLiEndWorking(RFL_RESULT_OK);
        break;
    case NAND_RESULT_MAXFILES:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        RFLiEndWorkingReason(RFL_RESULT_2, result);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    case NAND_RESULT_ECC_CRIT:
    case NAND_RESULT_ACCESS:
    default:
        RFLiEndWorkingReason(RFL_RESULT_CRITICAL, result);
        break;
    }

    if (RFLGetAsyncStatus() != RFL_RESULT_BUSY && info->callback != NULL) {
        info->callback();
    }
}

static void createdircallback1_(NANDResult result, NANDCommandBlock* block) {
    RFLAccessType type = RFLiGetType(block);
    RFLAccessInfo* info = RFLiGetAccInfo(type);

    switch (result) {
    case NAND_RESULT_OK:
    case NAND_RESULT_EXISTS:
        createDirCommon_(scSecondDirectory, createdircallback2_);
        break;
    case NAND_RESULT_MAXFILES:
    case NAND_RESULT_CORRUPT:
    case NAND_RESULT_BUSY:
    case NAND_RESULT_ALLOC_FAILED:
        RFLiEndWorkingReason(RFL_RESULT_2, result);
        break;
    case NAND_RESULT_FATAL_ERROR:
    case NAND_RESULT_UNKNOWN:
    case NAND_RESULT_INVALID:
    case NAND_RESULT_ECC_CRIT:
    case NAND_RESULT_ACCESS:
    default:
        RFLiEndWorkingReason(RFL_RESULT_CRITICAL, result);
        break;
    }

    if (RFLGetAsyncStatus() != RFL_RESULT_BUSY && info->callback != NULL) {
        info->callback();
    }
}

RFLResult RFLiCreateSaveDirAsync(RFLAccessCallback callback) {
    RFLAccessInfo* info = RFLiGetAccInfo(RFL_ACCESS_DB);

    RFLiStartWorking();
    info->callback = callback;
    createDirCommon_(scFirstDirectory, createdircallback1_);

    return RFLGetAsyncStatus();
}

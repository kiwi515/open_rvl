#include <RVLFaceLib/RVLFaceLibInternal.h>
#include <revolution/MEM.h>
#include <string.h>

#define RFL_SYSTEM_HEAP_SIZE 0x24800
#define RFL_WORK_SIZE 0x4B000
#define RFL_DELUXE_WORK_SIZE 0x64000

/**
 * These functions are inlined, but with their conditions inverted(?) from the
 * actual functions. To avoid manually inlining them, these defines will hide
 * the fake inlines.
 */
#define RFLiGetLoader_() (RFLAvailable() ? &RFLiGetManager()->loader : NULL)
#define RFLiGetLastReason_()                                                   \
    (RFLAvailable() ? RFLiGetManager()->lastReason : NAND_RESULT_OK)
#define RFLGetLastReason_()                                                    \
    RFLAvailable() ? RFLiGetLastReason_() : sRFLLastReason;

static const char* __RFLVersion =
    "<< RVL_SDK - RFL \trelease build: Jun  9 2007 17:25:33 (0x4199_60831) >>";

static const RFLiCoordinateData scCoordinate = {1, 2, 0, FALSE, FALSE, FALSE};

static RFLiManager* sRFLiManager = NULL;
static RFLErrcode sRFLLastErrCode = RFLErrcode_NotAvailable;
static u8 sRFLiFileBrokenType;
static s32 sRFLLastReason;

u32 RFLGetWorkSize(BOOL deluxeTex) {
    return deluxeTex ? RFL_DELUXE_WORK_SIZE + sizeof(RFLiManager)
                     : RFL_WORK_SIZE + sizeof(RFLiManager);
}

RFLErrcode RFLInitResAsync(void* workBuffer, void* resBuffer, u32 resSize,
                           BOOL deluxeTex) {
    u32 workSize;
    u32 heapSize;
    void* heapBuffer;
    RFLErrcode error;

    if (resBuffer == NULL) {
        return RFLErrcode_Fatal;
    }

    if (RFLiGetManager() == NULL) {
        OSRegisterVersion(__RFLVersion);

        workSize = deluxeTex ? RFL_DELUXE_WORK_SIZE : RFL_WORK_SIZE;
        memset(workBuffer, 0, workSize);

        sRFLiManager = (RFLiManager*)workBuffer;
        sRFLLastErrCode = RFLErrcode_NotAvailable;
        sRFLLastReason = NAND_RESULT_OK;
        sRFLiFileBrokenType = RFLiFileBrokenType_DBNotFound;
        sRFLiManager->workBuffer = (u8*)workBuffer + sizeof(RFLiManager);

        heapSize = deluxeTex ? RFL_DELUXE_WORK_SIZE - sizeof(RFLiManager)
                             : RFL_WORK_SIZE - sizeof(RFLiManager);

        RFLiGetManager()->rootHeap = MEMCreateExpHeapEx(
            RFLiGetManager()->workBuffer, heapSize, MEM_HEAP_OPT_CLEAR_ALLOC);

        heapBuffer = MEMAllocFromExpHeapEx(RFLiGetManager()->rootHeap,
                                           RFL_SYSTEM_HEAP_SIZE, 32);
        RFLiGetManager()->systemHeap = MEMCreateExpHeapEx(
            heapBuffer, RFL_SYSTEM_HEAP_SIZE, MEM_HEAP_OPT_CLEAR_ALLOC);

        heapSize = MEMGetAllocatableSizeForExpHeap(RFLiGetManager()->rootHeap);
        heapBuffer =
            MEMAllocFromExpHeapEx(RFLiGetManager()->rootHeap, heapSize, 8);
        RFLiGetManager()->tmpHeap =
            MEMCreateExpHeapEx(heapBuffer, heapSize, MEM_HEAP_OPT_CLEAR_ALLOC);

        RFLiGetManager()->lastErrCode = RFLErrcode_Success;
        RFLiGetManager()->beforeCloseErr = RFLErrcode_Success;
        RFLiGetManager()->lastReason = NAND_RESULT_OK;
        RFLiGetManager()->beforeCloseReason = NAND_RESULT_OK;
        RFLiGetManager()->deluxeTex = deluxeTex;
        RFLiGetManager()->brokenType = RFLiFileBrokenType_DBNotFound;

        RFLSetIconDrawDoneCallback(NULL);
        RFLiSetWorking(FALSE);
        RFLiInitDatabase(RFLiGetManager()->systemHeap);
        RFLiInitLoader();
        RFLiInitCtrlBuf(RFLiGetManager()->systemHeap);
        RFLiInitHiddenDatabase();
        RFLiInitAccessInfo(RFLiGetManager()->systemHeap);
        RFLiSetCoordinateData(&scCoordinate);

        if (resBuffer != NULL) {
            RFLiLoader* loader = RFLiGetLoader_();
            loader->cached = TRUE;
            loader->cacheSize = resSize;
            loader->cache = resBuffer;
        }

        error = RFLiBootLoadAsync();
        if (error != RFLErrcode_Busy && error != RFLErrcode_Success) {
            RFLExit();
        }
    } else {
        error = RFLErrcode_Success;
    }

    return error;
}

RFLErrcode RFLInitRes(void* workBuffer, void* resBuffer, u32 resSize,
                      BOOL deluxeTex) {
    RFLInitResAsync(workBuffer, resBuffer, resSize, deluxeTex);
    return RFLWaitAsync();
}

void RFLExit(void) {
    if (RFLiGetManager() == NULL) {
        return;
    }

    RFLWaitAsync();

    sRFLLastErrCode = RFLGetAsyncStatus();
    sRFLLastReason = RFLGetLastReason_();
    sRFLiFileBrokenType = RFLiGetManager()->brokenType;

    if (RFLIsResourceCached()) {
        RFLFreeCachedResource();
    }

    RFLiExitAccessInfo();

    MEMDestroyExpHeap(RFLiGetManager()->tmpHeap);
    MEMDestroyExpHeap(RFLiGetManager()->systemHeap);
    MEMDestroyExpHeap(RFLiGetManager()->rootHeap);

    sRFLiManager = NULL;
}

static void bootloadDB2Res_(void) {
    RFLiLoadResourceHeaderAsync();

    switch (RFLWaitAsync()) {
    case RFLErrcode_Loadfail:
    case RFLErrcode_Success:
    case RFLErrcode_Busy:
        break;
    default:
        RFLExit();
    }
}

RFLErrcode RFLiBootLoadAsync(void) {
    return RFLiBootLoadDatabaseAsync(bootloadDB2Res_);
}

BOOL RFLAvailable(void) { return sRFLiManager != NULL; }

static void* allocal_(u32 size, s32 align) {
    return MEMAllocFromExpHeapEx(RFLiGetManager()->tmpHeap, size, align);
}

void* RFLiAlloc(u32 size) { return allocal_(size, 8); }

void* RFLiAlloc32(u32 size) { return allocal_(size, 32); }

void RFLiFree(void* block) {
    MEMFreeToExpHeap(RFLiGetManager()->tmpHeap, block);
}

RFLiDBManager* RFLiGetDBManager(void) {
    return !RFLAvailable() ? NULL : &RFLiGetManager()->dbMgr;
}

RFLiHDBManager* RFLiGetHDBManager(void) {
    return !RFLAvailable() ? NULL : &RFLiGetManager()->hdbMgr;
}

RFLiLoader* RFLiGetLoader(void) {
    return !RFLAvailable() ? NULL : &RFLiGetManager()->loader;
}

BOOL RFLiGetWorking(void) {
    return !RFLAvailable() ? NULL : RFLiGetManager()->working;
}

void RFLiSetWorking(BOOL working) { RFLiGetManager()->working = working; }

RFLiManager* RFLiGetManager(void) { return sRFLiManager; }

RFLErrcode RFLGetAsyncStatus(void) {
    if (!RFLAvailable()) {
        return sRFLLastErrCode;
    }

    if (RFLiIsWorking()) {
        return RFLErrcode_Busy;
    }

    if (RFLiCriticalError()) {
        return RFLErrcode_Fatal;
    }

    return RFLiGetManager()->lastErrCode;
}

s32 RFLGetLastReason(void) {
    return !RFLAvailable() ? sRFLLastReason : RFLiGetLastReason_();
}

RFLErrcode RFLWaitAsync(void) {
    volatile RFLErrcode status;

    do {
        status = RFLGetAsyncStatus();
    } while (status == RFLErrcode_Busy);

    return status;
}

RFLiAccessInfo* RFLiGetAccInfo(RFLiFileType type) {
    return !RFLAvailable() ? NULL : &RFLiGetManager()->info[type];
}

RFLiCtrlBufManager* RFLiGetCtrlBufManager(void) {
    return !RFLAvailable() ? NULL : &RFLiGetManager()->ctrlMgr;
}

s32 RFLiGetLastReason(void) {
    return !RFLAvailable() ? NAND_RESULT_OK : RFLiGetManager()->lastReason;
}

void RFLiSetFileBroken(RFLiFileBrokenType type) {
    if (RFLAvailable()) {
        RFLiGetManager()->brokenType |= (1 << type) & 0xFF;
    }
}

BOOL RFLiNotFoundError(void) {
    u8* broken = &sRFLiFileBrokenType;

    if (RFLAvailable()) {
        broken = &RFLiGetManager()->brokenType;
    }

    return *broken >> RFLiFileBrokenType_DBNotFound & 1;
}

BOOL RFLiNeedRepairError(void) {
    u8* broken = &sRFLiFileBrokenType;

    if (RFLAvailable()) {
        broken = &RFLiGetManager()->brokenType;
    }

    return *broken >> RFLiFileBrokenType_DBBroken & 1;
}

BOOL RFLiCriticalError(void) {
    u8* broken = &sRFLiFileBrokenType;

    if (RFLAvailable()) {
        broken = &RFLiGetManager()->brokenType;
    }

    return *broken & 1 << RFLiFileBrokenType_ResBroken ||
           *broken & 1 << RFLiFileBrokenType_Corrupt;
}

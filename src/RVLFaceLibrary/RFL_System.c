#define RFL_SYSTEM_HEAP_SIZE 0x24800
#define RFL_WORK_SIZE 0x4B000
#define RFL_DELUXE_WORK_SIZE 0x64000

#include <RVLFaceLibrary.h>
#include <revolution/MEM.h>
#include <revolution/OS.h>
#include <string.h>

static const char* __RFLVersion =
    "<< RVL_SDK - RFL \trelease build: Jun  9 2007 17:25:33 (0x4199_60831) >>";

static const RFLCoordinateData scCoordinate = {
    0x01, 0x02, 0x00, 0x00, 0x00000000, 0x00000000, 0x00000000};

static RFLManager* sRFLManager = NULL;
static RFLErrcode sRFLLastErrCode = RFLErrcode_NotAvailable;
static u8 sRFLiFileBrokenType;
static NANDResult sRFLLastReason;

u32 RFLGetWorkSize(BOOL deluxTex) {
    return deluxTex ? RFL_DELUXE_WORK_SIZE + sizeof(RFLManager)
                    : RFL_WORK_SIZE + sizeof(RFLManager);
}

RFLErrcode RFLInitResAsync(void* workBuffer, void* resBuffer, u32 resSize,
                           BOOL deluxTex) {
    u32 workSize;
    u32 heapSize;
    void* heapBuffer;
    RFLErrcode error;

    if (resBuffer == NULL) {
        return RFLErrcode_Fatal;
    }

    if (RFLiGetManager() == NULL) {
        OSRegisterVersion(__RFLVersion);

        workSize = deluxTex ? RFL_DELUXE_WORK_SIZE : RFL_WORK_SIZE;
        memset(workBuffer, 0, workSize);

        sRFLManager = (RFLManager*)workBuffer;
        sRFLLastErrCode = RFLErrcode_NotAvailable;
        sRFLLastReason = NAND_RESULT_OK;
        sRFLiFileBrokenType = 0;
        sRFLManager->workBuffer = (u8*)workBuffer + sizeof(RFLManager);

        heapSize = deluxTex ? RFL_DELUXE_WORK_SIZE - sizeof(RFLManager)
                            : RFL_WORK_SIZE - sizeof(RFLManager);

        RFLiGetManager()->rootHeap =
            MEMCreateExpHeapEx(RFLiGetManager()->workBuffer, heapSize, 0x1);

        heapBuffer = MEMAllocFromExpHeapEx(RFLiGetManager()->rootHeap,
                                           RFL_SYSTEM_HEAP_SIZE, 32);
        RFLiGetManager()->systemHeap =
            MEMCreateExpHeapEx(heapBuffer, RFL_SYSTEM_HEAP_SIZE, 0x1);

        heapSize = MEMGetAllocatableSizeForExpHeap(RFLiGetManager()->rootHeap);
        heapBuffer =
            MEMAllocFromExpHeapEx(RFLiGetManager()->rootHeap, heapSize, 8);
        RFLiGetManager()->tmpHeap =
            MEMCreateExpHeapEx(heapBuffer, heapSize, 0x1);

        RFLiGetManager()->status = RFLErrcode_Success;
        RFLiGetManager()->lastStatus = RFLErrcode_Success;
        RFLiGetManager()->reason = NAND_RESULT_OK;
        RFLiGetManager()->lastReason = NAND_RESULT_OK;
        RFLiGetManager()->deluxTex = deluxTex;
        RFLiGetManager()->brokenType = 0;
        RFLSetIconDrawDoneCallback(NULL);
        RFLiSetWorking(FALSE);
        RFLiInitDatabase(RFLiGetManager()->systemHeap);
        RFLiInitLoader();
        RFLiInitCtrlBuf(RFLiGetManager()->systemHeap);
        RFLiInitHiddenDatabase();
        RFLiInitAccessInfo(RFLiGetManager()->systemHeap);
        RFLiSetCoordinateData(&scCoordinate);

        if (resBuffer != NULL) {
            RFLLoader* loader =
                RFLAvailable() ? &RFLiGetManager()->loader : NULL;
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
                      BOOL deluxTex) {
    RFLInitResAsync(workBuffer, resBuffer, resSize, deluxTex);
    return RFLWaitAsync();
}

void RFLExit(void) {
    if (RFLiGetManager() == NULL) {
        return;
    }

    RFLWaitAsync();
    sRFLLastErrCode = RFLGetAsyncStatus();
    sRFLLastReason = RFLAvailable() ? (RFLAvailable() ? RFLiGetManager()->reason
                                                      : NAND_RESULT_OK)
                                    : sRFLLastReason;
    sRFLiFileBrokenType = RFLiGetManager()->brokenType;

    if (RFLIsResourceCached()) {
        RFLFreeCachedResource();
    }

    RFLiExitAccessInfo();

    MEMDestroyExpHeap(RFLiGetManager()->tmpHeap);
    MEMDestroyExpHeap(RFLiGetManager()->systemHeap);
    MEMDestroyExpHeap(RFLiGetManager()->rootHeap);
    sRFLManager = NULL;
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

BOOL RFLAvailable(void) { return sRFLManager != NULL; }

static void* allocal_(u32 size, s32 align) {
    return MEMAllocFromExpHeapEx(RFLiGetManager()->tmpHeap, size, align);
}

void* RFLiAlloc(u32 size) { return allocal_(size, 8); }

void* RFLiAlloc32(u32 size) { return allocal_(size, 32); }

void RFLiFree(void* mem) { MEMFreeToExpHeap(RFLiGetManager()->tmpHeap, mem); }

RFLDBManager* RFLiGetDBManager(void) {
    return !RFLAvailable() ? NULL : &RFLiGetManager()->dbMgr;
}

RFLHDBManager* RFLiGetHDBManager(void) {
    return !RFLAvailable() ? NULL : &RFLiGetManager()->hdbMgr;
}

RFLLoader* RFLiGetLoader(void) {
    return !RFLAvailable() ? NULL : &RFLiGetManager()->loader;
}

BOOL RFLiGetWorking(void) {
    return !RFLAvailable() ? NULL : RFLiGetManager()->working;
}

void RFLiSetWorking(BOOL working) { RFLiGetManager()->working = working; }

RFLManager* RFLiGetManager(void) { return sRFLManager; }

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

    return RFLiGetManager()->status;
}

NANDResult RFLGetLastReason(void) {
    return !RFLAvailable()
               ? sRFLLastReason
               : (RFLAvailable() ? RFLiGetManager()->reason : NAND_RESULT_OK);
}

RFLErrcode RFLWaitAsync(void) {
    volatile RFLErrcode status;

    do {
        status = RFLGetAsyncStatus();
    } while (status == RFLErrcode_Busy);

    return status;
}

RFLAccessInfo* RFLiGetAccInfo(RFLiFileType type) {
    return !RFLAvailable() ? NULL : &RFLiGetManager()->info[type];
}

RFLCtrlBufManager* RFLiGetCtrlBufManager(void) {
    return !RFLAvailable() ? NULL : &RFLiGetManager()->ctrlMgr;
}

NANDResult RFLiGetLastReason(void) {
    return !RFLAvailable() ? NAND_RESULT_OK : RFLiGetManager()->reason;
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

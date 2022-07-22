#pragma ipa file

#define RFL_SYSTEM_HEAP_SIZE 0x24800
#define RFL_WORK_SIZE 0x4B000
#define RFL_DELUXE_WORK_SIZE 0x64000

#include "RFL_System.h"
#include "RFL_Controller.h"
#include "RFL_Database.h"
#include "RFL_HiddenDatabase.h"
#include "RFL_Icon.h"
#include "RFL_Model.h"
#include "RFL_NANDAccess.h"
#include "RFL_NANDLoader.h"

#include <MEM/mem_expHeap.h>
#include <OS/OS.h>
#include <OS/OSError.h>
#include <TRK/__mem.h>

static const char* __RFLVersion =
    "<< RVL_SDK - RFL \trelease build: Jun  9 2007 17:25:33 (0x4199_60831) >>";

static const RFLCoordinateData scCoordinate = {0x01200000, 0x00000000,
                                               0x00000000, 0x00000000};

static RFLiManager* sRFLManager = NULL;
static RFLError sRFLLastErrCode = RFL_ERR_1;
static u8 sRFLBrokenType;
static RFLReason sRFLLastReason;

u32 RFLGetWorkSize(BOOL deluxTex) {
    return deluxTex ? RFL_DELUXE_WORK_SIZE + sizeof(RFLiManager)
                    : RFL_WORK_SIZE + sizeof(RFLiManager);
}

RFLError RFLInitResAsync(void* workBuffer, void* resBuffer, u32 resSize,
                         BOOL deluxTex) {
    u32 workSize;
    u32 heapSize;
    void* heapBuffer;
    RFLError error;

    if (resBuffer == NULL) {
        return RFL_ERR_CRITICAL;
    }

    if (RFLiGetManager() == NULL) {
        OSRegisterVersion(__RFLVersion);

        workSize = deluxTex ? RFL_DELUXE_WORK_SIZE : RFL_WORK_SIZE;
        memset(workBuffer, 0, workSize);

        sRFLManager = (RFLiManager*)workBuffer;
        sRFLLastErrCode = RFL_ERR_1;
        sRFLLastReason = RFL_REASON_0;
        sRFLBrokenType = 0;
        sRFLManager->workBuffer = (u8*)workBuffer + sizeof(RFLiManager);

        heapSize = deluxTex ? RFL_DELUXE_WORK_SIZE - sizeof(RFLiManager)
                            : RFL_WORK_SIZE - sizeof(RFLiManager);

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

        RFLiGetManager()->status = RFL_ERR_NONE;
        RFLiGetManager()->WORD_0x1B44 = 0;
        RFLiGetManager()->reason = RFL_REASON_0;
        RFLiGetManager()->WORD_0x1B4C = 0;
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
            RFLiLoader* loader =
                RFLAvailable() ? &RFLiGetManager()->loader : NULL;
            loader->WORD_0x98 = 1;
            loader->resSize = resSize;
            loader->resBuffer = resBuffer;
        }

        error = RFLiBootLoadAsync();
        if (error != RFL_ERR_BUSY && error != RFL_ERR_NONE) {
            RFLExit();
        }
    } else {
        error = RFL_ERR_NONE;
    }

    return error;
}

RFLError RFLInitRes(void* workBuffer, void* resBuffer, u32 resSize,
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
                                                      : RFL_REASON_0)
                                    : sRFLLastReason;
    sRFLBrokenType = RFLiGetManager()->brokenType;

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
    case RFL_ERR_3:
    case RFL_ERR_NONE:
    case RFL_ERR_BUSY:
        break;
    default:
        RFLExit();
    }
}

RFLError RFLiBootLoadAsync(void) {
    return RFLiBootLoadDatabaseAsync(bootloadDB2Res_);
}

BOOL RFLAvailable(void) { return sRFLManager != NULL; }

static void* allocal_(u32 size, s32 align) {
    return MEMAllocFromExpHeapEx(RFLiGetManager()->tmpHeap, size, align);
}

void* RFLiAlloc(u32 size) { return allocal_(size, 8); }

void* RFLiAlloc32(u32 size) { return allocal_(size, 32); }

void RFLiFree(void* mem) { MEMFreeToExpHeap(RFLiGetManager()->tmpHeap, mem); }

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

RFLiManager* RFLiGetManager(void) { return sRFLManager; }

RFLError RFLGetAsyncStatus(void) {
    if (!RFLAvailable()) {
        return sRFLLastErrCode;
    }

    if (RFLiIsWorking()) {
        return RFL_ERR_BUSY;
    }

    if (RFLiCriticalError()) {
        return RFL_ERR_CRITICAL;
    }

    return RFLiGetManager()->status;
}

RFLReason RFLGetLastReason(void) {
    return !RFLAvailable()
               ? sRFLLastReason
               : (RFLAvailable() ? RFLiGetManager()->reason : RFL_REASON_0);
}

RFLError RFLWaitAsync(void) {
    volatile RFLError status;

    do {
        status = RFLGetAsyncStatus();
    } while (status == RFL_ERR_BUSY);

    return status;
}

RFLiAccInfo* RFLiGetAccInfo(RFLAccessType type) {
    return !RFLAvailable() ? NULL : &RFLiGetManager()->accInfo[type];
}

RFLiCtrlBufManager* RFLiGetCtrlBufManager(void) {
    return !RFLAvailable() ? NULL : &RFLiGetManager()->ctrlBufMgr;
}

RFLReason RFLiGetLastReason(void) {
    return !RFLAvailable() ? RFL_REASON_0 : RFLiGetManager()->reason;
}

void RFLiSetFileBroken(RFLBrokenType type) {
    if (RFLAvailable()) {
        RFLiGetManager()->brokenType |= (1 << type) & 0xFF;
    }
}

BOOL RFLiNotFoundError(void) {
    u8* broken = &sRFLBrokenType;

    if (RFLAvailable()) {
        broken = &RFLiGetManager()->brokenType;
    }

    return *broken >> RFL_BROKEN_NOT_FOUND & 1;
}

BOOL RFLiNeedRepairError(void) {
    u8* broken = &sRFLBrokenType;

    if (RFLAvailable()) {
        broken = &RFLiGetManager()->brokenType;
    }

    return *broken >> RFL_BROKEN_NEED_REPAIR & 1;
}

BOOL RFLiCriticalError(void) {
    u8* broken = &sRFLBrokenType;

    if (RFLAvailable()) {
        broken = &RFLiGetManager()->brokenType;
    }

    return *broken & 1 << RFL_BROKEN_IO_ERROR || *broken & 1 << RFL_BROKEN_3;
}

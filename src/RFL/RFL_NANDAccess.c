#define RFL_ACC_SAFE_BUFFER_SIZE 0x2000

#include "RFL_NANDAccess.h"

#include <MEM/mem_expHeap.h>
#include <NAND/nand.h>
#include <OS/OSInterrupt.h>
#include <TRK/__mem.h>

void RFLiInitAccessInfo(MEMiHeapHead* heap) {
    u16 i;

    for (i = 0; i < RFL_ACCESS_MAX; i++) {
        RFLAccInfo* info;
        void* safeBuffer;

        info = RFLiGetAccInfo((RFLAccessType)i);
        memset(info, 0, sizeof(RFLAccInfo));
        info->safeBuffer =
            MEMAllocFromExpHeapEx(heap, RFL_ACC_SAFE_BUFFER_SIZE, 32);
        OSCreateAlarm(&info->alarm);
    }
}

void RFLiExitAccessInfo(void) {
    u16 i;

    for (i = 0; i < RFL_ACCESS_MAX; i++) {
        RFLAccInfo* info;

        info = RFLiGetAccInfo((RFLAccessType)i);
        OSCancelAlarm(&info->alarm);
    }
}

BOOL RFLiIsWorking(void) { return RFLiGetWorking(); }

void RFLiStartWorking(void) {
    u32 intr;

    intr = OSDisableInterrupts();
    RFLiSetWorking(TRUE);
    RFLiGetManager()->status = RFL_RESULT_BUSY;
    OSRestoreInterrupts(intr);
}

void RFLiEndWorkingReason(RFLResult result, RFLReason reason) {
    u32 intr;

    switch (RFLiGetManager()->status) {
    case RFL_RESULT_BUSY:
    case RFL_RESULT_OK:
        intr = OSDisableInterrupts();
        RFLiSetWorking(FALSE);
        RFLiGetManager()->status = result;
        RFLiGetManager()->reason = reason;
        OSRestoreInterrupts(intr);
        break;
    default:
        break;
    }
}

void RFLiEndWorking(RFLResult result) {
    RFLiEndWorkingReason(result, RFL_REASON_NONE);
}

NANDFileInfo* RFLiGetWorkingFile(RFLAccessType type) {
    return !RFLAvailable() ? NULL : &RFLiGetAccInfo(type)->file;
}

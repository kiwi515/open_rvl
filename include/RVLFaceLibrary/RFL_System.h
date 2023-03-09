#ifndef RVL_FACE_LIBRARY_SYSTEM_H
#define RVL_FACE_LIBRARY_SYSTEM_H
#include <RVLFaceLibrary/RFL_Controller.h>
#include <RVLFaceLibrary/RFL_Database.h>
#include <RVLFaceLibrary/RFL_HiddenDatabase.h>
#include <RVLFaceLibrary/RFL_NANDAccess.h>
#include <RVLFaceLibrary/RFL_NANDLoader.h>
#include <RVLFaceLibrary/RFL_Types.h>
#include <revolution/CARD.h>
#include <revolution/MEM.h>
#include <revolution/NAND.h>
#include <revolution/OS.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct RFLManager {
    void* workBuffer;                     // at 0x0
    MEMiHeapHead* rootHeap;               // at 0x4
    MEMiHeapHead* systemHeap;             // at 0x8
    MEMiHeapHead* tmpHeap;                // at 0xC
    RFLDBManager dbMgr;                   // at 0x10
    RFLLoader loader;                     // at 0xC0
    RFLHDBManager hdbMgr;                 // at 0x16C
    RFLCtrlBufManager ctrlMgr;            // at 0x1AAC
    BOOL working;                         // at 0x1B34
    BOOL deluxTex;                        // at 0x1B38
    u8 brokenType;                        // at 0x1B3C
    RFLErrcode lastErrCode;               // at 0x1B40
    RFLErrcode beforeCloseErr;            // at 0x1B44
    NANDResult lastReason;                // at 0x1B48
    NANDResult beforeCloseReason;         // at 0x1B4C
    RFLAccessInfo info[RFLiFileType_Max]; // at 0x1B50
    RFLSystemCallback systemCb;           // at 0x1F10
    RFLIconDrawDoneCallback iconDrawCb;   // at 0x1F14
    RFLModelDrawDoneCallback modelDrawCb; // at 0x1F18
    // TODO: Can this be changed to 8 bytes? Is this u8 macaddr[8]?
    char UNK_0x1F1C[0x4];
} RFLManager;

u32 RFLGetWorkSize(BOOL);
RFLErrcode RFLInitResAsync(void*, void*, u32, BOOL);
RFLErrcode RFLInitRes(void*, void*, u32, BOOL);
void RFLExit(void);
RFLErrcode RFLiBootLoadAsync(void);
BOOL RFLAvailable(void);
void* RFLiAlloc(u32);
void* RFLiAlloc32(u32);
void RFLiFree(void*);
RFLDBManager* RFLiGetDBManager(void);
RFLHDBManager* RFLiGetHDBManager(void);
RFLLoader* RFLiGetLoader(void);
BOOL RFLiGetWorking(void);
void RFLiSetWorking(BOOL);
RFLManager* RFLiGetManager(void);
RFLErrcode RFLGetAsyncStatus(void);
NANDResult RFLGetLastReason(void);
RFLErrcode RFLWaitAsync(void);
RFLAccessInfo* RFLiGetAccInfo(RFLiFileType);
RFLCtrlBufManager* RFLiGetCtrlBufManager(void);
NANDResult RFLiGetLastReason(void);
void RFLiSetFileBroken(RFLiFileBrokenType);
BOOL RFLiNotFoundError(void);
BOOL RFLiNeedRepairError(void);
BOOL RFLiCriticalError(void);

#ifdef __cplusplus
}
#endif
#endif

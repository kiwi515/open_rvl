#ifndef RVL_FACE_LIBRARY_SYSTEM_H
#define RVL_FACE_LIBRARY_SYSTEM_H
#include <RVLFaceLib/RFL_Controller.h>
#include <RVLFaceLib/RFL_Database.h>
#include <RVLFaceLib/RFL_HiddenDatabase.h>
#include <RVLFaceLib/RFL_NANDAccess.h>
#include <RVLFaceLib/RFL_NANDLoader.h>
#include <RVLFaceLib/RFL_Types.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct MEMiHeapHead;

typedef struct RFLManager {
    void* workBuffer;                     // at 0x0
    struct MEMiHeapHead* rootHeap;        // at 0x4
    struct MEMiHeapHead* systemHeap;      // at 0x8
    struct MEMiHeapHead* tmpHeap;         // at 0xC
    RFLDBManager dbMgr;                   // at 0x10
    RFLLoader loader;                     // at 0xC0
    RFLHDBManager hdbMgr;                 // at 0x16C
    RFLCtrlBufManager ctrlMgr;            // at 0x1AAC
    BOOL working;                         // at 0x1B34
    BOOL deluxeTex;                       // at 0x1B38
    u8 brokenType;                        // at 0x1B3C
    RFLErrcode lastErrCode;               // at 0x1B40
    RFLErrcode beforeCloseErr;            // at 0x1B44
    s32 lastReason;                       // at 0x1B48
    s32 beforeCloseReason;                // at 0x1B4C
    RFLAccessInfo info[RFLiFileType_Max]; // at 0x1B50
    RFLSystemCallback systemCb;           // at 0x1F10
    RFLIconDrawDoneCallback iconDrawCb;   // at 0x1F14
    RFLModelDrawDoneCallback modelDrawCb; // at 0x1F18
    char UNK_0x1F1C[0x4];
} RFLManager;

u32 RFLGetWorkSize(BOOL deluxeTex);
RFLErrcode RFLInitResAsync(void* workBuffer, void* resBuffer, u32 resSize,
                           BOOL deluxeTex);
RFLErrcode RFLInitRes(void* workBuffer, void* resBuffer, u32 resSize,
                      BOOL deluxeTex);
void RFLExit(void);
RFLErrcode RFLiBootLoadAsync(void);
BOOL RFLAvailable(void);
void* RFLiAlloc(u32 size);
void* RFLiAlloc32(u32 size);
void RFLiFree(void* block);
RFLDBManager* RFLiGetDBManager(void);
RFLHDBManager* RFLiGetHDBManager(void);
RFLLoader* RFLiGetLoader(void);
BOOL RFLiGetWorking(void);
void RFLiSetWorking(BOOL working);
RFLManager* RFLiGetManager(void);
RFLErrcode RFLGetAsyncStatus(void);
s32 RFLGetLastReason(void);
RFLErrcode RFLWaitAsync(void);
RFLAccessInfo* RFLiGetAccInfo(RFLiFileType type);
RFLCtrlBufManager* RFLiGetCtrlBufManager(void);
s32 RFLiGetLastReason(void);
void RFLiSetFileBroken(RFLiFileBrokenType type);
BOOL RFLiNotFoundError(void);
BOOL RFLiNeedRepairError(void);
BOOL RFLiCriticalError(void);

#ifdef __cplusplus
}
#endif
#endif

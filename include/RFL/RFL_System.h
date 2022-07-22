#ifndef RVL_SDK_RFL_SYSTEM_H
#define RVL_SDK_RFL_SYSTEM_H
#include <MEM/mem_heapCommon.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

// TO-DO
typedef UNKTYPE (*RFLIconDrawDoneCallback)(UNKTYPE);
typedef UNKTYPE (*RFLModelDrawDoneCallback)(UNKTYPE);

typedef enum {
    RFL_ERR_NONE,
    RFL_ERR_1,
    RFL_ERR_2,
    RFL_ERR_3,
    RFL_ERR_4,
    RFL_ERR_CRITICAL,
    RFL_ERR_BUSY
} RFLError;

typedef enum {
    RFL_REASON_0,
} RFLReason;

typedef enum {
    RFL_BROKEN_NOT_FOUND,
    RFL_BROKEN_NEED_REPAIR,
    RFL_BROKEN_IO_ERROR,
    RFL_BROKEN_3,
} RFLBrokenType;

typedef enum {
    RFL_ACCESS_TYPE_0,
    RFL_ACCESS_TYPE_1,
    RFL_ACCESS_TYPE_MAX
} RFLAccessType;

typedef struct RFLiDBManager {
    void* database; // at 0x0
    char UNK_0x4[0xB0 - 0x4];
} RFLiDBManager;

typedef struct RFLiLoader {
    char UNK_0x0[0x90];
    u32 resSize;     // at 0x90
    void* resBuffer; // at 0x94
    u32 WORD_0x98;
    char UNK_0x9C[0xAC - 0x9C];
} RFLiLoader;

// TO-DO: Size
typedef struct RFLiHDBManager {
    u8 temp;
} RFLiHDBManager;

typedef struct RFLiAccInfo {
    char UNK_0x0[0x1E0];
} RFLiAccInfo;

// TO-DO: Size
typedef struct RFLiCtrlBufManager {
    u8 temp;
} RFLiCtrlBufManager;

typedef struct RFLiManager {
    void* workBuffer;         // at 0x0
    MEMiHeapHead* rootHeap;   // at 0x4
    MEMiHeapHead* systemHeap; // at 0x8
    MEMiHeapHead* tmpHeap;    // at 0xC
    RFLiDBManager dbMgr;      // at 0x10
    RFLiLoader loader;        // at 0xC0
    RFLiHDBManager hdbMgr;    // at 0x16C
    char UNK_0x16D[0x1AAC - 0x16D];
    RFLiCtrlBufManager ctrlBufMgr; // at 0x1AAC
    char UNK_0x1AAD[0x1B34 - 0x1AAD];
    BOOL working;    // at 0x1B34
    BOOL deluxTex;   // at 0x1B38
    u8 brokenType;   // at 0x1B3C
    RFLError status; // at 0x1B40
    u32 WORD_0x1B44;
    RFLReason reason; // at 0x1B48
    u32 WORD_0x1B4C;
    RFLiAccInfo accInfo[RFL_ACCESS_TYPE_MAX]; // at 0x1B50
    char UNK_0x1F10[0x1F14 - 0x1F10];
    RFLIconDrawDoneCallback iconDrawDone;   // at 0x1F14
    RFLModelDrawDoneCallback modelDrawDone; // at 0x1F18
    char UNK_0x1F1C[0x4];
} RFLiManager;

u32 RFLGetWorkSize(BOOL);
RFLError RFLInitResAsync(void*, void*, u32, BOOL);
RFLError RFLInitRes(void*, void*, u32, BOOL);
void RFLExit(void);
RFLError RFLiBootLoadAsync(void);
BOOL RFLAvailable(void);
void* RFLiAlloc(u32);
void* RFLiAlloc32(u32);
void RFLiFree(void*);
RFLiDBManager* RFLiGetDBManager(void);
RFLiHDBManager* RFLiGetHDBManager(void);
RFLiLoader* RFLiGetLoader(void);
BOOL RFLiGetWorking(void);
void RFLiSetWorking(BOOL);
RFLiManager* RFLiGetManager(void);
RFLError RFLGetAsyncStatus(void);
RFLReason RFLGetLastReason(void);
RFLError RFLWaitAsync(void);
RFLiAccInfo* RFLiGetAccInfo(RFLAccessType);
RFLiCtrlBufManager* RFLiGetCtrlBufManager(void);
RFLReason RFLiGetLastReason(void);
void RFLiSetFileBroken(RFLBrokenType);
BOOL RFLiNotFoundError(void);
BOOL RFLiNeedRepairError(void);
BOOL RFLiCriticalError(void);

#ifdef __cplusplus
}
#endif
#endif

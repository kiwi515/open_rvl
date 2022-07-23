#ifndef RVL_SDK_RFL_SYSTEM_H
#define RVL_SDK_RFL_SYSTEM_H
#include <MEM/mem_heapCommon.h>
#include <OS/OSAlarm.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

// TO-DO
typedef UNKTYPE (*RFLIconDrawDoneCallback)(UNKTYPE);
typedef UNKTYPE (*RFLModelDrawDoneCallback)(UNKTYPE);
typedef UNKTYPE (*RFLAccessCallback)(UNKTYPE);

typedef enum {
    RFL_RESULT_OK,
    RFL_RESULT_1,
    RFL_RESULT_2,
    RFL_RESULT_3,
    RFL_RESULT_4,
    RFL_RESULT_CRITICAL,
    RFL_RESULT_BUSY
} RFLResult;

typedef enum {
    RFL_REASON_NONE,
} RFLReason;

typedef enum {
    RFL_BROKEN_NOT_FOUND,
    RFL_BROKEN_NEED_REPAIR,
    RFL_BROKEN_IO_ERROR,
    RFL_BROKEN_3,
} RFLBrokenType;

typedef enum { RFL_ACCESS_DB, RFL_ACCESS_RES, RFL_ACCESS_MAX } RFLAccessType;

/**
 * @brief Face lib texture resources
 */
typedef enum {
    RFL_PART_TEX_EYE,
    RFL_PART_TEX_EYEBROW,
    RFL_PART_TEX_MOUTH,
    RFL_PART_TEX_FACIAL_HAIR,
    RFL_PART_TEX_MOLE,
} RFLPartTex;

/**
 * @brief Face lib texture resources (for shapes)
 */
typedef enum {
    RFL_PART_SHP_TEX_MASCARA,
    RFL_PART_SHP_TEX_CAP,
    RFL_PART_SHP_TEX_NOSE,
    RFL_PART_SHP_TEX_GLASSES,
} RFLPartShpTex;

/**
 * @brief Face lib shape resources
 */
typedef enum {
    RFL_PART_SHP_NOSE,
    RFL_PART_SHP_FOREHEAD,
    RFL_PART_SHP_FACE,
    RFL_PART_SHP_HAIR,
    RFL_PART_SHP_CAP,
    RFL_PART_SHP_BEARD,
    RFL_PART_SHP_NOSELINE,
    RFL_PART_SHP_MASK,
    RFL_PART_SHP_GLASSES,
} RFLPartShp;

/**
 * @brief Face lib archives (in RFL_Res.dat)
 */
typedef enum {
    RFL_ARC_SHP_BEARD,
    RFL_ARC_TEX_EYE,
    RFL_ARC_TEX_EYEBROW,

    RFL_ARC_SHP_FACE,
    RFL_ARC_TEX_MASCARA,

    RFL_ARC_SHP_FOREHEAD,

    RFL_ARC_SHP_GLASSES,
    RFL_ARC_TEX_GLASSES,

    RFL_ARC_SHP_HAIR,

    RFL_ARC_SHP_MASK,
    RFL_ARC_TEX_MOLE,
    RFL_ARC_TEX_MOUTH,
    RFL_ARC_TEX_FACIAL_HAIR,

    RFL_ARC_SHP_NOSE,
    RFL_ARC_SHP_NOSELINE,
    RFL_ARC_TEX_NOSE,

    RFL_ARC_SHP_CAP,
    RFL_ARC_TEX_CAP,

    RFL_ARC_MAX
} RFLPartArc;

typedef struct RFLDBManager {
    void* database; // at 0x0
    char UNK_0x4[0xB0 - 0x4];
} RFLDBManager;

typedef struct RFLResource {
    u16 numFiles;    // at 0x0
    u16 biggestSize; // at 0x2
    u32 offset;      // at 0x4
} RFLResource;

typedef struct RFLLoader {
    RFLResource resources[RFL_ARC_MAX]; // at 0x0
    u32 cacheSize;                      // at 0x90
    void* cache;                        // at 0x94
    BOOL cached;                        // at 0x98
    u16 version;                        // at 0x9C
    void* headerBuf1;                   // at 0xA0
    void* headerBuf2;                   // at 0xA4
    u8 numResources;                    // at 0xA8
} RFLLoader;

// TO-DO: Size
typedef struct RFLHDBManager {
    u8 temp;
} RFLHDBManager;

typedef struct RFLAccInfo {
    RFLAccessCallback callback; // at 0x0
    char UNK_0x4[0x48 - 0x4];
    NANDFileInfo file; // at 0x48
    char UNK_0xD4[0x198 - 0xD4];
    OSAlarm alarm; // at 0x198
    char UNK_0x1C4[0x1D4 - 0x1C4];
    void* safeBuffer; // at 0x1D4
    char UNK_0x1D8[0x1E0 - 0x1D8];
} RFLAccInfo;

// TO-DO: Size
typedef struct RFLCtrlBufManager {
    u8 temp;
} RFLCtrlBufManager;

typedef struct RFLManager {
    void* workBuffer;         // at 0x0
    MEMiHeapHead* rootHeap;   // at 0x4
    MEMiHeapHead* systemHeap; // at 0x8
    MEMiHeapHead* tmpHeap;    // at 0xC
    RFLDBManager dbMgr;       // at 0x10
    RFLLoader loader;         // at 0xC0
    RFLHDBManager hdbMgr;     // at 0x16C
    char UNK_0x16D[0x1AAC - 0x16D];
    RFLCtrlBufManager ctrlBufMgr; // at 0x1AAC
    char UNK_0x1AAD[0x1B34 - 0x1AAD];
    BOOL working;     // at 0x1B34
    BOOL deluxTex;    // at 0x1B38
    u8 brokenType;    // at 0x1B3C
    RFLResult status; // at 0x1B40
    u32 WORD_0x1B44;
    RFLReason reason; // at 0x1B48
    u32 WORD_0x1B4C;
    RFLAccInfo info[RFL_ACCESS_MAX]; // at 0x1B50
    char UNK_0x1F10[0x1F14 - 0x1F10];
    RFLIconDrawDoneCallback iconDrawDone;   // at 0x1F14
    RFLModelDrawDoneCallback modelDrawDone; // at 0x1F18
    char UNK_0x1F1C[0x4];
} RFLManager;

u32 RFLGetWorkSize(BOOL);
RFLResult RFLInitResAsync(void*, void*, u32, BOOL);
RFLResult RFLInitRes(void*, void*, u32, BOOL);
void RFLExit(void);
RFLResult RFLiBootLoadAsync(void);
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
RFLResult RFLGetAsyncStatus(void);
RFLReason RFLGetLastReason(void);
RFLResult RFLWaitAsync(void);
RFLAccInfo* RFLiGetAccInfo(RFLAccessType);
RFLCtrlBufManager* RFLiGetCtrlBufManager(void);
RFLReason RFLiGetLastReason(void);
void RFLiSetFileBroken(RFLBrokenType);
BOOL RFLiNotFoundError(void);
BOOL RFLiNeedRepairError(void);
BOOL RFLiCriticalError(void);

#ifdef __cplusplus
}
#endif
#endif

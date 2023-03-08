#ifndef RVL_SDK_MEM_FRAME_HEAP_H
#define RVL_SDK_MEM_FRAME_HEAP_H
#include <revolution/MEM/mem_heapCommon.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Specify how to free memory
typedef enum {
    MEM_FRM_HEAP_FREE_TO_HEAD = (1 << 0),
    MEM_FRM_HEAP_FREE_TO_TAIL = (1 << 1)
} MEMiFrmFreeFlag;

typedef struct MEMiFrmHeapState {
    u32 id;                        // at 0x0
    u8* head;                      // at 0x4
    u8* tail;                      // at 0x8
    struct MEMiFrmHeapState* next; // at 0xC
} MEMiFrmHeapState;

// Placed in heap after base heap head
typedef struct MEMiFrmHeapHead {
    u8* head;                 // at 0x0
    u8* tail;                 // at 0x4
    MEMiFrmHeapState* states; // at 0x8
} MEMiFrmHeapHead;

MEMiHeapHead* MEMCreateFrmHeapEx(void*, u32, u16);
MEMiHeapHead* MEMDestroyFrmHeap(MEMiHeapHead*);
void* MEMAllocFromFrmHeapEx(MEMiHeapHead*, u32, s32);
void MEMFreeToFrmHeap(MEMiHeapHead*, u32);
u32 MEMGetAllocatableSizeForFrmHeapEx(MEMiHeapHead*, s32);
BOOL MEMRecordStateForFrmHeap(MEMiHeapHead*, u32);
BOOL MEMFreeByStateToFrmHeap(MEMiHeapHead*, u32);
u32 MEMAdjustFrmHeap(MEMiHeapHead*);
u32 MEMResizeForMBlockFrmHeap(MEMiHeapHead*, void*, u32);

#ifdef __cplusplus
}
#endif
#endif

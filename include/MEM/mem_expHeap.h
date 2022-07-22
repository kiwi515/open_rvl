#ifndef RVL_SDK_MEM_EXP_HEAP_H
#define RVL_SDK_MEM_EXP_HEAP_H
#include "mem_heapCommon.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

MEMiHeapHead* MEMCreateExpHeapEx(void*, u32, u16);
void* MEMDestroyExpHeap(MEMiHeapHead*);
void* MEMAllocFromExpHeapEx(MEMiHeapHead*, u32, s32);

void MEMFreeToExpHeap(MEMiHeapHead*, void*);

u32 MEMGetAllocatableSizeForExpHeapEx(MEMiHeapHead*, s32);

static u32 MEMGetAllocatableSizeForExpHeap(MEMiHeapHead* heap) {
    return MEMGetAllocatableSizeForExpHeapEx(heap, 4);
}

#ifdef __cplusplus
}
#endif
#endif

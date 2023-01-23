#ifndef RVL_SDK_MEM_EXP_HEAP_H
#define RVL_SDK_MEM_EXP_HEAP_H
#include "mem_heapCommon.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MEM_EXP_HEAP_ALLOC_MODE_0,
} MEMiExpHeapAllocMode;

typedef struct MEMiExpHeapMBlock {
    u16 state; // at 0x0
    union {
        u16 settings;
        struct {
            u16 allocDir : 1;
            u16 align : 7;
            u16 group : 8;
        };
    };                              // at 0x2
    u32 size;                       // at 0x4
    struct MEMiExpHeapMBlock* prev; // at 0x8
    struct MEMiExpHeapMBlock* next; // at 0xC
} MEMiExpHeapMBlock;

typedef struct MEMiExpHeapMBlockList {
    MEMiExpHeapMBlock* head; // at 0x0
    MEMiExpHeapMBlock* tail; // at 0x4
} MEMiExpHeapMBlockList;

// Placed in heap after base heap head
typedef struct MEMiExpHeapHead {
    MEMiExpHeapMBlockList freeMBlocks; // at 0x0
    MEMiExpHeapMBlockList usedMBlocks; // at 0x8
    u16 group;                         // at 0x10
    union {
        u16 SHORT_0x12;
        struct {
            u16 SHORT_0x12_0_15 : 15;
            u16 allocMode : 1;
        };
    }; // at 0x12
} MEMiExpHeapHead;

MEMiHeapHead* MEMCreateExpHeapEx(void*, u32, u16);
MEMiHeapHead* MEMDestroyExpHeap(MEMiHeapHead*);
void* MEMAllocFromExpHeapEx(MEMiHeapHead*, u32, s32);
u32 MEMResizeForMBlockExpHeap(MEMiHeapHead*, void*, u32);
void MEMFreeToExpHeap(MEMiHeapHead*, void*);
u32 MEMGetAllocatableSizeForExpHeapEx(MEMiHeapHead*, s32);
u32 MEMAdjustExpHeap(MEMiHeapHead*);

static inline MEMiHeapHead* MEMCreateExpHeap(void* start, u32 size) {
    return MEMCreateExpHeapEx(start, size, 0);
}

static inline void* MEMAllocFromExpHeap(MEMiHeapHead* heap, u32 size) {
    return MEMAllocFromExpHeapEx(heap, size, 4);
}

static inline u32 MEMGetAllocatableSizeForExpHeap(MEMiHeapHead* heap) {
    return MEMGetAllocatableSizeForExpHeapEx(heap, 4);
}

#ifdef __cplusplus
}
#endif
#endif

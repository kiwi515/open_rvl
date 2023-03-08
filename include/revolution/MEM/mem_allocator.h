#ifndef RVL_SDK_MEM_ALLOCATOR_H
#define RVL_SDK_MEM_ALLOCATOR_H
#include <revolution/MEM/mem_heapCommon.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MEMAllocator;
typedef void* (*MEMAllocatorAllocFunc)(struct MEMAllocator*, u32);
typedef void (*MEMAllocatorFreeFunc)(struct MEMAllocator*, void*);

typedef struct MEMAllocatorFuncs {
    MEMAllocatorAllocFunc allocFunc; // at 0x0
    MEMAllocatorFreeFunc freeFunc;   // at 0x4
} MEMAllocatorFuncs;

typedef struct MEMAllocator {
    MEMAllocatorFuncs* funcs; // at 0x0
    MEMiHeapHead* heap;       // at 0x4
    s32 align;                // at 0x8
    UNKWORD WORD_0xC;
} MEMAllocator;

void* MEMAllocFromAllocator(MEMAllocator*, u32);
void MEMFreeToAllocator(MEMAllocator*, void*);
void MEMInitAllocatorForExpHeap(MEMAllocator*, MEMiHeapHead*, s32);
void MEMInitAllocatorForFrmHeap(MEMAllocator*, MEMiHeapHead*, s32);

#ifdef __cplusplus
}
#endif
#endif

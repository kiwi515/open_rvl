#ifndef RVL_SDK_OS_ALLOC_H
#define RVL_SDK_OS_ALLOC_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSHeapDescriptor {
    s32 size;                    // at 0x0
    struct OSHeapCell* freeList; // at 0x4
    struct OSHeapCell* usedList; // at 0x8
} OSHeapDescriptor;

typedef struct OSHeapCell {
    struct OSHeapCell* prev;     // at 0x0
    struct OSHeapCell* next;     // at 0x4
    s32 size;                    // at 0x8
    struct OSHeapDescriptor* hd; // at 0xC
    s32 usedSize;                // at 0x10
    char UNK_0x14[0x20 - 0x14];
} OSHeapCell;

extern volatile s32 __OSCurrHeap;

void* OSAllocFromHeap(s32, s32);
void OSFreeToHeap(s32, void*);
s32 OSSetCurrentHeap(s32);
void* OSInitAlloc(void*, void*, s32);
s32 OSCreateHeap(void*, void*);

#ifdef __cplusplus
}
#endif
#endif

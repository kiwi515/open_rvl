#ifndef RVL_SDK_MEM_HEAP_COMMON_H
#define RVL_SDK_MEM_HEAP_COMMON_H
#include "mem_list.h"
#include <OS/OSMutex.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum { MEM_HEAP_OPT_DEBUG_FILL = (1 << 1) } MEMHeapOpt;

typedef struct MEMiHeapHead {
    u32 magic;     // at 0x0
    MEMNode node;  // at 0x4
    MEMList list;  // at 0xC
    u8* start;     // at 0x18
    u8* end;       // at 0x1C
    OSMutex mutex; // at 0x20

    union {
        u32 WORD_0x38;
        struct {
            u32 WORD_0x38_0_3 : 24;
            u32 opt : 8;
        };
    }; // at 0x38
} MEMiHeapHead;

void MEMiInitHeapHead(MEMiHeapHead*, u32, void*, void*, u16);
void MEMiFinalizeHeap(MEMiHeapHead*);
MEMiHeapHead* MEMFindContainHeap(const void*);

#ifdef __cplusplus
}
#endif
#endif

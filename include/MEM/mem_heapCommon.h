#ifndef RVL_SDK_MEM_HEAP_COMMON_H
#define RVL_SDK_MEM_HEAP_COMMON_H
#include "mem_list.h"
#include <OS/OSMutex.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MEMiHeapHead {
    u32 magic;     // at 0x0
    MEMNode node;  // at 0x4
    MEMList list;  // at 0xC
    u8* start;     // at 0x18
    u8* end;       // at 0x1C
    OSMutex mutex; // at 0x20
    // . . .
} MEMiHeapHead;

#ifdef __cplusplus
}
#endif
#endif

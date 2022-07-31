#ifndef RVL_SDK_MEM_FRAME_HEAP_H
#define RVL_SDK_MEM_FRAME_HEAP_H
#include "mem_heapCommon.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void* MEMAllocFromFrmHeapEx(MEMiHeapHead*, u32, s32);

#ifdef __cplusplus
}
#endif
#endif

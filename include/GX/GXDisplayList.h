#ifndef RVL_SDK_GX_DISPLAY_LIST_H
#define RVL_SDK_GX_DISPLAY_LIST_H
#include "GX.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void GXBeginDisplayList(void*, u32);
u32 GXEndDisplayList(void);
void GXCallDisplayList(void*, u32);

static inline void GXFastCallDisplayList(void* list, u32 size) {
    WGPIPE.c = 0x40;
    WGPIPE.p = list;
    WGPIPE.i = size;
}

#ifdef __cplusplus
}
#endif
#endif

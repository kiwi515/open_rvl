#include "GXDisplayList.h"
#include "GXFifo.h"
#include "GXGeometry.h"
#include "GXInit.h"
#include "GXMisc.h"

#include <OS/OSInterrupt.h>
#include <TRK/__mem.h>

static GXFifoObj DisplayListFifo;
static GXData __savedGXdata;
static GXFifoObj OldCPUFifo;

void GXBeginDisplayList(void* list, u32 size) {
    GXData* data = __GXData;
    GXFifoObj* fifo = &DisplayListFifo;

    if (data->dirtyFlags != 0) {
        __GXSetDirtyState();
    }

    if (data->BYTE_0x5F9) {
        memcpy(&__savedGXdata, data, sizeof(GXData));
    }

    fifo->base = list;
    fifo->end = (u8*)list + size - 4;
    fifo->size = size;
    fifo->count = 0;
    fifo->readPtr = list;
    fifo->writePtr = list;
    data->dlistBegan = TRUE;

    GXGetCPUFifo(&OldCPUFifo);
    GXSetCPUFifo(&DisplayListFifo);
    GXResetWriteGatherPipe();
}

#ifdef __DECOMP_NON_MATCHING
// https://decomp.me/scratch/9jZkl
u32 GXEndDisplayList(void) {
    u8 wrap;
    GXData* data;
    UNKWORD bak;

    GXGetCPUFifo(&DisplayListFifo);
    wrap = GXGetFifoWrap(&DisplayListFifo);
    GXSetCPUFifo(&OldCPUFifo);

    data = __GXData;
    if (__GXData->BYTE_0x5F9) {
        u32 intr = OSDisableInterrupts();

        bak = __GXData->WORD_0x8;
        memcpy(__GXData, &__savedGXdata, sizeof(GXData));
        data->WORD_0x8 = bak;

        OSRestoreInterrupts(intr);
    }

    data->dlistBegan = FALSE;

    if (!wrap) {
        return GXGetFifoCount(&DisplayListFifo);
    }

    return 0;
}
#else
// #error This file has not yet been decompiled.
#endif

void GXCallDisplayList(void* list, u32 size) {
    GXData* data = __GXData;

    if (data->dirtyFlags != 0) {
        __GXSetDirtyState();
    }

    if (data->WORD_0x0 == 0) {
        __GXSendFlushPrim();
    }

    WGPIPE.c = 0x40;
    WGPIPE.p = list;
    WGPIPE.i = size;
}

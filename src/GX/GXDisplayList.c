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
    GXFifoObj* fifo = &DisplayListFifo;

    if (__GXData->dirtyFlags != 0) {
        __GXSetDirtyState();
    }

    if (__GXData->BYTE_0x5F9) {
        memcpy(&__savedGXdata, __GXData, sizeof(GXData));
    }

    fifo->base = list;
    fifo->end = (u8*)list + size - 4;
    fifo->size = size;
    fifo->count = 0;
    fifo->readPtr = list;
    fifo->writePtr = list;
    __GXData->dlistBegan = TRUE;

    GXGetCPUFifo(&OldCPUFifo);
    GXSetCPUFifo(&DisplayListFifo);
    GXResetWriteGatherPipe();
}

u32 GXEndDisplayList(void) {
    u8 wrap;
    u32 intr;
    UNKWORD bak;

    GXGetCPUFifo(&DisplayListFifo);
    wrap = GXGetFifoWrap(&DisplayListFifo);
    GXSetCPUFifo(&OldCPUFifo);

    if (__GXData->BYTE_0x5F9) {
        intr = OSDisableInterrupts();

        bak = __GXData->WORD_0x8;
        memcpy(__GXData, &__savedGXdata, sizeof(GXData));
        __GXData->WORD_0x8 = bak;

        OSRestoreInterrupts(intr);
    }

    __GXData->dlistBegan = FALSE;

    if (!wrap) {
        return GXGetFifoCount(&DisplayListFifo);
    }

    return 0;
}

void GXCallDisplayList(void* list, u32 size) {
    if (__GXData->dirtyFlags != 0) {
        __GXSetDirtyState();
    }

    if (__GXData->WORD_0x0 == 0) {
        __GXSendFlushPrim();
    }

    WGPIPE.c = 0x40;
    WGPIPE.p = list;
    WGPIPE.i = size;
}

#include "OSInterrupt.h"
#include "EXIBios.h"
#include "OS.h"
#include "OSAddress.h"
#include "OSError.h"
#include "OSInterrupt.h"
#include "OSMemory.h"
#include "OSTime.h"

#include <TRK/__mem.h>

static OSInterruptHandler* InterruptHandlerTable;
s64 __OSLastInterruptTime;
s16 __OSLastInterrupt;
u32 __OSLastInterruptSrr0;

static u32 InterruptPrioTable[] = {
    0x00000100, 0x00000040, 0xF8000000, 0x00000200, 0x00000080, 0x00000010,
    0x00003000, 0x00000020, 0x03FF8C00, 0x04000000, 0x00004000, 0xFFFFFFFF};

static void ExternalInterruptHandler(u8, OSContext*);

asm BOOL OSDisableInterrupts(void) {
    // clang-format off
    nofralloc

    entry __RAS_OSDisableInterrupts_begin

    mfmsr r3
    // Clear external interrupts bit
    rlwinm r4, r3, 0, 17, 15
    mtmsr r4

    entry __RAS_OSDisableInterrupts_end

    // Return old interrupt status
    rlwinm r3, r3, 17, 31, 31
    blr
    // clang-format on
}

asm BOOL OSEnableInterrupts(void) {
    // clang-format off
    nofralloc

    mfmsr r3
    // Set external interrupts bit
    ori r4, r3, (1 << 15)
    mtmsr r4

    // Return old interrupt status
    rlwinm r3, r3, 17, 31, 31
    blr
    // clang-format on
}

asm BOOL OSRestoreInterrupts(register BOOL status){
    // clang-format off
    nofralloc

    cmpwi status, 0
    mfmsr r4
    beq disable

    // Set external interrupts bit
    ori r5, r4, (1 << 15)
    b set_msr

disable:
    // Clear external interrupts bit
    rlwinm r5, r4, 0, 17, 15

set_msr:
    mtmsr r5
    // Return old interrupt status
    rlwinm r3, r4, 17, 31, 31
    blr
    // clang-format on
}

OSInterruptHandler
    __OSSetInterruptHandler(OSInterruptType type, OSInterruptHandler handler) {
    const OSInterruptHandler old = InterruptHandlerTable[type];
    InterruptHandlerTable[type] = handler;
    return old;
}

OSInterruptHandler __OSGetInterruptHandler(OSInterruptType type) {
    return InterruptHandlerTable[type];
}

void __OSInterruptInit(void) {
    InterruptHandlerTable =
        (OSInterruptHandler*)OSPhysicalToCached(OS_PHYS_INTR_TABLE);
    memset(InterruptHandlerTable, 0, sizeof(OSInterruptHandler) * OS_INTR_MAX);

    *(u32*)OSPhysicalToCached(OS_PHYS_PREV_INTR_MASK) = 0;
    *(u32*)OSPhysicalToCached(OS_PHYS_CURR_INTR_MASK) = 0;

    OS_PI_INTR_MASK = 0xF0;
    OS_PI_CD000034 = 0x40000000;

    __OSMaskInterrupts(0xFFFFFFF0);
    __OSSetExceptionHandler(OS_ERR_EXT_INTERRUPT, ExternalInterruptHandler);
}

static u32 SetInterruptMask(u32 type, u32 mask) {
    switch (__cntlzw(type)) {
    case OS_INTR_MEM_0:
    case OS_INTR_MEM_1:
    case OS_INTR_MEM_2:
    case OS_INTR_MEM_3:
    case OS_INTR_MEM_ADDRESS:
        u32 mi_intr = 0;

        if (!(mask & 0x80000000)) {
            mi_intr |= 0x1;
        }
        if (!(mask & 0x40000000)) {
            mi_intr |= 0x2;
        }
        if (!(mask & 0x20000000)) {
            mi_intr |= 0x4;
        }
        if (!(mask & 0x10000000)) {
            mi_intr |= 0x8;
        }
        if (!(mask & 0x8000000)) {
            mi_intr |= 0x10;
        }

        OS_MI_INTR_MASK = mi_intr;
        return type & 0x7FFFFFF;
    case OS_INTR_DSP_AI:
    case OS_INTR_DSP_ARAM:
    case OS_INTR_DSP_DSP:
        u32 dsp_intr = OS_DSP_INTR_MASK;
        dsp_intr &= ~0x1F8;

        if (!(mask & 0x4000000)) {
            dsp_intr |= 0x10;
        }
        if (!(mask & 0x2000000)) {
            dsp_intr |= 0x40;
        }
        if (!(mask & 0x1000000)) {
            dsp_intr |= 0x100;
        }

        OS_DSP_INTR_MASK = dsp_intr;
        return type & 0xF8FFFFFF;
    case OS_INTR_AI_AI:
        u32 ai_intr = OS_AI_INTR_MASK;
        ai_intr &= ~0x2C;

        if (!(mask & 0x800000)) {
            ai_intr |= 0x4;
        }

        OS_AI_INTR_MASK = ai_intr;
        return type & 0xFF7FFFFF;
    case OS_INTR_EXI_0_EXI:
    case OS_INTR_EXI_0_TC:
    case OS_INTR_EXI_0_EXT:
        u32 exi0_intr = EXI_CD006800[EXI_CHAN_0].WORD_0x0;
        exi0_intr &= ~0x2C0F;

        if (!(mask & 0x400000)) {
            exi0_intr |= 0x1;
        }
        if (!(mask & 0x200000)) {
            exi0_intr |= 0x4;
        }
        if (!(mask & 0x100000)) {
            exi0_intr |= 0x400;
        }

        EXI_CD006800[EXI_CHAN_0].WORD_0x0 = exi0_intr;
        return type & 0xFF8FFFFF;
    case OS_INTR_EXI_1_EXI:
    case OS_INTR_EXI_1_TC:
    case OS_INTR_EXI_1_EXT:
        u32 exi1_intr = EXI_CD006800[EXI_CHAN_1].WORD_0x0;
        exi1_intr &= ~0xC0F;

        if (!(mask & 0x80000)) {
            exi1_intr |= 0x1;
        }
        if (!(mask & 0x40000)) {
            exi1_intr |= 0x4;
        }
        if (!(mask & 0x20000)) {
            exi1_intr |= 0x400;
        }

        EXI_CD006800[EXI_CHAN_1].WORD_0x0 = exi1_intr;
        return type & 0xFFF1FFFF;
    case OS_INTR_EXI_2_EXI:
    case OS_INTR_EXI_2_TC:
        u32 exi2_intr = EXI_CD006800[EXI_CHAN_2].WORD_0x0;
        exi2_intr &= ~0xF;

        if (!(mask & 0x10000)) {
            exi2_intr |= 0x1;
        }
        if (!(mask & 0x8000)) {
            exi2_intr |= 0x4;
        }

        EXI_CD006800[EXI_CHAN_2].WORD_0x0 = exi2_intr;
        return type & 0xFFFE7FFF;
    case OS_INTR_PI_CP:
    case OS_INTR_PI_PE_TOKEN:
    case OS_INTR_PI_PE_FINISH:
    case OS_INTR_PI_SI:
    case OS_INTR_PI_DI:
    case OS_INTR_PI_RSW:
    case OS_INTR_PI_ERROR:
    case OS_INTR_PI_VI:
    case OS_INTR_PI_DEBUG:
    case OS_INTR_PI_HSP:
    case OS_INTR_PI_ACR:
        u32 pi_intr = 0xF0;

        if (!(mask & 0x4000)) {
            pi_intr |= 0x800;
        }
        if (!(mask & 0x800)) {
            pi_intr |= 0x8;
        }
        if (!(mask & 0x400)) {
            pi_intr |= 0x4;
        }
        if (!(mask & 0x200)) {
            pi_intr |= 0x2;
        }
        if (!(mask & 0x100)) {
            pi_intr |= 0x1;
        }
        if (!(mask & 0x80)) {
            pi_intr |= 0x100;
        }
        if (!(mask & 0x40)) {
            pi_intr |= 0x1000;
        }
        if (!(mask & 0x2000)) {
            pi_intr |= 0x200;
        }
        if (!(mask & 0x1000)) {
            pi_intr |= 0x400;
        }
        if (!(mask & 0x20)) {
            pi_intr |= 0x2000;
        }
        if (!(mask & 0x10)) {
            pi_intr |= 0x4000;
        }

        OS_PI_INTR_MASK = pi_intr;
        return type & 0xFFFF800F;
    default:
        return type;
    }
}

u32 __OSMaskInterrupts(u32 userMask) {
    const BOOL intr = OSDisableInterrupts();
    const u32 prevMask = *(u32*)OSPhysicalToCached(OS_PHYS_PREV_INTR_MASK);
    const u32 currMask = *(u32*)OSPhysicalToCached(OS_PHYS_CURR_INTR_MASK);

    u32 workMask = userMask & ~(prevMask | currMask);
    userMask = prevMask | userMask;
    *(u32*)OSPhysicalToCached(OS_PHYS_PREV_INTR_MASK) = userMask;

    while (workMask != 0) {
        workMask = SetInterruptMask(workMask, userMask | currMask);
    }

    OSRestoreInterrupts(intr);
    return prevMask;
}

u32 __OSUnmaskInterrupts(u32 userMask) {
    const BOOL intr = OSDisableInterrupts();
    const u32 prevMask = *(u32*)OSPhysicalToCached(OS_PHYS_PREV_INTR_MASK);
    const u32 currMask = *(u32*)OSPhysicalToCached(OS_PHYS_CURR_INTR_MASK);

    u32 workMask = userMask & (prevMask | currMask);
    userMask = prevMask & ~userMask;
    *(u32*)OSPhysicalToCached(OS_PHYS_PREV_INTR_MASK) = userMask;

    while (workMask != 0) {
        workMask = SetInterruptMask(workMask, userMask | currMask);
    }

    OSRestoreInterrupts(intr);
    return prevMask;
}

void __OSDispatchInterrupt(u8 intr, OSContext* ctx) {
    u32 intsr = OS_PI_INTR_CAUSE;
    intsr &= ~0x10000;

    const u32 pi_mask = OS_PI_INTR_MASK;
    if (intsr == 0 || !(intsr & pi_mask)) {
        OSLoadContext(ctx);
    }

    u32 cause = 0;
    u32 cause2 = 0;

    if (intsr & 0x80) {
        const u32 mi_mask = OS_MI_CC00401E;
        if (mi_mask & 0x1) {
            cause |= 0x80000000;
        }
        if (mi_mask & 0x2) {
            cause |= 0x40000000;
        }
        if (mi_mask & 0x4) {
            cause |= 0x20000000;
        }
        if (mi_mask & 0x8) {
            cause |= 0x10000000;
        }
        if (mi_mask & 0x10) {
            cause |= 0x8000000;
        }
    }

    if (intsr & 0x40) {
        const u32 dsp_mask = OS_DSP_INTR_MASK;
        if (dsp_mask & 0x8) {
            cause |= 0x4000000;
        }
        if (dsp_mask & 0x20) {
            cause |= 0x2000000;
        }
        if (dsp_mask & 0x80) {
            cause |= 0x1000000;
        }
    }

    if ((intsr & 0x20) && (OS_AI_INTR_MASK & 0x8)) {
        cause |= 0x800000;
    }

    if (intsr & 0x10) {
        const u32 exi0_mask = EXI_CD006800[EXI_CHAN_0].WORD_0x0;
        if (exi0_mask & 0x2) {
            cause |= 0x400000;
        }
        if (exi0_mask & 0x8) {
            cause |= 0x200000;
        }
        if (exi0_mask & 0x800) {
            cause |= 0x100000;
        }

        const u32 exi1_mask = EXI_CD006800[EXI_CHAN_1].WORD_0x0;
        if (exi1_mask & 0x2) {
            cause |= 0x80000;
        }
        if (exi1_mask & 0x8) {
            cause |= 0x40000;
        }
        if (exi1_mask & 0x800) {
            cause |= 0x20000;
        }

        const u32 exi2_mask = EXI_CD006800[EXI_CHAN_2].WORD_0x0;
        if (exi2_mask & 0x2) {
            cause |= 0x10000;
        }
        if (exi2_mask & 0x8) {
            cause |= 0x8000;
        }
    }

    if (intsr & 0x2000) {
        cause |= 0x20;
    }
    if (intsr & 0x1000) {
        cause |= 0x40;
    }
    if (intsr & 0x400) {
        cause |= 0x1000;
    }
    if (intsr & 0x200) {
        cause |= 0x2000;
    }
    if (intsr & 0x100) {
        cause |= 0x80;
    }
    if (intsr & 0x8) {
        cause |= 0x800;
    }
    if (intsr & 0x4) {
        cause |= 0x400;
    }
    if (intsr & 0x2) {
        cause |= 0x200;
    }
    if (intsr & 0x800) {
        cause |= 0x4000;
    }
    if (intsr & 0x1) {
        cause |= 0x100;
    }
    if (intsr & 0x4000) {
        cause |= 0x10;
    }

    cause2 = cause & ~(*(u32*)OSPhysicalToCached(OS_PHYS_PREV_INTR_MASK) |
                       *(u32*)OSPhysicalToCached(OS_PHYS_CURR_INTR_MASK));

    if (cause2 != 0) {
        s16 type;
        u32* pPrio;

        for (pPrio = InterruptPrioTable; TRUE; pPrio++) {
            if (cause2 & *pPrio) {
                type = __cntlzw(cause2 & *pPrio);
                break;
            }
        }

        OSInterruptHandler handler = __OSGetInterruptHandler(type);
        if (handler != NULL) {
            if (type > OS_INTR_MEM_ADDRESS) {
                __OSLastInterrupt = type;
                __OSLastInterruptTime = OSGetTime();
                __OSLastInterruptSrr0 = ctx->srr0;
            }

            OSDisableScheduler();
            handler(type, ctx);
            OSEnableScheduler();
            __OSReschedule();
            OSLoadContext(ctx);
        }
    }

    OSLoadContext(ctx);
}

static asm void ExternalInterruptHandler(register u8 type,
                                         register OSContext* ctx) {
    // clang-format off
    nofralloc

    stw r0, ctx->gprs[0]
    stw r1, ctx->gprs[1]
    stw r2, ctx->gprs[2]
    stmw r6, ctx->gprs[6]

    mfspr r0, GQR1
    stw r0, ctx->gqrs[1]
    mfspr r0, GQR2
    stw r0, ctx->gqrs[2]
    mfspr r0, GQR3
    stw r0, ctx->gqrs[3]
    mfspr r0, GQR4
    stw r0, ctx->gqrs[4]
    mfspr r0, GQR5
    stw r0, ctx->gqrs[5]
    mfspr r0, GQR6
    stw r0, ctx->gqrs[6]
    mfspr r0, GQR7
    stw r0, ctx->gqrs[7]

    stwu r1, -8(r1)
    b __OSDispatchInterrupt
    // clang-format on
}
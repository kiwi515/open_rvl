#include <BASE.h>
#include <EXI/EXIBios.h>
#include <EXI/EXICommon.h>
#include <OS.h>
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
    ori r4, r3, MSR_EE
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
    ori r5, r4, MSR_EE
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
        (OSInterruptHandler*)OSPhysicalToCached(OS_PHYS_INTR_HANDLER_TABLE);
    memset(InterruptHandlerTable, 0, sizeof(OSInterruptHandler) * OS_INTR_MAX);

    *(u32*)OSPhysicalToCached(OS_PHYS_PREV_INTR_MASK) = 0;
    *(u32*)OSPhysicalToCached(OS_PHYS_CURRENT_INTR_MASK) = 0;

    OS_PI_INTMR = 0xF0;
    OS_UNK_CD000034 = 0x40000000;

    __OSMaskInterrupts(
        OS_INTR_MASK(OS_INTR_MEM_0) | OS_INTR_MASK(OS_INTR_MEM_1) |
        OS_INTR_MASK(OS_INTR_MEM_2) | OS_INTR_MASK(OS_INTR_MEM_3) |
        OS_INTR_MASK(OS_INTR_MEM_ADDRESS) | OS_INTR_MASK(OS_INTR_DSP_AI) |
        OS_INTR_MASK(OS_INTR_DSP_ARAM) | OS_INTR_MASK(OS_INTR_DSP_DSP) |
        OS_INTR_MASK(OS_INTR_AI_AI) | OS_INTR_MASK(OS_INTR_EXI_0_EXI) |
        OS_INTR_MASK(OS_INTR_EXI_0_TC) | OS_INTR_MASK(OS_INTR_EXI_0_EXT) |
        OS_INTR_MASK(OS_INTR_EXI_1_EXI) | OS_INTR_MASK(OS_INTR_EXI_1_TC) |
        OS_INTR_MASK(OS_INTR_EXI_1_EXT) | OS_INTR_MASK(OS_INTR_EXI_2_EXI) |
        OS_INTR_MASK(OS_INTR_EXI_2_TC) | OS_INTR_MASK(OS_INTR_PI_CP) |
        OS_INTR_MASK(OS_INTR_PI_PE_TOKEN) | OS_INTR_MASK(OS_INTR_PI_PE_FINISH) |
        OS_INTR_MASK(OS_INTR_PI_SI) | OS_INTR_MASK(OS_INTR_PI_DI) |
        OS_INTR_MASK(OS_INTR_PI_RSW) | OS_INTR_MASK(OS_INTR_PI_ERROR) |
        OS_INTR_MASK(OS_INTR_PI_VI) | OS_INTR_MASK(OS_INTR_PI_DEBUG) |
        OS_INTR_MASK(OS_INTR_PI_HSP) | OS_INTR_MASK(OS_INTR_PI_ACR));

    __OSSetExceptionHandler(OS_ERR_EXT_INTERRUPT, ExternalInterruptHandler);
}

static u32 SetInterruptMask(u32 type, u32 mask) {
    u32 miIntr;
    u32 dspIntr;
    u32 aiIntr;
    u32 exi0Intr, exi1Intr, exi2Intr;
    u32 piIntr;

    switch (__cntlzw(type)) {
    case OS_INTR_MEM_0:
    case OS_INTR_MEM_1:
    case OS_INTR_MEM_2:
    case OS_INTR_MEM_3:
    case OS_INTR_MEM_ADDRESS:
        miIntr = 0;

        if (!(mask & 0x80000000)) {
            miIntr |= 0x1;
        }
        if (!(mask & 0x40000000)) {
            miIntr |= 0x2;
        }
        if (!(mask & 0x20000000)) {
            miIntr |= 0x4;
        }
        if (!(mask & 0x10000000)) {
            miIntr |= 0x8;
        }
        if (!(mask & 0x8000000)) {
            miIntr |= 0x10;
        }

        OS_MI_INTMR = miIntr;
        return type & 0x7FFFFFF;
    case OS_INTR_DSP_AI:
    case OS_INTR_DSP_ARAM:
    case OS_INTR_DSP_DSP:
        dspIntr = OS_DSP_CSR;
        dspIntr &= ~0x1F8;

        if (!(mask & 0x4000000)) {
            dspIntr |= 0x10;
        }
        if (!(mask & 0x2000000)) {
            dspIntr |= 0x40;
        }
        if (!(mask & 0x1000000)) {
            dspIntr |= 0x100;
        }

        OS_DSP_CSR = dspIntr;
        return type & 0xF8FFFFFF;
    case OS_INTR_AI_AI:
        aiIntr = OS_AI_INTMR;
        aiIntr &= ~0x2C;

        if (!(mask & 0x800000)) {
            aiIntr |= 0x4;
        }

        OS_AI_INTMR = aiIntr;
        return type & 0xFF7FFFFF;
    case OS_INTR_EXI_0_EXI:
    case OS_INTR_EXI_0_TC:
    case OS_INTR_EXI_0_EXT:
        exi0Intr = EXI_CHAN_CTRL[EXI_CHAN_0].csr;
        exi0Intr &= ~0x2C0F;

        if (!(mask & 0x400000)) {
            exi0Intr |= 0x1;
        }
        if (!(mask & 0x200000)) {
            exi0Intr |= 0x4;
        }
        if (!(mask & 0x100000)) {
            exi0Intr |= 0x400;
        }

        EXI_CHAN_CTRL[EXI_CHAN_0].csr = exi0Intr;
        return type & 0xFF8FFFFF;
    case OS_INTR_EXI_1_EXI:
    case OS_INTR_EXI_1_TC:
    case OS_INTR_EXI_1_EXT:
        exi1Intr = EXI_CHAN_CTRL[EXI_CHAN_1].csr;
        exi1Intr &= ~0xC0F;

        if (!(mask & 0x80000)) {
            exi1Intr |= 0x1;
        }
        if (!(mask & 0x40000)) {
            exi1Intr |= 0x4;
        }
        if (!(mask & 0x20000)) {
            exi1Intr |= 0x400;
        }

        EXI_CHAN_CTRL[EXI_CHAN_1].csr = exi1Intr;
        return type & 0xFFF1FFFF;
    case OS_INTR_EXI_2_EXI:
    case OS_INTR_EXI_2_TC:
        exi2Intr = EXI_CHAN_CTRL[EXI_CHAN_2].csr;
        exi2Intr &= ~0xF;

        if (!(mask & 0x10000)) {
            exi2Intr |= 0x1;
        }
        if (!(mask & 0x8000)) {
            exi2Intr |= 0x4;
        }

        EXI_CHAN_CTRL[EXI_CHAN_2].csr = exi2Intr;
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
        piIntr = 0xF0;

        if (!(mask & 0x4000)) {
            piIntr |= 0x800;
        }
        if (!(mask & 0x800)) {
            piIntr |= 0x8;
        }
        if (!(mask & 0x400)) {
            piIntr |= 0x4;
        }
        if (!(mask & 0x200)) {
            piIntr |= 0x2;
        }
        if (!(mask & 0x100)) {
            piIntr |= 0x1;
        }
        if (!(mask & 0x80)) {
            piIntr |= 0x100;
        }
        if (!(mask & 0x40)) {
            piIntr |= 0x1000;
        }
        if (!(mask & 0x2000)) {
            piIntr |= 0x200;
        }
        if (!(mask & 0x1000)) {
            piIntr |= 0x400;
        }
        if (!(mask & 0x20)) {
            piIntr |= 0x2000;
        }
        if (!(mask & 0x10)) {
            piIntr |= 0x4000;
        }

        OS_PI_INTMR = piIntr;
        return type & 0xFFFF800F;
    default:
        return type;
    }
}

u32 __OSMaskInterrupts(u32 userMask) {
    const BOOL enabled = OSDisableInterrupts();
    const u32 prevMask = *(u32*)OSPhysicalToCached(OS_PHYS_PREV_INTR_MASK);
    const u32 currMask = *(u32*)OSPhysicalToCached(OS_PHYS_CURRENT_INTR_MASK);

    u32 workMask = userMask & ~(prevMask | currMask);
    userMask = prevMask | userMask;
    *(u32*)OSPhysicalToCached(OS_PHYS_PREV_INTR_MASK) = userMask;

    while (workMask != 0) {
        workMask = SetInterruptMask(workMask, userMask | currMask);
    }

    OSRestoreInterrupts(enabled);
    return prevMask;
}

u32 __OSUnmaskInterrupts(u32 userMask) {
    const BOOL enabled = OSDisableInterrupts();
    const u32 prevMask = *(u32*)OSPhysicalToCached(OS_PHYS_PREV_INTR_MASK);
    const u32 currMask = *(u32*)OSPhysicalToCached(OS_PHYS_CURRENT_INTR_MASK);

    u32 workMask = userMask & (prevMask | currMask);
    userMask = prevMask & ~userMask;
    *(u32*)OSPhysicalToCached(OS_PHYS_PREV_INTR_MASK) = userMask;

    while (workMask != 0) {
        workMask = SetInterruptMask(workMask, userMask | currMask);
    }

    OSRestoreInterrupts(enabled);
    return prevMask;
}

void __OSDispatchInterrupt(u8 intr, OSContext* ctx) {
    u32 intsr;
    u32 cause, cause2;
    u32 miMask;
    u32 dspMask;
    u32 exi0Mask, exi1Mask, exi2Mask;
    u32 piMask;

    intsr = OS_PI_INTSR;
    intsr &= ~0x10000;

    piMask = OS_PI_INTMR;
    if (intsr == 0 || !(intsr & piMask)) {
        OSLoadContext(ctx);
    }

    cause = 0;
    cause2 = 0;

    if (intsr & 0x80) {
        miMask = OS_MI_CC00401E;
        if (miMask & 0x1) {
            cause |= 0x80000000;
        }
        if (miMask & 0x2) {
            cause |= 0x40000000;
        }
        if (miMask & 0x4) {
            cause |= 0x20000000;
        }
        if (miMask & 0x8) {
            cause |= 0x10000000;
        }
        if (miMask & 0x10) {
            cause |= 0x8000000;
        }
    }

    if (intsr & 0x40) {
        dspMask = OS_DSP_CSR;
        if (dspMask & 0x8) {
            cause |= 0x4000000;
        }
        if (dspMask & 0x20) {
            cause |= 0x2000000;
        }
        if (dspMask & 0x80) {
            cause |= 0x1000000;
        }
    }

    if ((intsr & 0x20) && (OS_AI_INTMR & 0x8)) {
        cause |= 0x800000;
    }

    if (intsr & 0x10) {
        exi0Mask = EXI_CHAN_CTRL[EXI_CHAN_0].csr;
        if (exi0Mask & 0x2) {
            cause |= 0x400000;
        }
        if (exi0Mask & 0x8) {
            cause |= 0x200000;
        }
        if (exi0Mask & 0x800) {
            cause |= 0x100000;
        }

        exi1Mask = EXI_CHAN_CTRL[EXI_CHAN_1].csr;
        if (exi1Mask & 0x2) {
            cause |= 0x80000;
        }
        if (exi1Mask & 0x8) {
            cause |= 0x40000;
        }
        if (exi1Mask & 0x800) {
            cause |= 0x20000;
        }

        exi2Mask = EXI_CHAN_CTRL[EXI_CHAN_2].csr;
        if (exi2Mask & 0x2) {
            cause |= 0x10000;
        }
        if (exi2Mask & 0x8) {
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
                       *(u32*)OSPhysicalToCached(OS_PHYS_CURRENT_INTR_MASK));

    if (cause2 != 0) {
        s16 type;
        u32* pPrio;
        OSInterruptHandler handler;

        for (pPrio = InterruptPrioTable; TRUE; pPrio++) {
            if (cause2 & *pPrio) {
                type = __cntlzw(cause2 & *pPrio);
                break;
            }
        }

        handler = __OSGetInterruptHandler(type);
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

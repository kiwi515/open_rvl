#include <BASE.h>
#include <DB.h>
#include <OS.h>

static asm void __OSLoadFPUContext(UNKWORD unused, register OSContext* ctx) {
    // clang-format off
    nofralloc

    lhz r5, ctx->SHORT_0x1A2
    clrlwi. r5, r5, 31
    beq _exit
    
    lfd f0, ctx->fpscr_tmp
    mtfs f0
    mfspr r5, 0x398
    rlwinm. r5, r5, 3, 31, 31
    beq _load_fprs
    
    psq_l f0, 0x1C8(ctx), 0, 0
    psq_l f1, 0x1D0(ctx), 0, 0
    psq_l f2, 0x1D8(ctx), 0, 0
    psq_l f3, 0x1E0(ctx), 0, 0
    psq_l f4, 0x1E8(ctx), 0, 0
    psq_l f5, 0x1F0(ctx), 0, 0
    psq_l f6, 0x1F8(ctx), 0, 0
    psq_l f7, 0x200(ctx), 0, 0
    psq_l f8, 0x208(ctx), 0, 0
    psq_l f9, 0x210(ctx), 0, 0
    psq_l f10, 0x218(ctx), 0, 0
    psq_l f11, 0x220(ctx), 0, 0
    psq_l f12, 0x228(ctx), 0, 0
    psq_l f13, 0x230(ctx), 0, 0
    psq_l f14, 0x238(ctx), 0, 0
    psq_l f15, 0x240(ctx), 0, 0
    psq_l f16, 0x248(ctx), 0, 0
    psq_l f17, 0x250(ctx), 0, 0
    psq_l f18, 0x258(ctx), 0, 0
    psq_l f19, 0x260(ctx), 0, 0
    psq_l f20, 0x268(ctx), 0, 0
    psq_l f21, 0x270(ctx), 0, 0
    psq_l f22, 0x278(ctx), 0, 0
    psq_l f23, 0x280(ctx), 0, 0
    psq_l f24, 0x288(ctx), 0, 0
    psq_l f25, 0x290(ctx), 0, 0
    psq_l f26, 0x298(ctx), 0, 0
    psq_l f27, 0x2A0(ctx), 0, 0
    psq_l f28, 0x2A8(ctx), 0, 0
    psq_l f29, 0x2B0(ctx), 0, 0
    psq_l f30, 0x2B8(ctx), 0, 0
    psq_l f31, 0x2C0(ctx), 0, 0

_load_fprs:
    lfd f0, ctx->fprs[0]
    lfd f1, ctx->fprs[1]
    lfd f2, ctx->fprs[2]
    lfd f3, ctx->fprs[3]
    lfd f4, ctx->fprs[4]
    lfd f5, ctx->fprs[5]
    lfd f6, ctx->fprs[6]
    lfd f7, ctx->fprs[7]
    lfd f8, ctx->fprs[8]
    lfd f9, ctx->fprs[9]
    lfd f10, ctx->fprs[10]
    lfd f11, ctx->fprs[11]
    lfd f12, ctx->fprs[12]
    lfd f13, ctx->fprs[13]
    lfd f14, ctx->fprs[14]
    lfd f15, ctx->fprs[15]
    lfd f16, ctx->fprs[16]
    lfd f17, ctx->fprs[17]
    lfd f18, ctx->fprs[18]
    lfd f19, ctx->fprs[19]
    lfd f20, ctx->fprs[20]
    lfd f21, ctx->fprs[21]
    lfd f22, ctx->fprs[22]
    lfd f23, ctx->fprs[23]
    lfd f24, ctx->fprs[24]
    lfd f25, ctx->fprs[25]
    lfd f26, ctx->fprs[26]
    lfd f27, ctx->fprs[27]
    lfd f28, ctx->fprs[28]
    lfd f29, ctx->fprs[29]
    lfd f30, ctx->fprs[30]
    lfd f31, ctx->fprs[31]

_exit:
    blr
    // clang-format on
}

static asm void __OSSaveFPUContext(UNKWORD unused, UNKWORD unused1,
                                   register OSContext* ctx) {
    // clang-format off
    nofralloc
    
    lhz r3, ctx->SHORT_0x1A2
    ori r3, r3, 1
    sth r3, ctx->SHORT_0x1A2

    stfd f0, ctx->fprs[0]
    stfd f1, ctx->fprs[1]
    stfd f2, ctx->fprs[2]
    stfd f3, ctx->fprs[3]
    stfd f4, ctx->fprs[4]
    stfd f5, ctx->fprs[5]
    stfd f6, ctx->fprs[6]
    stfd f7, ctx->fprs[7]
    stfd f8, ctx->fprs[8]
    stfd f9, ctx->fprs[9]
    stfd f10, ctx->fprs[10]
    stfd f11, ctx->fprs[11]
    stfd f12, ctx->fprs[12]
    stfd f13, ctx->fprs[13]
    stfd f14, ctx->fprs[14]
    stfd f15, ctx->fprs[15]
    stfd f16, ctx->fprs[16]
    stfd f17, ctx->fprs[17]
    stfd f18, ctx->fprs[18]
    stfd f19, ctx->fprs[19]
    stfd f20, ctx->fprs[20]
    stfd f21, ctx->fprs[21]
    stfd f22, ctx->fprs[22]
    stfd f23, ctx->fprs[23]
    stfd f24, ctx->fprs[24]
    stfd f25, ctx->fprs[25]
    stfd f26, ctx->fprs[26]
    stfd f27, ctx->fprs[27]
    stfd f28, ctx->fprs[28]
    stfd f29, ctx->fprs[29]
    stfd f30, ctx->fprs[30]
    stfd f31, ctx->fprs[31]

    mffs f0
    stfd f0, ctx->fpscr_tmp
    lfd f0, ctx->fprs[0]
    mfspr r3, 0x398
    rlwinm. r3, r3, 3, 31, 31
    beq _exit

    psq_st f0, 0x1C8(ctx), 0, 0
    psq_st f1, 0x1D0(ctx), 0, 0
    psq_st f2, 0x1D8(ctx), 0, 0
    psq_st f3, 0x1E0(ctx), 0, 0
    psq_st f4, 0x1E8(ctx), 0, 0
    psq_st f5, 0x1F0(ctx), 0, 0
    psq_st f6, 0x1F8(ctx), 0, 0
    psq_st f7, 0x200(ctx), 0, 0
    psq_st f8, 0x208(ctx), 0, 0
    psq_st f9, 0x210(ctx), 0, 0
    psq_st f10, 0x218(ctx), 0, 0
    psq_st f11, 0x220(ctx), 0, 0
    psq_st f12, 0x228(ctx), 0, 0
    psq_st f13, 0x230(ctx), 0, 0
    psq_st f14, 0x238(ctx), 0, 0
    psq_st f15, 0x240(ctx), 0, 0
    psq_st f16, 0x248(ctx), 0, 0
    psq_st f17, 0x250(ctx), 0, 0
    psq_st f18, 0x258(ctx), 0, 0
    psq_st f19, 0x260(ctx), 0, 0
    psq_st f20, 0x268(ctx), 0, 0
    psq_st f21, 0x270(ctx), 0, 0
    psq_st f22, 0x278(ctx), 0, 0
    psq_st f23, 0x280(ctx), 0, 0
    psq_st f24, 0x288(ctx), 0, 0
    psq_st f25, 0x290(ctx), 0, 0
    psq_st f26, 0x298(ctx), 0, 0
    psq_st f27, 0x2A0(ctx), 0, 0
    psq_st f28, 0x2A8(ctx), 0, 0
    psq_st f29, 0x2B0(ctx), 0, 0
    psq_st f30, 0x2B8(ctx), 0, 0
    psq_st f31, 0x2C0(ctx), 0, 0

_exit:
    blr
    // clang-format on
}

asm void OSSaveFPUContext(register OSContext* ctx) {
    // clang-format off
    nofralloc

    addi r5, ctx, 0
    b __OSSaveFPUContext
    // clang-format on
}

asm void OSSetCurrentContext(register OSContext* ctx) {
    // clang-format off
    nofralloc

    lis r4, 0x80000000@ha
    stw ctx, OS_CURRENT_CONTEXT@l(r4)

    clrlwi r5, ctx, 2
    stw r5, OS_CURRENT_CONTEXT_PHYS@l(r4)

    lwz r5, OS_CURRENT_FPU_CONTEXT@l(r4)
    cmpw r5, ctx
    bne _not_current_fpu_ctx

    lwz r6, ctx->srr1
    ori r6, r6, 0x2000
    stw r6, ctx->srr1
    mfmsr r6
    ori r6, r6, 2
    mtmsr r6
    blr

_not_current_fpu_ctx:
    lwz r6, ctx->srr1
    rlwinm r6, r6, 0, 19, 17
    stw r6, ctx->srr1
    mfmsr r6
    rlwinm r6, r6, 0, 19, 17
    ori r6, r6, 2
    mtmsr r6

    isync
    blr
    // clang-format on
}

OSContext* OSGetCurrentContext(void) { return OS_CURRENT_CONTEXT; }

asm BOOL OSSaveContext(register OSContext* ctx) {
    // clang-format off
    nofralloc

    stmw r13, ctx->gprs[13]

    mfgqr1 r0
    stw r0, ctx->gqrs[1]
    mfgqr2 r0
    stw r0, ctx->gqrs[2]
    mfgqr3 r0
    stw r0, ctx->gqrs[3]
    mfgqr4 r0
    stw r0, ctx->gqrs[4]
    mfgqr5 r0
    stw r0, ctx->gqrs[5]
    mfgqr6 r0
    stw r0, ctx->gqrs[6]
    mfgqr7 r0
    stw r0, ctx->gqrs[7]

    mfcr r0
    stw r0, ctx->cr

    mflr r0
    stw r0, ctx->lr
    stw r0, ctx->srr0

    mfmsr r0
    stw r0, ctx->srr1

    mfctr r0
    stw r0, ctx->ctr

    mfxer r0
    stw r0, ctx->xer

    stw r1, ctx->gprs[1]
    stw r2, ctx->gprs[2]

    li r0, 1
    stw r0, ctx->gprs[3]

    li r3, 0
    blr
    // clang-format on
}

asm void OSLoadContext(register OSContext* ctx) {
    // clang-format off
    nofralloc

    // If the context was in OSDisableInterrupts,
    // jump back to the beginning of the function
    lis r4, __RAS_OSDisableInterrupts_begin@ha
    lwz r6, ctx->srr0
    addi r5, r4, __RAS_OSDisableInterrupts_begin@l
    cmplw r6, r5
    ble _srr0_not_in_disableintr
    lis r4, __RAS_OSDisableInterrupts_end@ha
    addi r0, r4, __RAS_OSDisableInterrupts_end@l
    cmplw r6, r0
    bge _srr0_not_in_disableintr
    stw r5, ctx->srr0
        
_srr0_not_in_disableintr:
    lwz r0, ctx->gprs[0]
    lwz r1, ctx->gprs[1]
    lwz r2, ctx->gprs[2]
    lhz r4, ctx->SHORT_0x1A2
    rlwinm. r5, r4, 0, 30, 30
    beq _load_saved_gprs

    rlwinm r4, r4, 0, 31, 29
    sth r4, ctx->SHORT_0x1A2
    lmw r5, ctx->gprs[5]
    b _load_special_regs
    
_load_saved_gprs:
    lmw r13, ctx->gprs[13]
    
_load_special_regs:
    lwz r4, ctx->gqrs[1]
    mtgqr1 r4
    lwz r4, ctx->gqrs[2]
    mtgqr2 r4
    lwz r4, ctx->gqrs[3]
    mtgqr3 r4
    lwz r4, ctx->gqrs[4]
    mtgqr4 r4
    lwz r4, ctx->gqrs[5]
    mtgqr5 r4
    lwz r4, ctx->gqrs[6]
    mtgqr6 r4
    lwz r4, ctx->gqrs[7]
    mtgqr7 r4

    lwz r4, ctx->cr
    mtcr r4
    lwz r4, ctx->lr
    mtlr r4
    lwz r4, ctx->ctr
    mtctr r4
    lwz r4, ctx->xer
    mtxer r4

    mfmsr r4
    rlwinm r4, r4, 0, 17, 15
    rlwinm r4, r4, 0, 31, 29
    mtmsr r4

    lwz r4, ctx->srr0
    mtsrr0 r4

    lwz r4, ctx->srr1
    mtsrr1 r4

    lwz r4, ctx->gprs[4]
    lwz r3, ctx->gprs[3]

    rfi
    // clang-format on
}

asm void* OSGetStackPointer(void) {
    // clang-format off
    nofralloc

    mr r3, r1
    blr
    // clang-format on
}

asm void OSSwitchFiber(register void* func, register void* stack) {
    // clang-format off
    nofralloc

    mflr r0
    // Back chain
    mr r5, r1
    stwu r5, -8(stack)
    // LR save
    mr r1, stack
    stw r0, 4(r5)
    // Call function
    mtlr func
    blrl

    // Switch back
    lwz r5, 0(r1)
    lwz r0, 4(r5)
    mtlr r0
    mr r1, r5
    blr
    // clang-format on
}

asm void OSSwitchFiberEx(u32 r3, u32 r4, u32 r5, u32 r6, register void* func,
                         register void* stack) {
    // clang-format off
    nofralloc

    mflr r0
    // Back chain
    mr r9, r1
    stwu r9, -8(stack)
    // LR save
    mr r1, stack
    stw r0, 4(r9)
    // Call function (args in r3-r6)
    mtlr func
    blrl

    // Switch back
    lwz r5, 0(r1)
    lwz r0, 4(r5)
    mtlr r0
    mr r1, r5
    blr
    // clang-format on
}

void OSClearContext(OSContext* ctx) {
    ctx->SHORT_0x1A0 = 0;
    ctx->SHORT_0x1A2 = 0;

    if (ctx == OS_CURRENT_FPU_CONTEXT) {
        OS_CURRENT_FPU_CONTEXT = NULL;
    }
}

asm void OSInitContext(register OSContext* ctx, register void* _srr0,
                       register void* stack) {
    // clang-format off
    nofralloc
    
    stw _srr0, ctx->srr0
    stw stack, ctx->gprs[1]

    li r11, 0
    ori r11, r11, 0x9032
    stw r11, ctx->srr1

    li r0, 0
    stw r0, ctx->cr
    stw r0, ctx->xer

    stw r2, ctx->gprs[2]
    stw r13, ctx->gprs[13]

    stw r0, ctx->gprs[3]
    stw r0, ctx->gprs[4]
    stw r0, ctx->gprs[5]
    stw r0, ctx->gprs[6]
    stw r0, ctx->gprs[7]
    stw r0, ctx->gprs[8]
    stw r0, ctx->gprs[9]
    stw r0, ctx->gprs[10]
    stw r0, ctx->gprs[11]
    stw r0, ctx->gprs[12]
    stw r0, ctx->gprs[14]
    stw r0, ctx->gprs[15]
    stw r0, ctx->gprs[16]
    stw r0, ctx->gprs[17]
    stw r0, ctx->gprs[18]
    stw r0, ctx->gprs[19]
    stw r0, ctx->gprs[20]
    stw r0, ctx->gprs[21]
    stw r0, ctx->gprs[22]
    stw r0, ctx->gprs[23]
    stw r0, ctx->gprs[24]
    stw r0, ctx->gprs[25]
    stw r0, ctx->gprs[26]
    stw r0, ctx->gprs[27]
    stw r0, ctx->gprs[28]
    stw r0, ctx->gprs[29]
    stw r0, ctx->gprs[30]
    stw r0, ctx->gprs[31]

    stw r0, ctx->gqrs[0]
    stw r0, ctx->gqrs[1]
    stw r0, ctx->gqrs[2]
    stw r0, ctx->gqrs[3]
    stw r0, ctx->gqrs[4]
    stw r0, ctx->gqrs[5]
    stw r0, ctx->gqrs[6]
    stw r0, ctx->gqrs[7]

    b OSClearContext
    // clang-format on
}

void OSDumpContext(const OSContext* ctx) {
    u32 i;
    u32* sp;

    OSReport(
        "------------------------- Context 0x%08x -------------------------\n",
        ctx);

    for (i = 0; i < 16; i++) {
        OSReport("r%-2d  = 0x%08x (%14d)  r%-2d  = 0x%08x (%14d)\n", i,
                 ctx->gprs[i], ctx->gprs[i], i + 16, ctx->gprs[i + 16],
                 ctx->gprs[i + 16]);
    }

    OSReport("LR   = 0x%08x                   CR   = 0x%08x\n", ctx->lr,
             ctx->cr);
    OSReport("SRR0 = 0x%08x                   SRR1 = 0x%08x\n", ctx->srr0,
             ctx->srr1);

    OSReport("\nGQRs----------\n");
    for (i = 0; i < 4; i++) {
        OSReport("gqr%d = 0x%08x \t gqr%d = 0x%08x\n", i, ctx->gqrs[i], i + 4,
                 ctx->gqrs[i + 4]);
    }

    if (ctx->SHORT_0x1A2 & 1) {
        OSContext tempCtx;
        BOOL enabled = OSDisableInterrupts();
        OSContext* currCtx = OSGetCurrentContext();

        OSClearContext(&tempCtx);
        OSSetCurrentContext(&tempCtx);

        OSReport("\n\nFPRs----------\n");
        for (i = 0; i < 32; i += 2) {
            OSReport("fr%d \t= %d \t fr%d \t= %d\n", i, (u32)ctx->fprs[i],
                     i + 1, (u32)ctx->fprs[i + 1]);
        }

        OSReport("\n\nPSFs----------\n");
        for (i = 0; i < 32; i += 2) {
            OSReport("ps%d \t= 0x%x \t ps%d \t= 0x%x\n", i, (u32)ctx->psfs[i],
                     i + 1, (u32)ctx->psfs[i + 1]);
        }

        OSClearContext(&tempCtx);
        OSSetCurrentContext(currCtx);
        OSRestoreInterrupts(enabled);
    }

    OSReport("\nAddress:      Back Chain    LR Save\n");
    i = 0;
    sp = (u32*)ctx->gprs[1];
    for (; sp != NULL && sp != (u32*)0xFFFFFFFF && i++ < 16; sp = (u32*)*sp) {
        OSReport("0x%08x:   0x%08x    0x%08x\n", sp, *sp, sp[1]);
    }
}

static asm void OSSwitchFPUContext(register u8 err, register OSContext* ctx) {
    // clang-format off
    nofralloc

    mfmsr r5
    ori r5, r5, MSR_FP
    mtmsr r5
    
    isync
    
    lwz r5, ctx->srr1
    ori r5, r5, 0x2000
    mtsrr1 r5

    lis r3, OS_CURRENT_FPU_CONTEXT@ha
    lwz r5, OS_CURRENT_FPU_CONTEXT@l(r3)
    stw ctx, OS_CURRENT_FPU_CONTEXT@l(r3)
    cmpw r5, ctx
    beq _ctx_is_curr_fpu_ctx
    cmpwi r5, 0
    beq _ctx_is_null
    bl __OSSaveFPUContext

_ctx_is_null:
    bl __OSLoadFPUContext

_ctx_is_curr_fpu_ctx:
    lwz r3, ctx->cr
    mtcr r3
    lwz r3, ctx->lr
    mtlr r3
    lwz r3, ctx->srr0
    mtsrr0 r3
    lwz r3, ctx->ctr
    mtctr r3
    lwz r3, ctx->xer
    mtxer r3

    lhz r3, ctx->SHORT_0x1A2
    rlwinm r3, r3, 0, 31, 29
    sth r3, ctx->SHORT_0x1A2

    lwz r5, ctx->gprs[5]
    lwz r3, ctx->gprs[3]
    lwz r4, ctx->gprs[4]

    rfi
    // clang-format on
}

void __OSContextInit(void) {
    __OSSetExceptionHandler(OS_ERR_FP_UNAVAIL, OSSwitchFPUContext);
    OS_CURRENT_FPU_CONTEXT = NULL;
    DBPrintf("FPU-unavailable handler installed\n");
}

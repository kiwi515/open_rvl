#include "OSContext.h"
#include "OS.h"
#include "OSError.h"
#include "OSInterrupt.h"

#include <NdevExi2AD/db.h>

static asm void __OSLoadFPUContext(UNKWORD unused, register OSContext* ctx) {
    // clang-format off
    nofralloc

    lhz r5, 0x1a2(ctx)
    clrlwi. r5, r5, 0x1f
    beq exit
    
    lfd f0, 0x190(ctx)
    mtfsf 0xff, f0
    mfspr r5, 0x398
    rlwinm. r5, r5, 3, 0x1f, 0x1f
    beq load_fprs
    
    psq_l f0, 456(ctx), 0, 0
    psq_l f1, 464(ctx), 0, 0
    psq_l f2, 472(ctx), 0, 0
    psq_l f3, 480(ctx), 0, 0
    psq_l f4, 488(ctx), 0, 0
    psq_l f5, 496(ctx), 0, 0
    psq_l f6, 504(ctx), 0, 0
    psq_l f7, 512(ctx), 0, 0
    psq_l f8, 520(ctx), 0, 0
    psq_l f9, 528(ctx), 0, 0
    psq_l f10, 536(ctx), 0, 0
    psq_l f11, 544(ctx), 0, 0
    psq_l f12, 552(ctx), 0, 0
    psq_l f13, 560(ctx), 0, 0
    psq_l f14, 568(ctx), 0, 0
    psq_l f15, 576(ctx), 0, 0
    psq_l f16, 584(ctx), 0, 0
    psq_l f17, 592(ctx), 0, 0
    psq_l f18, 600(ctx), 0, 0
    psq_l f19, 608(ctx), 0, 0
    psq_l f20, 616(ctx), 0, 0
    psq_l f21, 624(ctx), 0, 0
    psq_l f22, 632(ctx), 0, 0
    psq_l f23, 640(ctx), 0, 0
    psq_l f24, 648(ctx), 0, 0
    psq_l f25, 656(ctx), 0, 0
    psq_l f26, 664(ctx), 0, 0
    psq_l f27, 672(ctx), 0, 0
    psq_l f28, 680(ctx), 0, 0
    psq_l f29, 688(ctx), 0, 0
    psq_l f30, 696(ctx), 0, 0
    psq_l f31, 704(ctx), 0, 0

load_fprs:
    lfd f0, 0x90(ctx)
    lfd f1, 0x98(ctx)
    lfd f2, 0xa0(ctx)
    lfd f3, 0xa8(ctx)
    lfd f4, 0xb0(ctx)
    lfd f5, 0xb8(ctx)
    lfd f6, 0xc0(ctx)
    lfd f7, 0xc8(ctx)
    lfd f8, 0xd0(ctx)
    lfd f9, 0xd8(ctx)
    lfd f10, 0xe0(ctx)
    lfd f11, 0xe8(ctx)
    lfd f12, 0xf0(ctx)
    lfd f13, 0xf8(ctx)
    lfd f14, 0x100(ctx)
    lfd f15, 0x108(ctx)
    lfd f16, 0x110(ctx)
    lfd f17, 0x118(ctx)
    lfd f18, 0x120(ctx)
    lfd f19, 0x128(ctx)
    lfd f20, 0x130(ctx)
    lfd f21, 0x138(ctx)
    lfd f22, 0x140(ctx)
    lfd f23, 0x148(ctx)
    lfd f24, 0x150(ctx)
    lfd f25, 0x158(ctx)
    lfd f26, 0x160(ctx)
    lfd f27, 0x168(ctx)
    lfd f28, 0x170(ctx)
    lfd f29, 0x178(ctx)
    lfd f30, 0x180(ctx)
    lfd f31, 0x188(ctx)

exit:
    blr
    // clang-format on
}

asm static void __OSSaveFPUContext(UNKWORD unused, UNKWORD unused1,
                                   register OSContext* ctx) {
    // clang-format off
    nofralloc
    
    lhz r3, 0x1a2(ctx)
    ori r3, r3, 1
    sth r3, 0x1a2(ctx)

    stfd f0, 0x90(ctx)
    stfd f1, 0x98(ctx)
    stfd f2, 0xa0(ctx)
    stfd f3, 0xa8(ctx)
    stfd f4, 0xb0(ctx)
    stfd f5, 0xb8(ctx)
    stfd f6, 0xc0(ctx)
    stfd f7, 0xc8(ctx)
    stfd f8, 0xd0(ctx)
    stfd f9, 0xd8(ctx)
    stfd f10, 0xe0(ctx)
    stfd f11, 0xe8(ctx)
    stfd f12, 0xf0(ctx)
    stfd f13, 0xf8(ctx)
    stfd f14, 0x100(ctx)
    stfd f15, 0x108(ctx)
    stfd f16, 0x110(ctx)
    stfd f17, 0x118(ctx)
    stfd f18, 0x120(ctx)
    stfd f19, 0x128(ctx)
    stfd f20, 0x130(ctx)
    stfd f21, 0x138(ctx)
    stfd f22, 0x140(ctx)
    stfd f23, 0x148(ctx)
    stfd f24, 0x150(ctx)
    stfd f25, 0x158(ctx)
    stfd f26, 0x160(ctx)
    stfd f27, 0x168(ctx)
    stfd f28, 0x170(ctx)
    stfd f29, 0x178(ctx)
    stfd f30, 0x180(ctx)
    stfd f31, 0x188(ctx)

    mffs f0
    stfd f0, 0x190(ctx)
    lfd f0, 0x90(ctx)
    mfspr r3, 0x398
    rlwinm. r3, r3, 3, 0x1f, 0x1f
    beq exit

    psq_st f0, 456(ctx), 0, 0
    psq_st f1, 464(ctx), 0, 0
    psq_st f2, 472(ctx), 0, 0
    psq_st f3, 480(ctx), 0, 0
    psq_st f4, 488(ctx), 0, 0
    psq_st f5, 496(ctx), 0, 0
    psq_st f6, 504(ctx), 0, 0
    psq_st f7, 512(ctx), 0, 0
    psq_st f8, 520(ctx), 0, 0
    psq_st f9, 528(ctx), 0, 0
    psq_st f10, 536(ctx), 0, 0
    psq_st f11, 544(ctx), 0, 0
    psq_st f12, 552(ctx), 0, 0
    psq_st f13, 560(ctx), 0, 0
    psq_st f14, 568(ctx), 0, 0
    psq_st f15, 576(ctx), 0, 0
    psq_st f16, 584(ctx), 0, 0
    psq_st f17, 592(ctx), 0, 0
    psq_st f18, 600(ctx), 0, 0
    psq_st f19, 608(ctx), 0, 0
    psq_st f20, 616(ctx), 0, 0
    psq_st f21, 624(ctx), 0, 0
    psq_st f22, 632(ctx), 0, 0
    psq_st f23, 640(ctx), 0, 0
    psq_st f24, 648(ctx), 0, 0
    psq_st f25, 656(ctx), 0, 0
    psq_st f26, 664(ctx), 0, 0
    psq_st f27, 672(ctx), 0, 0
    psq_st f28, 680(ctx), 0, 0
    psq_st f29, 688(ctx), 0, 0
    psq_st f30, 696(ctx), 0, 0
    psq_st f31, 704(ctx), 0, 0

exit:
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

    lis r4, OS_CURRENT_CONTEXT@ha
    stw ctx, OS_CURRENT_CONTEXT@l(r4)
    clrlwi r5, ctx, 2
    stw r5, 0xc0(r4)
    lwz r5, 0xd8(r4)
    cmpw r5, ctx
    bne lbl_800EE9AC
    lwz r6, 0x19c(ctx)
    ori r6, r6, 0x2000
    stw r6, 0x19c(ctx)
    mfmsr r6
    ori r6, r6, 2
    mtmsr r6
    blr

lbl_800EE9AC:
    lwz r6, 0x19c(ctx)
    rlwinm r6, r6, 0, 0x13, 0x11
    stw r6, 0x19c(ctx)
    mfmsr r6
    rlwinm r6, r6, 0, 0x13, 0x11
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

    stmw r13, 0x34(ctx)

    mfspr r0, GQR1
    stw r0, 0x1a8(ctx)
    mfspr r0, GQR2
    stw r0, 0x1ac(ctx)
    mfspr r0, GQR3
    stw r0, 0x1b0(ctx)
    mfspr r0, GQR4
    stw r0, 0x1b4(ctx)
    mfspr r0, GQR5
    stw r0, 0x1b8(ctx)
    mfspr r0, GQR6
    stw r0, 0x1bc(ctx)
    mfspr r0, GQR7
    stw r0, 0x1c0(ctx)

    mfcr r0
    stw r0, 0x80(ctx)
    mflr r0
    stw r0, 0x84(ctx)
    stw r0, 0x198(ctx)
    mfmsr r0
    stw r0, 0x19c(ctx)
    mfctr r0
    stw r0, 0x88(ctx)
    mfxer r0
    stw r0, 0x8c(ctx)

    stw r1, 4(ctx)
    stw r2, 8(ctx)

    li r0, 1
    stw r0, 0xc(ctx)

    li r3, 0
    blr
    // clang-format on
}

asm void OSLoadContext(register OSContext* ctx) {
    // clang-format off
    nofralloc

    lis r4, __RAS_OSDisableInterrupts_begin@ha
    lwz r6, 0x198(ctx)
    addi r5, r4, __RAS_OSDisableInterrupts_begin@l
    cmplw r6, r5
    ble srr0_not_in_disableintr
    lis r4, __RAS_OSDisableInterrupts_end@ha
    addi r0, r4, __RAS_OSDisableInterrupts_end@l
    cmplw r6, r0
    bge srr0_not_in_disableintr
    stw r5, 0x198(ctx)
        
srr0_not_in_disableintr:
    lwz r0, 0(ctx)
    lwz r1, 4(ctx)
    lwz r2, 8(ctx)
    lhz r4, 0x1a2(ctx)
    rlwinm. r5, r4, 0, 0x1e, 0x1e
    beq load_saved_gprs
    rlwinm r4, r4, 0, 0x1f, 0x1d
    sth r4, 0x1a2(ctx)
    lmw r5, 0x14(ctx)
    b load_special_regs
    
load_saved_gprs:
    lmw r13, 0x34(ctx)
    
load_special_regs:
    lwz r4, 0x1a8(ctx)
    mtspr 0x391, r4
    lwz r4, 0x1ac(ctx)
    mtspr 0x392, r4
    lwz r4, 0x1b0(ctx)
    mtspr 0x393, r4
    lwz r4, 0x1b4(ctx)
    mtspr 0x394, r4
    lwz r4, 0x1b8(ctx)
    mtspr 0x395, r4
    lwz r4, 0x1bc(ctx)
    mtspr 0x396, r4
    lwz r4, 0x1c0(ctx)
    mtspr 0x397, r4
    lwz r4, 0x80(ctx)
    mtcrf 0xff, r4
    lwz r4, 0x84(ctx)
    mtlr r4
    lwz r4, 0x88(ctx)
    mtctr r4
    lwz r4, 0x8c(ctx)
    mtxer r4
    mfmsr r4
    rlwinm r4, r4, 0, 0x11, 0xf
    rlwinm r4, r4, 0, 0x1f, 0x1d
    mtmsr r4
    lwz r4, 0x198(ctx)
    mtspr 0x1a, r4
    lwz r4, 0x19c(ctx)
    mtspr 0x1b, r4
    lwz r4, 0x10(ctx)
    lwz ctx, 0xc(ctx)
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

asm void OSInitContext(register OSContext* ctx, register void* srr0,
                       register void* stack) {
    // clang-format off
    nofralloc
    
    stw srr0, 0x198(ctx)
    stw stack, 4(ctx)
    li r11, 0
    ori r11, r11, 0x9032
    stw r11, 0x19c(ctx)
    li r0, 0
    stw r0, 0x80(ctx)
    stw r0, 0x8c(ctx)
    stw r2, 8(ctx)
    stw r13, 0x34(ctx)
    stw r0, 0xc(ctx)
    stw r0, 0x10(ctx)
    stw r0, 0x14(ctx)
    stw r0, 0x18(ctx)
    stw r0, 0x1c(ctx)
    stw r0, 0x20(ctx)
    stw r0, 0x24(ctx)
    stw r0, 0x28(ctx)
    stw r0, 0x2c(ctx)
    stw r0, 0x30(ctx)
    stw r0, 0x38(ctx)
    stw r0, 0x3c(ctx)
    stw r0, 0x40(ctx)
    stw r0, 0x44(ctx)
    stw r0, 0x48(ctx)
    stw r0, 0x4c(ctx)
    stw r0, 0x50(ctx)
    stw r0, 0x54(ctx)
    stw r0, 0x58(ctx)
    stw r0, 0x5c(ctx)
    stw r0, 0x60(ctx)
    stw r0, 0x64(ctx)
    stw r0, 0x68(ctx)
    stw r0, 0x6c(ctx)
    stw r0, 0x70(ctx)
    stw r0, 0x74(ctx)
    stw r0, 0x78(ctx)
    stw r0, 0x7c(ctx)
    stw r0, 0x1a4(ctx)
    stw r0, 0x1a8(ctx)
    stw r0, 0x1ac(ctx)
    stw r0, 0x1b0(ctx)
    stw r0, 0x1b4(ctx)
    stw r0, 0x1b8(ctx)
    stw r0, 0x1bc(ctx)
    stw r0, 0x1c0(ctx)

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
        u32 interrupt = OSDisableInterrupts();
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
        OSRestoreInterrupts(interrupt);
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
    ori r5, r5, 0x2000
    mtmsr r5
    isync
    lwz r5, 0x19c(ctx)
    ori r5, r5, 0x2000
    mtspr 0x1b, r5
    lis r3, OS_CURRENT_FPU_CONTEXT@ha
    lwz r5, OS_CURRENT_FPU_CONTEXT@l(r3)
    stw ctx, 0xd8(r3)
    cmpw r5, ctx
    beq ctx_is_curr_fpu_ctx
    cmpwi r5, 0
    beq ctx_is_null
    bl __OSSaveFPUContext

ctx_is_null:
    bl __OSLoadFPUContext

ctx_is_curr_fpu_ctx:
    lwz r3, 0x80(ctx)
    mtcrf 0xff, r3
    lwz r3, 0x84(ctx)
    mtlr r3
    lwz r3, 0x198(ctx)
    mtspr 0x1a, r3
    lwz r3, 0x88(ctx)
    mtctr r3
    lwz r3, 0x8c(ctx)
    mtxer r3
    lhz r3, 0x1a2(ctx)
    rlwinm r3, r3, 0, 0x1f, 0x1d
    sth r3, 0x1a2(ctx)
    lwz r5, 0x14(ctx)
    lwz r3, 0xc(ctx)
    lwz ctx, 0x10(ctx)
    rfi
    // clang-format on
}

void __OSContextInit(void) {
    __OSSetExceptionHandler(OS_ERR_FP_UNAVAIL, OSSwitchFPUContext);
    OS_CURRENT_FPU_CONTEXT = NULL;
    DBPrintf("FPU-unavailable handler installed\n");
}

#include "OSMemory.h"
#include "OS.h"
#include "OSCache.h"
#include "OSError.h"
#include "OSInterrupt.h"
#include "OSReset.h"

#define MEM_MB_TO_B(mb) ((mb)*1024 * 1024)

static BOOL OnShutdown(u32, u32);
static OSShutdownFunctionInfo ShutdownFunctionInfo = {OnShutdown, 127, NULL,
                                                      NULL};

u32 OSGetPhysicalMem1Size(void) { return OS_PHYSICAL_MEM1_SIZE; }

u32 OSGetPhysicalMem2Size(void) { return OS_PHYSICAL_MEM2_SIZE; }

u32 OSGetConsoleSimulatedMem1Size(void) { return OS_SIMULATED_MEM1_SIZE; }

u32 OSGetConsoleSimulatedMem2Size(void) { return OS_SIMULATED_MEM2_SIZE; }

static BOOL OnShutdown(u32 arg0, u32 arg1) {
    if (arg0 != 0) {
        OS_MI_CC004010 = 0xFF;
        __OSMaskInterrupts(0xF0000000);
    }

    return TRUE;
}

// Typo
static void MEMIntrruptHandler(u32 error, OSContext* ctx) {
    u32 dsisr = OS_MI_CC00401E;
    u32 dar = (OS_MI_CC004024 & 0x3ff) << 0x10 | OS_MI_CC004022;
    OS_MI_CC004020 = 0;

    if (__OSErrorTable[OS_ERR_PROTECTION] != NULL) {
        __OSErrorTable[OS_ERR_PROTECTION](OS_ERR_PROTECTION, ctx, dsisr, dar);
    } else {
        __OSUnhandledException(OS_ERR_PROTECTION, ctx, dsisr, dar);
    }
}

static asm void ConfigMEM1_24MB(void) {
    // clang-format off
    nofralloc

    li r7, 0
    lis r4, 0x00000002@ha
    addi r4, r4, 0x00000002@l
    lis r3, 0x800001FF@ha
    addi r3, r3, 0x800001FF@l
    lis r6, 0x01000002@ha
    addi r6, r6, 0x01000002@l
    lis r5, 0x810000FF@ha
    addi r5, r5, 0x810000FF@l
    isync
    mtdbatu 0, r7
    mtdbatl 0, r4
    mtdbatu 0, r3
    isync
    mtibatu 0, r7
    mtibatl 0, r4
    mtibatu 0, r3
    isync
    mtdbatu 2, r7
    mtdbatl 2, r6
    mtdbatu 2, r5
    isync
    mtibatu 2, r7
    mtibatl 2, r6
    mtibatu 2, r5
    isync
    mfmsr r3
    ori r3, r3, 0x30
    mtspr 0x1b, r3
    mflr r3
    mtspr 0x1a, r3
    rfi
    // clang-format on
}

static asm void ConfigMEM1_48MB(void) {
    // clang-format off
    nofralloc

    li r7, 0
    lis r4, 0x00000002@ha
    addi r4, r4, 0x00000002@l
    lis r3, 0x800003FF@ha
    addi r3, r3, 0x800003FF@l
    lis r6, 0x02000002@ha
    addi r6, r6, 0x02000002@l
    lis r5, 0x820001FF@ha
    addi r5, r5, 0x820001FF@l
    isync
    mtdbatu 0, r7
    mtdbatl 0, r4
    mtdbatu 0, r3
    isync
    mtibatu 0, r7
    mtibatl 0, r4
    mtibatu 0, r3
    isync
    mtdbatu 2, r7
    mtdbatl 2, r6
    mtdbatu 2, r5
    isync
    mtibatu 2, r7
    mtibatl 2, r6
    mtibatu 2, r5
    isync
    mfmsr r3
    ori r3, r3, 0x30
    mtspr 0x1b, r3
    mflr r3
    mtspr 0x1a, r3
    rfi
    // clang-format on
}

static asm void ConfigMEM2_52MB(void) {
    // clang-format off
    nofralloc

    li r7, 0
    lis r4, 0x10000002@ha
    addi r4, r4, 0x10000002@l
    lis r3, 0x900003FF@ha
    addi r3, r3, 0x900003FF@l
    lis r6, 0x1000002A@ha
    addi r6, r6, 0x1000002A@l
    lis r5, 0xD00007FF@ha
    addi r5, r5, 0xD00007FF@l
    isync
    mtspr 0x238, r7
    mtspr 0x239, r4
    mtspr 0x238, r3
    isync
    mtspr 0x230, r7
    mtspr 0x231, r4
    mtspr 0x230, r3
    isync
    mtspr 0x23a, r7
    mtspr 0x23b, r6
    mtspr 0x23a, r5
    isync
    mtspr 0x232, r7
    mtspr 0x233, r7
    isync
    lis r4, 0x12000002@ha
    addi r4, r4, 0x12000002@l
    lis r3, 0x920001FF@ha
    addi r3, r3, 0x920001FF@l
    lis r6, 0x13000002@ha
    addi r6, r6, 0x13000002@l
    lis r5, 0x9300007F@ha
    addi r5, r5, 0x9300007F@l
    isync
    mtspr 0x23c, r7
    mtspr 0x23d, r4
    mtspr 0x23c, r3
    isync
    mtspr 0x234, r7
    mtspr 0x235, r4
    mtspr 0x234, r3
    isync
    mtspr 0x23e, r7
    mtspr 0x23f, r6
    mtspr 0x23e, r5
    isync
    mtspr 0x236, r7
    mtspr 0x237, r6
    mtspr 0x236, r5
    isync
    mfmsr r3
    ori r3, r3, 0x30
    mtspr 0x1b, r3
    mflr r3
    mtspr 0x1a, r3
    rfi
    // clang-format on
}

static asm void ConfigMEM2_56MB(void) {
    // clang-format off
    nofralloc

    li r7, 0
    lis r4, 0x10000002@ha
    addi r4, r4, 0x10000002@l
    lis r3, 0x900003FF@ha
    addi r3, r3, 0x900003FF@l
    lis r6, 0x1000002A@ha
    addi r6, r6, 0x1000002A@l
    lis r5, 0xD00007FF@ha
    addi r5, r5, 0xD00007FF@l
    isync
    mtspr 0x238, r7
    mtspr 0x239, r4
    mtspr 0x238, r3
    isync
    mtspr 0x230, r7
    mtspr 0x231, r4
    mtspr 0x230, r3
    isync
    mtspr 0x23a, r7
    mtspr 0x23b, r6
    mtspr 0x23a, r5
    isync
    mtspr 0x232, r7
    mtspr 0x233, r7
    isync
    lis r4, 0x12000002@ha
    addi r4, r4, 0x12000002@l
    lis r3, 0x920001FF@ha
    addi r3, r3, 0x920001FF@l
    lis r6, 0x13000002@ha
    addi r6, r6, 0x13000002@l
    lis r5, 0x930000FF@ha
    addi r5, r5, 0x930000FF@l
    isync
    mtspr 0x23c, r7
    mtspr 0x23d, r4
    mtspr 0x23c, r3
    isync
    mtspr 0x234, r7
    mtspr 0x235, r4
    mtspr 0x234, r3
    isync
    mtspr 0x23e, r7
    mtspr 0x23f, r6
    mtspr 0x23e, r5
    isync
    mtspr 0x236, r7
    mtspr 0x237, r6
    mtspr 0x236, r5
    isync
    mfmsr r3
    ori r3, r3, 0x30
    mtspr 0x1b, r3
    mflr r3
    mtspr 0x1a, r3
    rfi
    // clang-format on
}

static asm void ConfigMEM2_64MB(void) {
    // clang-format off
    nofralloc

    li r7, 0
    lis r4, 0x10000002@ha
    addi r4, r4, 0x10000002@l
    lis r3, 0x900007FF@ha
    addi r3, r3, 0x900007FF@l
    lis r6, 0x1000002A@ha
    addi r6, r6, 0x1000002A@l
    lis r5, 0xD00007FF@ha
    addi r5, r5, 0xD00007FF@l
    isync
    mtspr 0x238, r7
    mtspr 0x239, r4
    mtspr 0x238, r3
    isync
    mtspr 0x230, r7
    mtspr 0x231, r4
    mtspr 0x230, r3
    isync
    mtspr 0x23a, r7
    mtspr 0x23b, r6
    mtspr 0x23a, r5
    isync
    mtspr 0x232, r7
    mtspr 0x233, r7
    isync
    mtspr 0x234, r7
    mtspr 0x235, r7
    isync
    mtspr 0x236, r7
    mtspr 0x237, r7
    isync
    mtspr 0x23c, r7
    mtspr 0x23d, r7
    isync
    mtspr 0x23e, r7
    mtspr 0x23f, r7
    isync
    mfmsr r3
    ori r3, r3, 0x30
    mtspr 0x1b, r3
    mflr r3
    mtspr 0x1a, r3
    rfi
    // clang-format on
}

static asm void ConfigMEM2_112MB(void) {
    // clang-format off
    nofralloc

    li r7, 0
    lis r4, 0x10000002@ha
    addi r4, r4, 0x10000002@l
    lis r3, 0x900007FF@ha
    addi r3, r3, 0x900007FF@l
    lis r6, 0x1000002A@ha
    addi r6, r6, 0x1000002A@l
    lis r5, 0xD0000FFF@ha
    addi r5, r5, 0xD0000FFF@l
    isync
    mtspr 0x238, r7
    mtspr 0x239, r4
    mtspr 0x238, r3
    isync
    mtspr 0x230, r7
    mtspr 0x231, r4
    mtspr 0x230, r3
    isync
    mtspr 0x23a, r7
    mtspr 0x23b, r6
    mtspr 0x23a, r5
    isync
    mtspr 0x232, r7
    mtspr 0x233, r7
    isync
    lis r4, 0x14000002@ha
    addi r4, r4, 0x14000002@l
    lis r3, 0x940003FF@ha
    addi r3, r3, 0x940003FF@l
    lis r6, 0x16000002@ha
    addi r6, r6, 0x16000002@l
    lis r5, 0x960001FF@ha
    addi r5, r5, 0x960001FF@l
    isync
    mtspr 0x23c, r7
    mtspr 0x23d, r4
    mtspr 0x23c, r3
    isync
    mtspr 0x234, r7
    mtspr 0x235, r4
    mtspr 0x234, r3
    isync
    mtspr 0x23e, r7
    mtspr 0x23f, r6
    mtspr 0x23e, r5
    isync
    mtspr 0x236, r7
    mtspr 0x237, r6
    mtspr 0x236, r5
    isync
    mfmsr r3
    ori r3, r3, 0x30
    mtspr 0x1b, r3
    mflr r3
    mtspr 0x1a, r3
    rfi
    // clang-format on
}

static asm void ConfigMEM2_128MB(void) {
    // clang-format off
    nofralloc

    li r7, 0
    lis r4, 0x10000002@ha
    addi r4, r4, 0x10000002@l
    lis r3, 0x90000FFF@ha
    addi r3, r3, 0x90000FFF@l
    lis r6, 0x1000002A@ha
    addi r6, r6, 0x1000002A@l
    lis r5, 0xD0000FFF@ha
    addi r5, r5, 0xD0000FFF@l
    isync
    mtspr 0x238, r7
    mtspr 0x239, r4
    mtspr 0x238, r3
    isync
    mtspr 0x230, r7
    mtspr 0x231, r4
    mtspr 0x230, r3
    isync
    mtspr 0x23a, r7
    mtspr 0x23b, r6
    mtspr 0x23a, r5
    isync
    mtspr 0x232, r7
    mtspr 0x233, r7
    isync
    mtspr 0x234, r7
    mtspr 0x235, r7
    isync
    mtspr 0x236, r7
    mtspr 0x237, r7
    isync
    mtspr 0x23c, r7
    mtspr 0x23d, r7
    isync
    mtspr 0x23e, r7
    mtspr 0x23f, r7
    isync
    mfmsr r3
    ori r3, r3, 0x30
    mtspr 0x1b, r3
    mflr r3
    mtspr 0x1a, r3
    rfi
    // clang-format on
}

static asm void ConfigMEM_ES1_0(void) {
    // clang-format off
    nofralloc

    li r7, 0
    lis r4, 0x00000002@ha
    addi r4, r4, 0x00000002@l
    lis r3, 0x80000FFF@ha
    addi r3, r3, 0x80000FFF@l
    isync
    mtdbatu 0, r7
    mtdbatl 0, r4
    mtdbatu 0, r3
    isync
    mtibatu 0, r7
    mtibatl 0, r4
    mtibatu 0, r3
    isync
    mfmsr r3
    ori r3, r3, 0x30
    mtspr 0x1b, r3
    mflr r3
    mtspr 0x1a, r3
    rfi
    // clang-format on
}

static asm void RealMode(register void* config) {
    // clang-format off
    nofralloc

    clrlwi config, config, 2
    mtspr 0x1a, config
    mfmsr config
    rlwinm config, config, 0, 0x1c, 0x19
    mtspr 0x1b, config
    rfi
    // clang-format on
}

static void BATConfig(void) {
    u32 mem1sim;
    u32 mem1phys;
    void* mem2end;

    if (OS_HOLLYWOOD_VER == 0) {
        // Did they mean to call the function?
        if (&OSGetPhysicalMem1Size == NULL) {
            RealMode(ConfigMEM_ES1_0);
            return;
        }
    }

    mem1sim = OSGetConsoleSimulatedMem1Size();
    mem1phys = OSGetPhysicalMem1Size();
    if (OSGetConsoleSimulatedMem1Size() < mem1phys &&
        mem1sim == MEM_MB_TO_B(24)) {
        DCInvalidateRange((void*)0x81800000, MEM_MB_TO_B(24));
        OS_MI_CC004028 = 2;
    }

    if (mem1sim <= MEM_MB_TO_B(24)) {
        RealMode(ConfigMEM1_24MB);
    } else if (mem1sim <= MEM_MB_TO_B(48)) {
        RealMode(ConfigMEM1_48MB);
    }

    mem2end = OS_MEM2_END;
    if (OSGetConsoleSimulatedMem2Size() <= MEM_MB_TO_B(64)) {
        if (mem2end <= (void*)0x93400000) {
            RealMode(ConfigMEM2_52MB);
        } else if (mem2end <= (char*)0x93400000 + MEM_MB_TO_B(4)) {
            RealMode(ConfigMEM2_56MB);
        } else {
            RealMode(ConfigMEM2_64MB);
        }
    } else if (OSGetConsoleSimulatedMem2Size() <= MEM_MB_TO_B(128)) {
        if (mem2end <= (void*)0x97000000) {
            RealMode(ConfigMEM2_112MB);
        } else {
            RealMode(ConfigMEM2_128MB);
        }
    }
}

void __OSInitMemoryProtection(void) {
    u32 interrupt = OSDisableInterrupts();

    OS_MI_CC004020 = 0;
    OS_MI_CC004010 = 0xFF;

    __OSMaskInterrupts(0xF0000000);
    __OSSetInterruptHandler(OS_ERR_SYSTEM_RESET, MEMIntrruptHandler);
    __OSSetInterruptHandler(OS_ERR_MACHINE_CHECK, MEMIntrruptHandler);
    __OSSetInterruptHandler(OS_ERR_DSI, MEMIntrruptHandler);
    __OSSetInterruptHandler(OS_ERR_ISI, MEMIntrruptHandler);
    __OSSetInterruptHandler(OS_ERR_EXT_INTERRUPT, MEMIntrruptHandler);
    OSRegisterShutdownFunction(&ShutdownFunctionInfo);

    BATConfig();
    __OSUnmaskInterrupts(0x8000000);

    OSRestoreInterrupts(interrupt);
}

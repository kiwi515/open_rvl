#include "OSSync.h"
#include "OSAddress.h"
#include "OSCache.h"

#include <TRK/__mem.h>

#define OS_INTR_SLOT_SIZE 0x100

static asm void SystemCallVector(void) {
    // clang-format off
    nofralloc

    entry __OSSystemCallVectorStart
    mfspr r9, 0x3F0
    ori r10, r9, 0x8
    mtspr 0x3F0, r10
    isync
    sync
    mtspr 0x3F0, r9
    rfi

    entry __OSSystemCallVectorEnd
    nop
    // clang-format on
}

void __OSInitSystemCall(void) {
    memcpy(OSPhysicalToCached(OS_PHYS_SYSCALL_INTR), __OSSystemCallVectorStart,
           (u32)__OSSystemCallVectorEnd - (u32)__OSSystemCallVectorStart);

    DCFlushRangeNoSync(OSPhysicalToCached(OS_PHYS_SYSCALL_INTR),
                       OS_INTR_SLOT_SIZE);
    __sync();
    ICInvalidateRange(OSPhysicalToCached(OS_PHYS_SYSCALL_INTR),
                      OS_INTR_SLOT_SIZE);
}

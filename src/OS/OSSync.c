#include "OSSync.h"
#include "OSCache.h"

#include <TRK/__mem.h>

#define OS_SYS_CALL_HANDLER ((void*)0x80000C00)
#define OS_HANDLER_SLOT_SIZE 0x100

static asm void SystemCallVector(void) {
    // clang-format off
    nofralloc

    entry __OSSystemCallVectorStart
    mfspr r9, DBSR
    ori r10, r9, 8
    mtspr DBSR, r10
    isync
    sync
    mtspr DBSR, r9
    rfi

    entry __OSSystemCallVectorEnd
    nop
    // clang-format on
}

void __OSInitSystemCall(void) {
    memcpy(OS_SYS_CALL_HANDLER, __OSSystemCallVectorStart,
           (u32)__OSSystemCallVectorEnd - (u32)__OSSystemCallVectorStart);

    DCFlushRangeNoSync(OS_SYS_CALL_HANDLER, OS_HANDLER_SLOT_SIZE);
    asm {sync}
    ICInvalidateRange(OS_SYS_CALL_HANDLER, OS_HANDLER_SLOT_SIZE);
}

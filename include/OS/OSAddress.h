#ifndef RVL_SDK_OS_ADDRESS_H
#define RVL_SDK_OS_ADDRESS_H
#include "OSContext.h"
#include "OSThread.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define OS_PHYS_PREV_INTR_MASK 0x000000C4
#define OS_PHYS_CURR_INTR_MASK 0x000000C8
#define OS_PHYS_SYSCALL_INTR 0x00000C00
#define OS_PHYS_INTR_TABLE 0x00003040

static inline void* OSPhysicalToCached(u32 ofs) {
    return (void*)(ofs + 0x80000000);
}

static inline void* OSPhysicalToUncached(u32 ofs) {
    return (void*)(ofs + 0xC0000000);
}

static inline void* OSCachedToPhysical(const void* ofs) {
    return (char*)ofs - 0x80000000;
}

#ifdef __cplusplus
}
#endif
#endif

#ifndef RVL_SDK_EXI_COMMON_H
#define RVL_SDK_EXI_COMMON_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

static inline u32 __EXISwap32(u32 val) {
    return val >> 24 & 0x000000FF | val >> 8 & 0x0000FF00 |
           val << 8 & 0x00FF0000 | val << 24 & 0xFF000000;
}

#ifdef __cplusplus
}
#endif
#endif

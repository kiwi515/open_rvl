#ifndef RVL_SDK_OS_INTERRUPT
#define RVL_SDK_OS_INTERRUPT
#include "OSContext.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*OSInterruptHandler)(u32, OSContext*);

extern u32 __OSLastInterruptSrr0;
extern s16 __OSLastInterrupt;
extern s64 __OSLastInterruptTime;

u32 OSDisableInterrupts(void);
void OSRestoreInterrupts(u32);
OSInterruptHandler __OSSetInterruptHandler(u32, OSInterruptHandler);

u32 __OSMaskInterrupts(u32);
u32 __OSUnmaskInterrupts(u32);

#ifdef __cplusplus
}
#endif
#endif

#ifndef RVL_SDK_OS_STATETM_H
#define RVL_SDK_OS_STATETM_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*OSStateCallback)(void);

OSStateCallback OSSetResetCallback(OSStateCallback);
OSStateCallback OSSetPowerCallback(OSStateCallback);
BOOL __OSInitSTM(void);
void __OSShutdownToSBY(void);
void __OSHotReset(void);
BOOL __OSGetResetButtonStateRaw(void);
s32 __OSSetVIForceDimming(u32, u32, u32);
s32 __OSSetIdleLEDMode(u32);
s32 __OSUnRegisterStateEvent(void);

#ifdef __cplusplus
}
#endif
#endif

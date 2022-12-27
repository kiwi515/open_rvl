#ifndef RVL_SDK_OS_EXEC_H
#define RVL_SDK_OS_EXEC_H
#include "OS.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

OSExecParams* DOL_EXEC_PARAMS_ADDR : 0x800030F0;

extern BOOL __OSInReboot;

void __OSGetExecParams(OSExecParams*);
void __OSLaunchMenu(void);

#ifdef __cplusplus
}
#endif
#endif

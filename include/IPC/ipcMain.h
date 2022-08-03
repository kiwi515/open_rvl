#ifndef RVL_SDK_IPC_MAIN_H
#define RVL_SDK_IPC_MAIN_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

u32 IPC_REGS[] : 0xCD000000;

void IPCInit(void);
u32 IPCReadReg(s32);
void IPCWriteReg(s32, u32);
void* IPCGetBufferHi(void);
void* IPCGetBufferLo(void);
void IPCSetBufferLo(void*);

#ifdef __cplusplus
}
#endif
#endif

#ifndef RVL_SDK_OS_RESET_H
#define RVL_SDK_OS_RESET_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef BOOL (*OSShutdownFunction)(u32, u32);

typedef struct OSShutdownFunctionInfo {
    OSShutdownFunction func;             // at 0x0
    u32 prio;                            // at 0x4
    struct OSShutdownFunctionInfo* next; // at 0x8
    struct OSShutdownFunctionInfo* prev; // at 0xC
} OSShutdownFunctionInfo;

typedef struct OSShutdownFunctionQueue {
    OSShutdownFunctionInfo* head; // at 0x0
    OSShutdownFunctionInfo* tail; // at 0x4
} OSShutdownFunctionQueue;

void OSRegisterShutdownFunction(OSShutdownFunctionInfo*);
BOOL __OSCallShutdownFunctions(u32, u32);
void __OSShutdownDevices(u32);
void __OSGetDiscState(u8*);
void OSShutdownSystem(void);
void OSReturnToMenu(void);
u32 OSGetResetCode(void);
void OSResetSystem(void);

#ifdef __cplusplus
}
#endif
#endif

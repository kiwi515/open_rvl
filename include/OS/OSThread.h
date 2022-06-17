#ifndef RVL_SDK_OS_THREAD
#define RVL_SDK_OS_THREAD
#include "OSContext.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSThread {
    // TO-DO: Figure out how big OSContext is
    OSContext context;
    char UNK_0x2C8[0x2FC - 0x2C8];
    struct OSThread* next; // at 0x2FC
    char UNK_0x300[0x4];
    void* stack_top; // at 0x304
    void* stack_bot; // at 0x308
    char UNK_0x30C[0xC];
} OSThread;

OSThread* OS_FIRST_THREAD : 0x800000DC;

u32 OSDisableScheduler(void);
u32 OSEnableScheduler(void);

void __OSReschedule(void);

#ifdef __cplusplus
}
#endif
#endif

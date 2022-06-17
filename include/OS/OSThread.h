#ifndef RVL_SDK_OS_THREAD_H
#define RVL_SDK_OS_THREAD_H
#include "OSContext.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define OS_PRIORITY_MIN 0
#define OS_PRIORITY_MAX 32

typedef struct OSThread {
    OSContext context;
    u16 state;    // at 0x2C8
    s32 priority; // at 0x2D0
    char UNK_0x2D4[0x2DC - 0x2D4];
    struct OSThreadQueue* queue; // at 0x2DC
    char UNK_0x2E0[0x2F0 - 0x2E0];
    struct OSMutex* mutex; // at 0x2F0
    char UNK_0x2F4[0x2FC - 0x2F4];
    struct OSThread* next; // at 0x2FC
    char UNK_0x300[0x4];
    void* stack_top; // at 0x304
    void* stack_bot; // at 0x308
    char UNK_0x30C[0xC];
} OSThread;

typedef struct OSThreadQueue {
    OSThread* THREAD_0x0;
    OSThread* THREAD_0x4;
} OSThreadQueue;

OSThread* OS_FIRST_THREAD : 0x800000DC;

void OSInitThreadQueue(OSThreadQueue*);

u32 OSDisableScheduler(void);
u32 OSEnableScheduler(void);

void __OSReschedule(void);

void OSSleepThread(OSThreadQueue*);
void OSWakeupThread(OSThreadQueue*);

#ifdef __cplusplus
}
#endif
#endif

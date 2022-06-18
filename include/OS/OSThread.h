#ifndef RVL_SDK_OS_THREAD_H
#define RVL_SDK_OS_THREAD_H
#include "OSContext.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define OS_PRIORITY_MIN 0
#define OS_PRIORITY_MAX 31

typedef struct OSThreadQueue {
    struct OSThread* head; // at 0x0
    struct OSThread* tail; // at 0x4
} OSThreadQueue;

typedef struct OSMutexQueue {
    struct OSMutex* head; // at 0x0
    struct OSMutex* tail; // at 0x4
} OSMutexQueue;

typedef struct OSThread {
    OSContext context;
    u16 state; // at 0x2C8
    char UNK_0x2EC[0x2D0 - 0x2CC];
    s32 priority; // at 0x2D0
    s32 WORD_0x2D4;
    char UNK_0x2D8[0x2DC - 0x2D8];
    OSThreadQueue* queue; // at 0x2DC
    char UNK_0x2E0[0x2F0 - 0x2E0];
    struct OSMutex* mutex;
    OSMutexQueue mutexQueue; // at 0x2F4
    struct OSThread* next;   // at 0x2FC
    char UNK_0x300[0x4];
    void* stack_top; // at 0x304
    void* stack_bot; // at 0x308
    char UNK_0x30C[0xC];
} OSThread;

OSThreadQueue OS_THREAD_QUEUE : 0x800000DC;

void OSInitThreadQueue(OSThreadQueue*);
OSThread* OSGetCurrentThread(void);

u32 OSDisableScheduler(void);
u32 OSEnableScheduler(void);

s32 __OSGetEffectivePriority(OSThread*);

void __OSPromoteThread(OSThread*, s32);

void __OSReschedule(void);

void OSSleepThread(OSThreadQueue*);
void OSWakeupThread(OSThreadQueue*);

#ifdef __cplusplus
}
#endif
#endif

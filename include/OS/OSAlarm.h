#ifndef RVL_SDK_OS_ALARM_H
#define RVL_SDK_OS_ALARM_H
#include "OSContext.h"
#include "OSThread.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSAlarm;
typedef void (*OSAlarmHandler)(struct OSAlarm*, OSContext*);

typedef struct OSAlarm {
    OSAlarmHandler handler; // at 0x0
    u32 tag;                // at 0x4
    s64 end;
    struct OSAlarm* prev; // at 0x10
    struct OSAlarm* next; // at 0x14
    s64 period;           // at 0x18
    s64 start;            // at 0x20
    void* userData;       // at 0x28
} OSAlarm;

typedef struct OSAlarmQueue {
    OSAlarm* head; // at 0x0
    OSAlarm* tail; // at 0x4
} OSAlarmQueue;

void __OSInitAlarm(void);
void OSCreateAlarm(OSAlarm*);
void OSSetAlarm(OSAlarm*, s64, OSAlarmHandler);
void OSSetPeriodicAlarm(OSAlarm*, s64, s64, OSAlarmHandler);
void OSCancelAlarm(OSAlarm*);
void OSSetAlarmTag(OSAlarm*, u32);
void OSSetAlarmUserData(OSAlarm*, void*);
void* OSGetAlarmUserData(const OSAlarm*);

#ifdef __cplusplus
}
#endif
#endif

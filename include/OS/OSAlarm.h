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
    char UNK_0x8[0x28 - 0x8];
    u32 userData; // at 0x28
} OSAlarm;

typedef struct OSAlarmQueue {
    OSAlarm* head; // at 0x0
    OSAlarm* tail; // at 0x4
} OSAlarmQueue;

void OSCreateAlarm(OSAlarm*);
void OSSetAlarm(OSAlarm*, s64, OSAlarmHandler);
void OSCancelAlarm(OSAlarm*);
void OSSetAlarmTag(OSAlarm*, u32);
void OSSetAlarmUserData(OSAlarm*, u32);
u32 OSGetAlarmUserData(OSAlarm*);

#ifdef __cplusplus
}
#endif
#endif

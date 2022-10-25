#ifndef RVL_SDK_OS_TIME_H
#define RVL_SDK_OS_TIME_H
#include "OS.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define OS_TIME_TO_MICRO_SEC(x) ((x) / (OS_BUS_SPEED >> 2))
#define OS_TIME_TO_MILLI_SEC(x) ((x) / ((OS_BUS_SPEED >> 2) / 1000))
#define OS_TIME_TO_SEC(x) (x) / ((OS_BUS_SPEED >> 2) / 1000000))

#define OS_MICRO_SEC_TO_TIME(x) ((x) * ((OS_BUS_SPEED >> 2)))
#define OS_MILLI_SEC_TO_TIME(x) ((x) * ((OS_BUS_SPEED >> 2) / 1000))
#define OS_SEC_TO_TIME(x) ((x) * ((OS_BUS_SPEED >> 2) / 1000000))

s64 OS_SYSTEM_TIME : 0x800030D8;

typedef struct OSCalendarTime {
    s32 sec;      // at 0x0
    s32 min;      // at 0x4
    s32 hour;     // at 0x8
    s32 mon_day;  // at 0xC
    s32 mon;      // at 0x10
    s32 year;     // at 0x14
    s32 week_day; // at 0x18
    s32 year_day; // at 0x1C
    s32 msec;     // at 0x20
    s32 usec;     // at 0x24
} OSCalendarTime;

s64 OSGetTime(void);
s32 OSGetTick(void);

s64 __OSGetSystemTime(void);
s64 __OSTimeToSystemTime(s64 time);

void OSTicksToCalendarTime(s64 time, OSCalendarTime* cal);
s64 OSCalendarTimeToTicks(const OSCalendarTime* cal);

#ifdef __cplusplus
}
#endif
#endif

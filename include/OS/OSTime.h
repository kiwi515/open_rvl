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

typedef struct OSCalendarTime {
    u8 temp;
} OSCalendarTime;

s64 OSGetTime(void);
s32 OSGetTick(void);

s64 __OSGetSystemTime(void);
s64 __OSTimeToSystemTime(void);

void OSTicksToCalendarTime(s64 time, OSCalendarTime* cal);
s64 OSCalendarTimeToTicks(const OSCalendarTime* cal);

#ifdef __cplusplus
}
#endif
#endif

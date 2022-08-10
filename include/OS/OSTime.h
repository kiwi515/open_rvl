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

s64 OSGetTime(void);
s32 OSGetTick(void);

#ifdef __cplusplus
}
#endif
#endif

#ifndef RVL_SDK_OS_TIME_H
#define RVL_SDK_OS_TIME_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

s64 OSGetTime(void);
s32 OSGetTick(void);

#ifdef __cplusplus
}
#endif
#endif

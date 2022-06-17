#ifndef RVL_SDK_OS_TIME
#define RVL_SDK_OS_TIME
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

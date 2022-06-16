#ifndef RVL_SDK_OS_SYNC
#define RVL_SDK_OS_SYNC
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void __OSSystemCallVectorStart(void);
void __OSSystemCallVectorEnd(void);

void __OSInitSystemCall(void);

#ifdef __cplusplus
}
#endif
#endif

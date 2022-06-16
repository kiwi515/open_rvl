#ifndef RVL_SDK_OS_CACHE
#define RVL_SDK_OS_CACHE
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void DCFlushRangeNoSync(void*, int);

void ICInvalidateRange(void*, int);

#ifdef __cplusplus
}
#endif
#endif

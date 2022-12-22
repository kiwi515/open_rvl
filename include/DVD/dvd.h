#ifndef RVL_SDK_DVD_H
#define RVL_SDK_DVD_H
#include <OS/OSAlarm.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

u32 __DVDGetCoverStatus(void);

void __DVDPrepareReset(void);
BOOL __DVDTestAlarm(OSAlarm*);

#ifdef __cplusplus
}
#endif
#endif

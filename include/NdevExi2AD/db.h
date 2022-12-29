#ifndef RVL_SDK_NDEV_EXI2AD_DB_H
#define RVL_SDK_NDEV_EXI2AD_DB_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void DBInit(void);

BOOL __DBIsExceptionMarked(u8);

void DBPrintf(const char*, ...);

#ifdef __cplusplus
}
#endif
#endif

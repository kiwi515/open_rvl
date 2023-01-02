#ifndef RVL_SDK_NDEV_EXI2AD_DB_H
#define RVL_SDK_NDEV_EXI2AD_DB_H
#include <OSGlobals.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

extern OSDebugInterface* __DBInterface;
extern BOOL DBVerbose;

void DBInit(void);
void __DBExceptionDestinationAux(void);
void __DBExceptionDestination(void);
BOOL __DBIsExceptionMarked(u8 exc);
void DBPrintf(const char* msg, ...);

#ifdef __cplusplus
}
#endif
#endif

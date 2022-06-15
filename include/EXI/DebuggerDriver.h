#ifndef RVL_SDK_DB_DEBUGGER_DRIVER_H
#define RVL_SDK_DB_DEBUGGER_DRIVER_H
#include <OS/OSInterrupt.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void DBInitComm(u8**, OSInterruptHandler);
void DBInitInterrupts(void);
u32 DBQueryData(void);
BOOL DBRead(void*, u32);
BOOL DBWrite(const void*, u32);
void DBOpen(void);
void DBClose(void);

#ifdef __cplusplus
}
#endif
#endif

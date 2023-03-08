#ifndef RVL_SDK_NDEV_EXI2AD_DEBUGGER_DRIVER_H
#define RVL_SDK_NDEV_EXI2AD_DEBUGGER_DRIVER_H
#include <revolution/OS.h>
#include <revolution/types.h>
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

static inline BOOL __DBReadMailbox(u32* mailOut) {
    return __DBEXIReadReg(0x34000200, mailOut, sizeof(*mailOut));
}

static inline BOOL __DBRead(u32 ofs, void* dest, u32 size) {
    return __DBEXIReadRam(((ofs + 0xD10000) * 0x40) & 0x3FFFFF00, dest, size);
}

static inline BOOL __DBWriteMailbox(u32 mail) {
    return __DBEXIWriteReg(0xB4000100, &mail, sizeof(mail));
}

static inline BOOL __DBWrite(u32 ofs, const void* src, u32 size) {
    return __DBEXIWriteRam(
        (((ofs + 0xD10000) * 0x40) & 0x3FFFFF00) | 0x80000000, src, size);
}

#ifdef __cplusplus
}
#endif
#endif

#include "DebuggerDriver.h"
#include "exi2.h"

#include <OS/OS.h>
#include <OS/OSContext.h>
#include <OS/OSInterrupt.h>

static u32 __DBRecvDataSize;
static u32 __DBRecvMail;
static u8 __DBEXIInputFlag;
static OSInterruptHandler __DBDbgCallback;
static OSInterruptHandler __DBMtrCallback;

static u8 __DBReadUSB_CSR(void);
static void __DBWaitForSendMail(void);

static u32 ODEMUGetSize(u32);
static BOOL ODEMUIsValidMail(u32);
static u32 ODEMUGetPc2NngcOffset(u32);
static u32 ODEMUGetPage(u32);
static u32 ODEMUGenMailData(u32, u32);

static BOOL __DBReadMailbox(u32*);
static BOOL __DBRead(u32, void*, u32);
static BOOL __DBWriteMailbox(u32);
static BOOL __DBWrite(u32, const void*, u32);

void __DBMtrHandler(u32 type, OSContext* ctx) {
    __DBEXIInputFlag = TRUE;

    if (__DBMtrCallback != NULL)
        __DBMtrCallback(0, ctx);
}

void __DBIntrHandler(u32 type, OSContext* ctx) {
    OS_PI_REG = 0x1000;
    if (__DBDbgCallback != NULL)
        __DBDbgCallback(type, ctx);
}

static void __DBCheckMailBox(void) {
    const u8 csr = __DBReadUSB_CSR();
    if (!(csr & 0x8)) {
        u32 mail;
        __DBReadMailbox(&mail);
        if (ODEMUIsValidMail(mail)) {
            __DBRecvMail = mail;
            __DBRecvDataSize = ODEMUGetSize(mail);
            __DBEXIInputFlag = TRUE;
        }
    }
}

static u8 __DBReadUSB_CSR(void) {
    u8 val;
    __DBEXIReadReg(0x34000000, &val, sizeof(val));
    return val;
}

void DBInitComm(u8** flagOut, OSInterruptHandler mtrCb) {
    const u32 msr = OSDisableInterrupts();

    *flagOut = &__DBEXIInputFlag;
    __DBMtrCallback = mtrCb;
    __DBEXIInit();

    OSRestoreInterrupts(msr);
}

#ifdef NONMATCHING
// https://decomp.me/scratch/YjmTr
void DBInitInterrupts(void) {
    __OSMaskInterrupts(0x18000);
    __OSMaskInterrupts(0x40);
    __DBDbgCallback = __DBMtrHandler;
    __OSSetInterruptHandler(0x19, __DBIntrHandler);
    __OSUnmaskInterrupts(0x40);
}
#else
#error
#endif

u32 DBQueryData(void) {
    __DBEXIInputFlag = FALSE;

    if (__DBRecvDataSize == 0) {
        const u32 msr = OSDisableInterrupts();
        __DBCheckMailBox();
        OSRestoreInterrupts(msr);
    }

    return __DBRecvDataSize;
}

BOOL DBRead(void* dest, u32 size) {
    const u32 msr = OSDisableInterrupts();

    __DBRead(ODEMUGetPc2NngcOffset(__DBRecvMail) + 0x1000, dest,
             ROUND_UP(size, 4));
    __DBRecvDataSize = 0;
    __DBEXIInputFlag = FALSE;

    OSRestoreInterrupts(msr);

    return FALSE;
}

BOOL DBWrite(const void* src, u32 size) {
    static u8 l_byOffsetCounter = 128;
    const u32 msr = OSDisableInterrupts();

    u32 ofs, mail;

    __DBWaitForSendMail();

    ofs = 0;
    if (!(++l_byOffsetCounter & 0x1)) {
        ofs = 0;
    } else {
        ofs = 0x800;
    }

    while (__DBWrite(ofs, src, ROUND_UP(size, 4)) == 0) {
    }

    __DBWaitForSendMail();

    mail = ODEMUGenMailData(l_byOffsetCounter, size);
    while (__DBWriteMailbox(mail) == 0) {
    }

    __DBWaitForSendMail();

    OSRestoreInterrupts(msr);

    return FALSE;
}

static void __DBWaitForSendMail(void) {
    while (__DBReadUSB_CSR() & 0x4) {
    }
}

void DBOpen(void) {}

void DBClose(void) {}

static u32 ODEMUGetSize(u32 mail) { return mail & 0x1FFF; }

static BOOL ODEMUIsValidMail(u32 mail) {
    return (mail & 0x1F000000) == 0x1F000000;
}

static u32 ODEMUGetPc2NngcOffset(u32 mail) {
    if (!(ODEMUGetPage(mail) & 0x1)) {
        return 0;
    }

    return 0x800;
}

static u32 ODEMUGetPage(u32 mail) { return (mail & 0xFF0000) >> 16; }

static u32 ODEMUGenMailData(u32 ofs, u32 size) {
    return (ofs & 0xff) << 0x10 | 0x1f000000 | size & 0x1fff;
}

static BOOL __DBReadMailbox(u32* mailOut) {
    return __DBEXIReadReg(0x34000200, mailOut, sizeof(*mailOut));
}

static BOOL __DBRead(u32 ofs, void* dest, u32 size) {
    return __DBEXIReadRam(((ofs + 0xD10000) * 0x40) & 0x3FFFFF00, dest, size);
}

static BOOL __DBWriteMailbox(u32 mail) {
    return __DBEXIWriteReg(0xB4000100, &mail, sizeof(mail));
}

static BOOL __DBWrite(u32 ofs, const void* src, u32 size) {
    return __DBEXIWriteRam(
        (((ofs + 0xD10000) * 0x40) & 0x3FFFFF00) | 0x80000000, src, size);
}

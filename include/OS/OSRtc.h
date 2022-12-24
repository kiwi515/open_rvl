#ifndef RVL_SDK_OS_RTC_H
#define RVL_SDK_OS_RTC_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Documentation from:
 * https://www.gc-forever.com/yagcd/chap10.html#sec10.5
 */

typedef struct OSSram {
    u16 checksum;    // at 0x0
    u16 invchecksum; // at 0x2
    u32 ead0;        // at 0x4
    u32 ead1;        // at 0x8
    u32 counterBias; // at 0xC
    u8 dispOfsH;     // at 0x10
    u8 ntd;          // at 0x11
    u8 lang;         // at 0x12
    u8 flags;        // at 0x13
} OSSram;

typedef struct OSSramEx {
    char UNK_0x0[0x1C];
    u16 wirelessPadId[4]; // at 0x1C
    char UNK_0x38[0x3C - 0x38];
    u16 gbs; // at 0x3C
    char UNK_0x3E[0x40 - 0x3E];
} OSSramEx;

void __OSInitSram(void);
OSSramEx* __OSLockSramEx(void);
BOOL __OSUnlockSramEx(BOOL);
BOOL __OSSyncSram(void);
BOOL __OSReadROM(void*, s32, const void*);
u16 OSGetWirelessID(s32);
void OSSetWirelessID(s32, u16);
u16 OSGetGbsMode(void);
void OSSetGbsMode(u16);
BOOL __OSGetRTCFlags(u32*);
BOOL __OSClearRTCFlags(void);

#ifdef __cplusplus
}
#endif
#endif

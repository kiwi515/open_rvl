#ifndef RVL_SDK_OS_H
#define RVL_SDK_OS_H
#include "OSContext.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

u32 OS_HOLLYWOOD_VER : 0x80003138;

volatile u32 OS_PI_REG : 0xCC003000;

volatile u16 OS_ARAM_DMA_ADDR_HI : 0xCC005020;
volatile u16 OS_ARAM_DMA_ADDR_LO : 0xCC005022;

volatile u16 OS_AI_DMA_ADDR_HI : 0xCC005030;
volatile u16 OS_AI_DMA_ADDR_LO : 0xCC005032;

volatile u32 OS_DI_DMA_ADDR : 0xCD006014;

typedef struct OSIOSRev {
    u8 id;        // at 0x0
    u8 verMajor;  // at 0x1
    u8 verMinor;  // at 0x2
    u8 buildMon;  // at 0x3
    u8 buildDay;  // at 0x4
    u8 buildYear; // at 0x5
} OSIOSRev;

typedef void (*OSExceptionHandler)(u8, OSContext*);

void __OSGetIOSRev(OSIOSRev*);

void __OSSetExceptionHandler(u16, OSExceptionHandler);

void OSRegisterVersion(const char*);

#ifdef __cplusplus
}
#endif
#endif

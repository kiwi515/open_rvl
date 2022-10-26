#ifndef RVL_SDK_OS_H
#define RVL_SDK_OS_H
#include "OSContext.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

u32 OS_BUS_SPEED : 0x800000F8;

volatile s32 OS_GLOBAL_800030C0[] : 0x800030C0;

volatile u16 OS_DVD_DEVICE_CODE_ADDR : 0x800030E6;

u32 OS_HOLLYWOOD_REV : 0x80003138;

volatile u32 OS_PI_CD000034 : 0xCD000034;
volatile u32 OS_PI_INTR_CAUSE : 0xCC003000;
volatile u32 OS_PI_INTR_MASK : 0xCC003004;

volatile u16 OS_MI_INTR_MASK : 0xCC00401C;

volatile u16 OS_DSP_INTR_MASK : 0xCC00500A;

volatile u16 OS_ARAM_DMA_ADDR_HI : 0xCC005020;
volatile u16 OS_ARAM_DMA_ADDR_LO : 0xCC005022;

volatile u16 OS_AI_DMA_ADDR_HI : 0xCC005030;
volatile u16 OS_AI_DMA_ADDR_LO : 0xCC005032;
volatile u32 OS_AI_INTR_MASK : 0xCD006C00;

volatile u32 OS_DI_DMA_ADDR : 0xCD006014;

typedef enum {
    OS_CONSOLE_MASK = 0xF0000000,
    OS_CONSOLE_MASK_RVL = 0x00000000,
    OS_CONSOLE_MASK_EMU = 0x10000000,
    OS_CONSOLE_MASK_TDEV = 0x20000000,

    OS_CONSOLE_RVL_PP_1 = 0x00000011,
    OS_CONSOLE_RVL_PP_2_1 = 0x00000012,
    OS_CONSOLE_RVL_PP_2_2 = 0x00000100,

    OS_CONSOLE_RVL_EMU = 0x10000008,
    OS_CONSOLE_NDEV_1_0 = 0x10000010,
    OS_CONSOLE_NDEV_1_1 = 0x10000011,
    OS_CONSOLE_NDEV_1_2 = 0x10000012,
    OS_CONSOLE_NDEV_2_0 = 0x10000020,
    OS_CONSOLE_NDEV_2_1 = 0x10000021,
} OSConsoleType;

typedef struct OSIOSRev {
    u8 idHi;       // at 0x0
    u8 idLo;       // at 0x1
    u8 verMajor;   // at 0x2
    u8 verMinor;   // at 0x3
    u8 buildMon;   // at 0x4
    u8 buildDay;   // at 0x5
    u16 buildYear; // at 0x6
} OSIOSRev;

typedef struct OSBootInfo {
    char UNK_0x0[0x2C];
    u32 consoleType; // at 0x2C
} OSBootInfo;

typedef void (*OSExceptionHandler)(u8, OSContext*);

extern BOOL __OSInIPL;
extern BOOL __OSInNandBoot;
extern s64 __OSStartTime;
extern const char* __OSVersion;

u32 __OSGetHollywoodRev(void);
void __OSGetIOSRev(OSIOSRev*);

OSExceptionHandler __OSSetExceptionHandler(u16, OSExceptionHandler);
OSExceptionHandler __OSGetExceptionHandler(u16);
void OSDefaultExceptionHandler(u8, OSContext*);
void __OSPSInit(void);
u32 __OSGetDIConfig(void);
void OSRegisterVersion(const char*);
const char* OSGetAppGamename(void);

#ifdef __cplusplus
}
#endif
#endif

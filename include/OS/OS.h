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

volatile u16 OS_VI_DISP_CONF : 0xCC002002;
volatile u16 OS_VI_CLOCK_MODE : 0xCC00206C;
volatile u16 OS_VI_DTV_STATUS : 0xCC00206E;

volatile u32 OS_PI_CD000034 : 0xCD000034;
volatile u32 OS_PI_INTR_CAUSE : 0xCC003000;
volatile u32 OS_PI_INTR_MASK : 0xCC003004;
volatile u32 OS_PI_RESET_REG : 0xCC003024;

volatile u16 OS_MI_INTR_MASK : 0xCC00401C;

volatile u16 OS_DSP_DSPMBOX_HI : 0xCC005000;
volatile u16 OS_DSP_DSPMBOX_LO : 0xCC005002;
volatile u16 OS_DSP_CPUMBOX_HI : 0xCC005004;
volatile u16 OS_DSP_CPUMBOX_LO : 0xCC005006;
volatile u16 OS_DSP_CONTROL : 0xCC00500A;
volatile u16 OS_DSP_CC005012 : 0xCC005012;
volatile u32 OS_DSP_DMA_MAINADDR : 0xCC005020;
volatile u16 OS_DSP_DMA_MAINADDR_HI : 0xCC005020;
volatile u16 OS_DSP_DMA_MAINADDR_LO : 0xCC005022;
volatile u32 OS_DSP_DMA_ARAMADDR : 0xCC005024;
volatile u16 OS_DSP_DMA_ARAMADDR_HI : 0xCC005024;
volatile u16 OS_DSP_DMA_ARAMADDR_LO : 0xCC005026;
volatile u32 OS_DSP_DMA_CONTROL : 0xCC005028;

volatile u16 OS_AI_DMA_ADDR_HI : 0xCC005030;
volatile u16 OS_AI_DMA_ADDR_LO : 0xCC005032;
volatile u16 OS_AI_DMA_CONTROL : 0xCC005036;
volatile u32 OS_AI_INTR_MASK : 0xCD006C00;

volatile u32 OS_DI_DMA_ADDR : 0xCD006014;

volatile u32 OS_CD800180 : 0xCD800180;
volatile u32 OS_CD8001CC : 0xCD8001CC;
volatile u32 OS_CD8001D0 : 0xCD8001D0;

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

typedef struct OSExecParams {
    UNKWORD WORD_0x0;
    UNKWORD WORD_0x4;
    char UNK_0x8[0x4];
    void* regionStart; // at 0xC
    void* regionEnd;   // at 0x10
    char UNK_0x14[0x1C - 0x14];
} OSExecParams;

typedef void (*OSExceptionHandler)(u8, OSContext*);

extern BOOL __OSInIPL;
extern BOOL __OSInNandBoot;
extern s64 __OSStartTime;
extern const char* __OSVersion;
extern OSExecParams __OSRebootParams;

u32 __OSGetHollywoodRev(void);
void __OSGetIOSRev(OSIOSRev*);
u32 OSGetConsoleType(void);

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

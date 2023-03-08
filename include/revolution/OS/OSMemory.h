#ifndef RVL_SDK_OS_MEMORY_H
#define RVL_SDK_OS_MEMORY_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define OS_MEM_MB_TO_B(mb) ((mb)*1024 * 1024)
#define OS_MEM_B_TO_MB(mb) ((mb) / 1024 / 1024)

#define OS_MEM_IS_MEM1(addr) (((u32)(addr)&0x30000000) == 0)
#define OS_MEM_IS_MEM2(addr) (((u32)(addr)&0x30000000) == 0x10000000)

volatile u16 OS_MI_CC004010 : 0xCC004010;
volatile u16 OS_MI_CC00401E : 0xCC00401E;
volatile u16 OS_MI_CC004020 : 0xCC004020;
volatile u16 OS_MI_CC004022 : 0xCC004022;
volatile u16 OS_MI_CC004024 : 0xCC004024;
volatile u16 OS_MI_CC004028 : 0xCC004028;

u32 OSGetPhysicalMem1Size(void);
u32 OSGetPhysicalMem2Size(void);
u32 OSGetConsoleSimulatedMem1Size(void);
u32 OSGetConsoleSimulatedMem2Size(void);
void __OSInitMemoryProtection(void);

#ifdef __cplusplus
}
#endif
#endif

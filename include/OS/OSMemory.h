#ifndef RVL_SDK_OS_MEMORY_H
#define RVL_SDK_OS_MEMORY_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

volatile u16 OS_MI_CC004010 : 0xCC004010;
volatile u16 OS_MI_CC00401E : 0xCC00401E;
volatile u16 OS_MI_CC004020 : 0xCC004020;
volatile u16 OS_MI_CC004022 : 0xCC004022;
volatile u16 OS_MI_CC004024 : 0xCC004024;
volatile u16 OS_MI_CC004028 : 0xCC004028;

u32 OS_PHYSICAL_MEM1_SIZE : 0x80003100;
u32 OS_SIMULATED_MEM1_SIZE : 0x80003104;
u32 OS_PHYSICAL_MEM2_SIZE : 0x80003118;
u32 OS_SIMULATED_MEM2_SIZE : 0x8000311C;
void* OS_MEM2_END : 0x80003120;

u32 OSGetPhysicalMem1Size(void);
u32 OSGetPhysicalMem2Size(void);
u32 OSGetConsoleSimulatedMem1Size(void);
u32 OSGetConsoleSimulatedMem2Size(void);
void __OSInitMemoryProtection(void);

#ifdef __cplusplus
}
#endif
#endif

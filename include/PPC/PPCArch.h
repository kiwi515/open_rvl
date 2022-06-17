#ifndef RVL_SDK_PPC_ARCH_H
#define RVL_SDK_PPC_ARCH_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

u32 PPCMfmsr(void);
void PPCMtmsr(u32);

u32 PPCMffpscr(void);
void PPCMtfpscr(u32);

void PPCHalt(void);

#ifdef __cplusplus
}
#endif
#endif

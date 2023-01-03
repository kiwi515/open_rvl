#ifndef RVL_SDK_VF_PF_CODE_H
#define RVL_SDK_VF_PF_CODE_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

int VFiPFCODE_Combine_Width(s16 lo, s16 hi);
void VFiPFCODE_Divide_Width(u32 ch, s16* loOut, s16* hiOut);

#ifdef __cplusplus
}
#endif
#endif

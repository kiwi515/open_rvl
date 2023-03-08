#ifndef RVL_SDK_VF_PF_CODE_H
#define RVL_SDK_VF_PF_CODE_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define VF_FN_MIN_CHAR 0x20
#define VF_FN_MAX_CHAR 0x80

typedef enum {
    PFCODE_FN_INVALID = 0,
    PFCODE_FN_VALID_LONG = (1 << 0),
    PFCODE_FN_VALID_SHORT = (1 << 1),
} PFCODE_FnValidity;

extern const u8 VFipf_valid_fn_char[VF_FN_MAX_CHAR - VF_FN_MIN_CHAR];

int VFiPFCODE_Combine_Width(s16 lo, s16 hi);
void VFiPFCODE_Divide_Width(u32 ch, s16* loOut, s16* hiOut);

#ifdef __cplusplus
}
#endif
#endif

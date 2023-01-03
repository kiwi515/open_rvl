#include "pf_code.h"

int VFiPFCODE_Combine_Width(s16 lo, s16 hi) { return (lo << 16) + hi; }

void VFiPFCODE_Divide_Width(u32 ch, s16* loOut, s16* hiOut) {
    *loOut = ch >> 16;
    *hiOut = ch;
}

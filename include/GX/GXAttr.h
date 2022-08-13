#ifndef RVL_SDK_GX_ATTR_H
#define RVL_SDK_GX_ATTR_H
#include "GX.h"
#include "GXTev.h"
#include "GXTexture.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void GXSetVtxDesc(GXAttr, GXAttr);

void __GXSetVCD(void);
void __GXCalculateVLim(void);

void GXClearVtxDesc(void);
void GXSetVtxAttrFmt(UNKWORD formatIndex, GXAttr, UNKWORD, UNKWORD, u8 frac);

void __GXSetVAT(void);

void GXSetArray(GXAttr, const void*, u8);

void GXSetTexCoordGen2(GXTexCoordID, GXTexGenType, GXTexGenSrc, UNKWORD, u8,
                       UNKWORD);
void GXSetNumTexGens(u8);

static inline void GXSetTexCoordGen(GXTexCoordID coord, GXTexGenType type,
                                    GXTexGenSrc src, UNKWORD r6) {
    GXSetTexCoordGen2(coord, type, src, r6, 0, 0x7D);
}

#ifdef __cplusplus
}
#endif
#endif

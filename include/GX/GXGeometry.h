#ifndef RVL_SDK_GX_GEOMETRY_H
#define RVL_SDK_GX_GEOMETRY_H
#include "GX.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void __GXSetDirtyState(void);
void GXBegin(GXPrimitive, UNKWORD, u16);
static void GXEnd(void) {}
void __GXSendFlushPrim(void);
void GXSetLineWidth(u8, UNKWORD);
void GXSetPointSize(u8, UNKWORD);
void GXEnableTexOffsets(UNKWORD, u8, u8);
void GXSetCullMode(GXCullMode);
void GXGetCullMode(GXCullMode*);
void GXSetCoPlanar(u8);
void __GXSetGenMode(void);

#ifdef __cplusplus
}
#endif
#endif

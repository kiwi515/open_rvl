#ifndef RVL_SDK_GX_GEOMETRY_H
#define RVL_SDK_GX_GEOMETRY_H
#include <revolution/GX/GXTypes.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

static void GXEnd(void) {}

void __GXSetDirtyState(void);
void GXBegin(GXPrimitive prim, UNKWORD fmtIndex, u16 num);
void __GXSendFlushPrim(void);
void GXSetLineWidth(u8 width, UNKWORD r5);
void GXSetPointSize(u8 width, UNKWORD r5);
void GXEnableTexOffsets(UNKWORD coordId, GXBool8 r4, GXBool8 r5);
void GXSetCullMode(GXCullMode mode);
void GXGetCullMode(GXCullMode* out);
void GXSetCoPlanar(u8 enable);
void __GXSetGenMode(void);

#ifdef __cplusplus
}
#endif
#endif

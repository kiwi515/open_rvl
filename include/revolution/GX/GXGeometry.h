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
void GXSetLineWidth(u8 width, UNKWORD arg1);
void GXSetPointSize(u8 width, UNKWORD arg1);
void GXEnableTexOffsets(UNKWORD coordId, GXBool8 arg1, GXBool8 arg2);
void GXSetCullMode(GXCullMode mode);
void GXGetCullMode(GXCullMode* out);
void GXSetCoPlanar(GXBool8 enable);
void __GXSetGenMode(void);

#ifdef __cplusplus
}
#endif
#endif

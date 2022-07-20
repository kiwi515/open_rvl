#ifndef RVL_SDK_GX_GEOMETRY_H
#define RVL_SDK_GX_GEOMETRY_H
#include "GX.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum _GXCullMode {
    GX_CULL_MODE_0,
} GXCullMode;

typedef enum _GXPrimitive {
    GX_PRIMITIVE_0,
} GXPrimitive;

typedef enum _GXDirtyFlag {
    GX_DIRTY_SU_TEX = (1 << 0),
    GX_DIRTY_BP_MASK = (1 << 1),
    GX_DIRTY_GEN_MODE = (1 << 2),
    GX_DIRTY_VCD = (1 << 3),
    GX_DIRTY_VAT = (1 << 4),
    GX_DIRTY_AMB_COLOR0 = (1 << 8),
    GX_DIRTY_AMB_COLOR1 = (1 << 9),
    GX_DIRTY_MAT_COLOR0 = (1 << 10),
    GX_DIRTY_MAT_COLOR1 = (1 << 11),
    GX_DIRTY_MTX_IDX = (1 << 26),
    GX_DIRTY_PROJECTION = (1 << 27),
    GX_DIRTY_VIEWPORT = (1 << 28),

    GX_DIRTY_VLIM = GX_DIRTY_VCD | GX_DIRTY_VAT,

    GX_AMB_MAT_MASK = GX_DIRTY_AMB_COLOR0 | GX_DIRTY_AMB_COLOR1 |
                      GX_DIRTY_MAT_COLOR0 | GX_DIRTY_MAT_COLOR1,
    GX_LIGHT_CHAN_MASK = 0x100F000,
    GX_TEX_GEN_MASK = 0x2FF0000,
} GXDirtyFlag;

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

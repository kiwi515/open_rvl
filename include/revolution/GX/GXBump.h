#ifndef RVL_SDK_GX_BUMP_H
#define RVL_SDK_GX_BUMP_H
#include <revolution/GX/GX.h>
#include <revolution/GX/GXTev.h>
#include <revolution/GX/GXTexture.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void GXSetTevIndirect(GXTevStageID, GXIndTexStageID, GXIndTexFormat,
                      GXIndTexBiasSel, GXIndTexMtxID, GXIndTexWrap,
                      GXIndTexWrap, u8, u8, GXIndTexAlphaSel);
void GXSetIndTexMtx(GXIndTexMtxID, const f32[6], s8);
void GXSetIndTexCoordScale(GXIndTexStageID, GXIndTexScale, GXIndTexScale);
void GXSetIndTexOrder(GXIndTexStageID, GXTexCoordID, GXTexMapID);
void GXSetNumIndStages(u8);
void GXSetTevDirect(GXTevStageID);
void __GXUpdateBPMask();
void __GXSetIndirectMask(u32);
void __GXFlushTextureState();

#ifdef __cplusplus
}
#endif
#endif

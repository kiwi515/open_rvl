#ifndef RVL_SDK_GX_LIGHT_H
#define RVL_SDK_GX_LIGHT_H
#include <revolution/GX/GXInternal.h>
#include <revolution/GX/GXTypes.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

GX_DECL_PUBLIC_STRUCT(GXLightObj, 64);

void GXInitLightAttn(GXLightObj* light, f32 aa, f32 ab, f32 ac, f32 ka, f32 kb,
                     f32 kc);
void GXInitLightAttnA(GXLightObj* light, f32 a, f32 b, f32 c);
void GXInitLightAttnK(GXLightObj* light, f32 a, f32 b, f32 c);
void GXInitLightSpot(GXLightObj* light, f32 angle, GXSpotFn fn);
void GXInitLightDistAttn(GXLightObj* light, f32 f1, f32 f2, GXDistAttnFn fn);
void GXInitLightPos(GXLightObj* light, f32 x, f32 y, f32 z);
void GXGetLightPos(const GXLightObj* light, f32* x, f32* y, f32* z);
void GXInitLightDir(GXLightObj* light, f32 x, f32 y, f32 z);
void GXGetLightDir(const GXLightObj* light, f32* x, f32* y, f32* z);
void GXInitSpecularDir(GXLightObj* light, f32 x, f32 y, f32 z);
void GXInitLightColor(GXLightObj* light, GXColor color);
void GXLoadLightObjImm(const GXLightObj* light, GXLightID id);
void GXLoadLightObjIndx(u32 index, GXLightID id);
void GXSetChanAmbColor(GXChannelID chan, GXColor color);
void GXSetChanMatColor(GXChannelID chan, GXColor color);
void GXSetNumChans(u8 num);
void GXSetChanCtrl(GXChannelID chan, GXBool8 r4, GXColorSrc src0,
                   GXColorSrc src1, GXLightID light, GXDiffuseFn diffFn,
                   GXAttnFn attnFn);

#ifdef __cplusplus
}
#endif
#endif

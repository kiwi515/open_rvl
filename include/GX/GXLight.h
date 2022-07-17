#ifndef RVL_SDK_GX_LIGHT_H
#define RVL_SDK_GX_LIGHT_H
#include <GX.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum _GXAttnFn {
    GX_ATTN_FN_0,
    GX_ATTN_FN_1,
    GX_ATTN_FN_2,
} GXAttnFn;

typedef enum _GXColorSrc {
    GX_COLOR_SRC_0,
} GXColorSrc;

typedef enum _GXChannelID {
    GX_CHAN_RGB_0,
    GX_CHAN_RGB_1,
    GX_CHAN_ALPHA_0,
    GX_CHAN_ALPHA_1,
    GX_CHAN_RGBA_0,
    GX_CHAN_RGBA_1
} GXChannelID;

typedef enum _GXDiffuseFn {
    GX_DIFFUSE_FN_0,
} GXDiffuseFn;

typedef enum _GXDistAttnFn {
    GX_DIST_ATTN_FN_0,
    GX_DIST_ATTN_FN_1,
    GX_DIST_ATTN_FN_2,
    GX_DIST_ATTN_FN_3
} GXDistAttnFn;

typedef enum _GXLightID {
    GX_LIGHT_ID_0,
} GXLightID;

typedef enum _GXSpotFn {
    GX_SPOT_FN_0,
    GX_SPOT_FN_1,
    GX_SPOT_FN_2,
    GX_SPOT_FN_3,
    GX_SPOT_FN_4,
    GX_SPOT_FN_5,
    GX_SPOT_FN_6
} GXSpotFn;

typedef struct _GXLightObj {
    char UNK_0x0[0xC];
    GXColor color; // at 0xC
    f32 aa;        // at 0x10
    f32 ab;        // at 0x14
    f32 ac;        // at 0x18
    f32 ka;        // at 0x1C
    f32 kb;        // at 0x20
    f32 kc;        // at 0x24
    f32 posX;      // at 0x28
    f32 posY;      // at 0x2C
    f32 posZ;      // at 0x30
    f32 dirX;      // at 0x34
    f32 dirY;      // at 0x38
    f32 dirZ;      // at 0x3C
} GXLightObj;

void GXInitLightAttn(GXLightObj*, f32, f32, f32, f32, f32, f32);
void GXInitLightAttnA(GXLightObj*, f32, f32, f32);
void GXInitLightAttnK(GXLightObj*, f32, f32, f32);
void GXInitLightSpot(GXLightObj*, f32, GXSpotFn);
void GXInitLightDistAttn(GXLightObj*, f32, f32, GXDistAttnFn);
void GXInitLightPos(GXLightObj*, f32, f32, f32);
void GXGetLightPos(const GXLightObj*, f32*, f32*, f32*);
void GXInitLightDir(GXLightObj*, f32, f32, f32);
void GXGetLightDir(const GXLightObj*, f32*, f32*, f32*);
void GXInitSpecularDir(GXLightObj*, f32, f32, f32);
void GXInitLightColor(GXLightObj*, GXColor);
void GXLoadLightObjImm(GXLightObj*, u32);
void GXLoadLightObjIndx(u32, u32);
void GXSetChanAmbColor(GXChannelID, GXColor);
void GXSetChanMatColor(GXChannelID, GXColor);
void GXSetNumChans(u8);
void GXSetChanCtrl(GXChannelID, u8, GXColorSrc, GXColorSrc, u32, GXDiffuseFn,
                   GXAttnFn);

#ifdef __cplusplus
}
#endif
#endif

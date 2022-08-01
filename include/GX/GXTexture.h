#ifndef RVL_SDK_GX_TEXTURE_H
#define RVL_SDK_GX_TEXTURE_H
#include "GX.h"
#include "GXTev.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum _GXTexCoord {
    GX_TEX_COORD_0,
} GXTexCoord;

typedef enum _GXTexFilter { GX_LINEAR = 1, GX_LIN_MIP_LIN = 5 } GXTexFilter;

typedef enum _GXTexFmt {
    GX_TF_I4,
    GX_TF_I8,
    GX_TF_IA4,
    GX_TF_3,
    GX_TF_4,
    GX_TF_5,
    GX_TF_RGBA8,
    GX_TF_7,

    // CI formats
    GX_TF_8,
    GX_TF_9,
    GX_TF_10,

    GX_TF_11,
    GX_TF_12,
    GX_TF_13,
    GX_TF_14,
    GX_TF_15,
    GX_TF_16,
    GX_TF_17,
    GX_TF_18,
    GX_TF_19,
    GX_TF_20,
    GX_TF_21,
    GX_TF_22
} GXTexFmt;

typedef enum _GXTexMapID {
    GX_TEXMAP_0,
    GX_TEXMAP_1,
    GX_TEXMAP_2,
    GX_TEXMAP_3,
    GX_TEXMAP_4,
    GX_TEXMAP_5,
    GX_TEXMAP_6,
    GX_TEXMAP_7,

    GX_MAX_TEXMAP
} GXTexMapID;

typedef enum _GXTexWrapMode {
    GX_TEXWRAPMODE_0,
    GX_TEXWRAPMODE_1,
    GX_TEXWRAPMODE_2,
    GX_MAX_TEXWRAPMODE
} GXTexWrapMode;

typedef enum _GXTlutFmt { GX_TLUT_FMT_0 } GXTlutFmt;

void __GXSetSUTexRegs(void);

#ifdef __cplusplus
}
#endif
#endif

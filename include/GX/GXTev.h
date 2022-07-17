#ifndef RVL_SDK_GX_TEV_H
#define RVL_SDK_GX_TEV_H
#include "GX.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum _GXIndTexAlphaSel {
    GX_IND_TEX_ALPHA_SEL_0,
} GXIndTexAlphaSel;

typedef enum _GXIndTexBiasSel {
    GX_IND_TEX_BIAS_SEL_0,
} GXIndTexBiasSel;

typedef enum _GXIndTexFormat {
    GX_IND_TEX_FMT_0,
} GXIndTexFormat;

typedef enum _GXIndTexMtxID {
    GX_IND_TEX_MTX_ID_0,
    GX_IND_TEX_MTX_ID_1,
    GX_IND_TEX_MTX_ID_2,
    GX_IND_TEX_MTX_ID_3,
    GX_IND_TEX_MTX_ID_4,
    GX_IND_TEX_MTX_ID_5,
    GX_IND_TEX_MTX_ID_6,
    GX_IND_TEX_MTX_ID_7,
    GX_IND_TEX_MTX_ID_8,
    GX_IND_TEX_MTX_ID_9,
    GX_IND_TEX_MTX_ID_10,
    GX_IND_TEX_MTX_ID_11,
} GXIndTexMtxID;

typedef enum _GXIndTexScale {
    GX_IND_TEX_SCALE_0,
} GXIndTexScale;

typedef enum _GXIndTexStageID {
    GX_IND_TEX_STAGE_ID_0,
    GX_IND_TEX_STAGE_ID_1,
    GX_IND_TEX_STAGE_ID_2,
    GX_IND_TEX_STAGE_ID_3
} GXIndTexStageID;

typedef enum _GXIndTexWrap {
    GX_IND_TEX_WRAP_0,
} GXIndTexWrap;

typedef enum _GXTevStageID {
    GX_TEV_STAGE_ID_0,
} GXTevStageID;

#ifdef __cplusplus
}
#endif
#endif

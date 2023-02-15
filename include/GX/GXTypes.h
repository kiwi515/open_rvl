#ifndef RVL_SDK_GX_TYPES_H
#define RVL_SDK_GX_TYPES_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Common types used throughout many GX files.
 * To avoid circular depencies, common structures and all enums have been moved
 * here. Everything is organized in alphabetical order.
 */

//! TODO: Many GX functions use 8-bit booleans.
//! As those files are decompiled, update prototypes that use u8 -> Bool8,
//! so TRUE/FALSE can be used in place of 0/1 by callers
typedef unsigned char Bool8;

/**
 * Common structs
 */

typedef struct _GXColor {
    u8 r, g, b, a;
} GXColor;

/**
 * Enums
 */

typedef enum _GXAlphaOp {
    GX_ALPHA_OP_0,
    GX_ALPHA_OP_1,
} GXAlphaOp;

typedef enum _GXAnisotropy {
    GX_ANISOTROPY_0,
} GXAnisotropy;

typedef enum _GXAttnFn {
    GX_ATTN_FN_0,
    GX_ATTN_FN_1,
    GX_ATTN_FN_2,
} GXAttnFn;

typedef enum _GXAttr {
    GX_ATTR_0,
    GX_ATTR_1,
    GX_ATTR_2,
    GX_ATTR_3,
    GX_ATTR_4,
    GX_ATTR_5,
    GX_ATTR_6,
    GX_ATTR_7,
    GX_ATTR_8,

    GX_ATTR_VTX,

    GX_ATTR_VTX_POS = GX_ATTR_VTX,
    GX_ATTR_VTX_NRM,

    GX_ATTR_VTX_CLR,
    GX_ATTR_VTX_CLR_COUNT = 2,

    GX_ATTR_VTX_TEX_COORD = GX_ATTR_VTX_CLR + GX_ATTR_VTX_CLR_COUNT,
    GX_ATTR_VTX_TEX_COORD_COUNT = 8,

    GX_ATTR_14 = 14,
    GX_ATTR_15,
    GX_ATTR_16,
    GX_ATTR_17,
    GX_ATTR_18,
    GX_ATTR_19,
    GX_ATTR_20,

    GX_ATTR_COUNT = GX_ATTR_VTX_TEX_COORD + GX_ATTR_VTX_TEX_COORD_COUNT,
    GX_ATTR_VTX_COUNT = GX_ATTR_COUNT - GX_ATTR_VTX,

    GX_ATTR_INVALID = 0xFF
} GXAttr;

typedef enum _GXBlendFactor {
    GX_BLEND_FACTOR_0,
    GX_BLEND_FACTOR_1,
    GX_BLEND_FACTOR_2,
    GX_BLEND_FACTOR_3,
    GX_BLEND_FACTOR_4,
    GX_BLEND_FACTOR_5,
} GXBlendFactor;

typedef enum _GXBlendMode {
    GX_BLEND_MODE_0,
    GX_BLEND_MODE_1,
} GXBlendMode;

typedef enum _GXChannelID {
    GX_CHAN_RGB_0,
    GX_CHAN_RGB_1,
    GX_CHAN_ALPHA_0,
    GX_CHAN_ALPHA_1,
    GX_CHAN_RGBA_0,
    GX_CHAN_RGBA_1,

    GX_CHAN_INVALID = 0xFF
} GXChannelID;

typedef enum _GXColorSrc {
    GX_COLOR_SRC_0,
} GXColorSrc;

typedef enum _GXCompare {
    GX_COMPARE_0,
    GX_COMPARE_1,
    GX_COMPARE_2,
    GX_COMPARE_3,
    GX_COMPARE_4,
} GXCompare;

typedef enum _GXCullMode {
    GX_CULL_MODE_0,
    GX_CULL_MODE_1,
    GX_CULL_MODE_2,
} GXCullMode;

typedef enum _GXDiffuseFn {
    GX_DIFFUSE_FN_0,
} GXDiffuseFn;

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

typedef enum _GXDistAttnFn {
    GX_DIST_ATTN_FN_0,
    GX_DIST_ATTN_FN_1,
    GX_DIST_ATTN_FN_2,
    GX_DIST_ATTN_FN_3
} GXDistAttnFn;

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

typedef enum _GXLightID {
    GX_LIGHT_ID_0,
} GXLightID;

typedef enum _GXLogicOp {
    GX_LOGIC_OP_0,
    GX_LOGIC_OP_1,
    GX_LOGIC_OP_2,
    GX_LOGIC_OP_3,
} GXLogicOp;

typedef enum _GXPrimitive {
    GX_PRIMITIVE_0,
} GXPrimitive;

typedef enum _GXSpotFn {
    GX_SPOT_FN_0,
    GX_SPOT_FN_1,
    GX_SPOT_FN_2,
    GX_SPOT_FN_3,
    GX_SPOT_FN_4,
    GX_SPOT_FN_5,
    GX_SPOT_FN_6
} GXSpotFn;

typedef enum _GXTevAlphaArg {
    GX_TEV_ALPHA_ARG_0,
    GX_TEV_ALPHA_ARG_1,
    GX_TEV_ALPHA_ARG_2,
    GX_TEV_ALPHA_ARG_3,
    GX_TEV_ALPHA_ARG_4,
    GX_TEV_ALPHA_ARG_5,
    GX_TEV_ALPHA_ARG_6,
    GX_TEV_ALPHA_ARG_7
} GXTevAlphaArg;

typedef enum _GXTevBias {
    GX_TEV_BIAS_0,
} GXTevBias;

typedef enum _GXTevColorArg {
    GX_TEV_COLOR_ARG_0,
    GX_TEV_COLOR_ARG_1,
    GX_TEV_COLOR_ARG_2,
    GX_TEV_COLOR_ARG_3,
    GX_TEV_COLOR_ARG_4,
    GX_TEV_COLOR_ARG_5,
    GX_TEV_COLOR_ARG_6,
    GX_TEV_COLOR_ARG_7,
    GX_TEV_COLOR_ARG_8,
    GX_TEV_COLOR_ARG_9,
    GX_TEV_COLOR_ARG_10,
    GX_TEV_COLOR_ARG_11,
    GX_TEV_COLOR_ARG_12,
    GX_TEV_COLOR_ARG_13,
    GX_TEV_COLOR_ARG_14,
    GX_TEV_COLOR_ARG_15
} GXTevColorArg;

typedef enum _GXTevColorChan {
    GX_TEV_COLOR_CHAN_0,
    GX_TEV_COLOR_CHAN_1,
    GX_TEV_COLOR_CHAN_2,
    GX_TEV_COLOR_CHAN_3
} GXTevColorChan;

typedef enum _GXTevOp {
    GX_TEV_OP_0,
} GXTevOp;

typedef enum _GXTevRegID {
    GX_TEVPREV,
    // . . .
    GX_TEVREG_2 = 3
} GXTevRegID;

typedef enum _GXTevScale {
    GX_TEV_SCALE_0,
    GX_TEV_SCALE_1,
    GX_TEV_SCALE_2,
    GX_TEV_SCALE_3,
} GXTevScale;

typedef enum _GXTevStageID {
    GX_TEV_STAGE_ID_0,
    GX_TEV_STAGE_ID_1,
} GXTevStageID;

typedef enum _GXTevSwapSel {
    GX_TEV_SWAP_SEL_0,
    GX_TEV_SWAP_SEL_1,
    GX_TEV_SWAP_SEL_2,
    GX_TEV_SWAP_SEL_3
} GXTevSwapSel;

typedef enum _GXTevKAlphaSel {
    GX_TEV_KALPHA_SEL_0,
} GXTevKAlphaSel;

typedef enum _GXTevKColorID {
    GX_KCOLOR0,
    GX_KCOLOR1,
    GX_KCOLOR2,
    GX_KCOLOR3,
} GXTevKColorID;

typedef enum _GXTevKColorSel {
    GX_TEV_KCOLOR_SEL_0,
} GXTevKColorSel;

typedef enum _GXTexCoordID {
    GX_TEXCOORD0,

    GX_TEXCOORD_INVALID = 0xFF
} GXTexCoordID;

typedef enum _GXTexFilter {
    GX_NEAR,
    GX_LINEAR,
    GX_NEAR_MIP_NEAR,
    GX_LIN_MIP_NEAR,
    GX_NEAR_MIP_LIN,
    GX_LIN_MIP_LIN,
} GXTexFilter;

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

typedef enum _GXTexGenSrc {
    GX_TEX_GEN_SRC_0,
    GX_TEX_GEN_SRC_1,
    GX_TEX_GEN_SRC_2,
    GX_TEX_GEN_SRC_3,
    GX_TEX_GEN_SRC_4,
} GXTexGenSrc;

typedef enum _GXTexGenType {
    GX_TEX_GEN_TYPE_0,
    GX_TEX_GEN_TYPE_1,
} GXTexGenType;

typedef enum _GXTexMapID {
    GX_TEXMAP_0,
    GX_TEXMAP_1,
    GX_TEXMAP_2,
    GX_TEXMAP_3,
    GX_TEXMAP_4,
    GX_TEXMAP_5,
    GX_TEXMAP_6,
    GX_TEXMAP_7,

    GX_MAX_TEXMAP,
    GX_TEXMAP_INVALID = 0xFF
} GXTexMapID;

typedef enum _GXTexWrapMode {
    GX_TEXWRAPMODE_0,
    GX_TEXWRAPMODE_1,
    GX_TEXWRAPMODE_2,
    GX_MAX_TEXWRAPMODE
} GXTexWrapMode;

typedef enum _GXTlutFmt { GX_TLUT_FMT_0 } GXTlutFmt;

#ifdef __cplusplus
}
#endif
#endif

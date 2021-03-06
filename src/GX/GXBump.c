#include "GXBump.h"
#include "GXInit.h"

void GXSetTevIndirect(GXTevStageID tevStage, GXIndTexStageID texStage,
                      GXIndTexFormat texFmt, GXIndTexBiasSel biasSel,
                      GXIndTexMtxID mtxId, GXIndTexWrap wrapS,
                      GXIndTexWrap wrapT, u8 addPrev, u8 utcLod,
                      GXIndTexAlphaSel alphaSel) {
    u32 field = 0;
    const u32 stage = tevStage + 0x10;

    GX_BITFIELD_SET(field, 30, 2, texStage);
    GX_BITFIELD_SET(field, 28, 2, texFmt);
    GX_BITFIELD_SET(field, 25, 3, biasSel);
    GX_BITFIELD_SET(field, 23, 2, alphaSel);
    GX_BITFIELD_SET(field, 19, 4, mtxId);
    GX_BITFIELD_SET(field, 16, 3, wrapS);
    GX_BITFIELD_SET(field, 13, 3, wrapT);
    GX_BITFIELD_SET(field, 12, 1, utcLod);
    GX_BITFIELD_SET(field, 11, 1, addPrev);
    GX_BITFIELD_SET(field, 0, 8, stage);

    WGPIPE.c = 0x61;
    WGPIPE.i = field;
    __GXData->SHORTS_0x0[1] = 0;
}

void GXSetIndTexMtx(GXIndTexMtxID id, const f32 mtx[6], s8 r5) {
    u32 val;
    u32 field;

    switch (id) {
    case GX_IND_TEX_MTX_ID_1:
    case GX_IND_TEX_MTX_ID_2:
    case GX_IND_TEX_MTX_ID_3:
        val = id - 1;
        break;
    case GX_IND_TEX_MTX_ID_5:
    case GX_IND_TEX_MTX_ID_6:
    case GX_IND_TEX_MTX_ID_7:
        val = id - 5;
        break;
    case GX_IND_TEX_MTX_ID_9:
    case GX_IND_TEX_MTX_ID_10:
    case GX_IND_TEX_MTX_ID_11:
        val = id - 9;
        break;
    case GX_IND_TEX_MTX_ID_4:
    case GX_IND_TEX_MTX_ID_8:
    default:
        val = 0;
    }

    r5 += 0x11;

    field = 0;
    GX_BITFIELD_SET(field, 21, 11, 1024.0f * mtx[0]);
    GX_BITFIELD_SET(field, 10, 11, 1024.0f * mtx[3]);
    GX_BITFIELD_SET(field, 8, 2, r5);
    GX_BITFIELD_SET(field, 0, 8, (val * 4) - val + 6);
    WGPIPE.c = 0x61;
    WGPIPE.i = field;

    field = 0;
    GX_BITFIELD_SET(field, 21, 11, 1024.0f * mtx[1]);
    GX_BITFIELD_SET(field, 10, 11, 1024.0f * mtx[4]);
    field = __rlwimi(field, r5, 20, 8, 9);
    GX_BITFIELD_SET(field, 0, 8, (val * 4) - val + 7);
    WGPIPE.c = 0x61;
    WGPIPE.i = field;

    field = 0;
    GX_BITFIELD_SET(field, 21, 11, 1024.0f * mtx[2]);
    GX_BITFIELD_SET(field, 10, 11, 1024.0f * mtx[5]);
    field = __rlwimi(field, r5, 18, 8, 9);
    GX_BITFIELD_SET(field, 0, 8, (val * 4) - val + 8);
    WGPIPE.c = 0x61;
    WGPIPE.i = field;

    __GXData->SHORTS_0x0[1] = 0;
}

void GXSetIndTexCoordScale(GXIndTexStageID stage, GXIndTexScale scaleS,
                           GXIndTexScale scaleT) {
    switch (stage) {
    case GX_IND_TEX_STAGE_ID_0:
        GX_BITFIELD_SET(__GXData->WORD_0x178, 28, 4, scaleS);
        GX_BITFIELD_SET(__GXData->WORD_0x178, 24, 4, scaleT);
        GX_BITFIELD_SET(__GXData->WORD_0x178, 0, 8, 0x25);
        WGPIPE.c = 0x61;
        WGPIPE.i = __GXData->WORD_0x178;
        break;
    case GX_IND_TEX_STAGE_ID_1:
        GX_BITFIELD_SET(__GXData->WORD_0x178, 20, 4, scaleS);
        GX_BITFIELD_SET(__GXData->WORD_0x178, 16, 4, scaleT);
        GX_BITFIELD_SET(__GXData->WORD_0x178, 0, 8, 0x25);
        WGPIPE.c = 0x61;
        WGPIPE.i = __GXData->WORD_0x178;
        break;
    case GX_IND_TEX_STAGE_ID_2:
        GX_BITFIELD_SET(__GXData->WORD_0x17C, 28, 4, scaleS);
        GX_BITFIELD_SET(__GXData->WORD_0x17C, 24, 4, scaleT);
        GX_BITFIELD_SET(__GXData->WORD_0x17C, 0, 8, 0x26);
        WGPIPE.c = 0x61;
        WGPIPE.i = __GXData->WORD_0x17C;
        break;
    case GX_IND_TEX_STAGE_ID_3:
        GX_BITFIELD_SET(__GXData->WORD_0x17C, 20, 4, scaleS);
        GX_BITFIELD_SET(__GXData->WORD_0x17C, 16, 4, scaleT);
        GX_BITFIELD_SET(__GXData->WORD_0x17C, 0, 8, 0x26);
        WGPIPE.c = 0x61;
        WGPIPE.i = __GXData->WORD_0x17C;
        break;
    }

    __GXData->SHORTS_0x0[1] = 0;
}

void GXSetIndTexOrder(GXIndTexStageID stage, GXTexCoord coord, GXTexMapID map) {
    if (map == 0xFF) {
        map = GX_TEXMAP_0;
    }

    if (coord == 0xFF) {
        coord = GX_TEX_COORD_0;
    }

    switch (stage) {
    case GX_IND_TEX_STAGE_ID_0:
        GX_BITFIELD_SET(__GXData->WORD_0x170, 29, 3, map);
        GX_BITFIELD_SET(__GXData->WORD_0x170, 26, 3, coord);
        break;
    case GX_IND_TEX_STAGE_ID_1:
        GX_BITFIELD_SET(__GXData->WORD_0x170, 23, 3, map);
        GX_BITFIELD_SET(__GXData->WORD_0x170, 20, 3, coord);
        break;
    case GX_IND_TEX_STAGE_ID_2:
        GX_BITFIELD_SET(__GXData->WORD_0x170, 17, 3, map);
        GX_BITFIELD_SET(__GXData->WORD_0x170, 14, 3, coord);
        break;
    case GX_IND_TEX_STAGE_ID_3:
        GX_BITFIELD_SET(__GXData->WORD_0x170, 11, 3, map);
        GX_BITFIELD_SET(__GXData->WORD_0x170, 8, 3, coord);
        break;
    }

    WGPIPE.c = 0x61;
    WGPIPE.i = __GXData->WORD_0x170;
    __GXData->dirtyFlags |= 0x3;
    __GXData->SHORTS_0x0[1] = 0;
}

void GXSetNumIndStages(u8 num) {
    GX_BITFIELD_SET(__GXData->WORD_0x254, 13, 3, num);
    __GXData->dirtyFlags |= 0x6;
}

void GXSetTevDirect(GXTevStageID stage) {
    GXSetTevIndirect(stage, GX_IND_TEX_STAGE_ID_0, GX_IND_TEX_FMT_0,
                     GX_IND_TEX_BIAS_SEL_0, GX_IND_TEX_MTX_ID_0,
                     GX_IND_TEX_WRAP_0, GX_IND_TEX_WRAP_0, FALSE, FALSE,
                     GX_IND_TEX_ALPHA_SEL_0);
}

void __GXUpdateBPMask(void) {}

void __GXSetIndirectMask(u32 mask) {
    GX_BITFIELD_SET(__GXData->WORD_0x174, 24, 8, mask);
    WGPIPE.c = 0x61;
    WGPIPE.i = __GXData->WORD_0x174;
    __GXData->SHORTS_0x0[1] = 0;
}

void __GXFlushTextureState(void) {
    WGPIPE.c = 0x61;
    WGPIPE.i = __GXData->WORD_0x174;
    __GXData->SHORTS_0x0[1] = 0;
}

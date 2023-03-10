#include <revolution/GX.h>

void GXSetTevIndirect(GXTevStageID tevStage, GXIndTexStageID texStage,
                      GXIndTexFormat texFmt, GXIndTexBiasSel biasSel,
                      GXIndTexMtxID mtxId, GXIndTexWrap wrapS,
                      GXIndTexWrap wrapT, GXBool8 addPrev, GXBool8 utcLod,
                      GXIndTexAlphaSel alphaSel) {
    u32 field = 0;
    const u32 stage = tevStage + GX_MAX_TEVSTAGE;

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

    WGPIPE.c = GX_FIFO_ACCESS_BP;
    WGPIPE.i = field;
    __GXData->SHORTS_0x0[1] = 0;
}

void GXSetIndTexMtx(GXIndTexMtxID id, const f32 mtx[6], s8 scaleExp) {
    u32 val;
    u32 field;

    switch (id) {
    case GX_ITM_0:
    case GX_ITM_1:
    case GX_ITM_2:
        val = id - 1;
        break;
    case GX_ITM_S0:
    case GX_ITM_S1:
    case GX_ITM_S2:
        val = id - 5;
        break;
    case GX_ITM_T0:
    case GX_ITM_T1:
    case GX_ITM_T2:
        val = id - 9;
        break;
    case 4:
    case 8:
    default:
        val = 0;
    }

    scaleExp += 0x11;

    field = 0;
    GX_BITFIELD_SET(field, 21, 11, 1024.0f * mtx[0]);
    GX_BITFIELD_SET(field, 10, 11, 1024.0f * mtx[3]);
    GX_BITFIELD_SET(field, 8, 2, scaleExp);
    GX_BITFIELD_SET(field, 0, 8, (val * 4) - val + 6);
    WGPIPE.c = GX_FIFO_ACCESS_BP;
    WGPIPE.i = field;

    field = 0;
    GX_BITFIELD_SET(field, 21, 11, 1024.0f * mtx[1]);
    GX_BITFIELD_SET(field, 10, 11, 1024.0f * mtx[4]);
    field = __rlwimi(field, scaleExp, 20, 8, 9);
    GX_BITFIELD_SET(field, 0, 8, (val * 4) - val + 7);
    WGPIPE.c = GX_FIFO_ACCESS_BP;
    WGPIPE.i = field;

    field = 0;
    GX_BITFIELD_SET(field, 21, 11, 1024.0f * mtx[2]);
    GX_BITFIELD_SET(field, 10, 11, 1024.0f * mtx[5]);
    field = __rlwimi(field, scaleExp, 18, 8, 9);
    GX_BITFIELD_SET(field, 0, 8, (val * 4) - val + 8);
    WGPIPE.c = GX_FIFO_ACCESS_BP;
    WGPIPE.i = field;

    __GXData->SHORTS_0x0[1] = 0;
}

void GXSetIndTexCoordScale(GXIndTexStageID stage, GXIndTexScale scaleS,
                           GXIndTexScale scaleT) {
    switch (stage) {
    case GX_INDTEXSTAGE0:
        GX_BITFIELD_SET(__GXData->WORD_0x178, 28, 4, scaleS);
        GX_BITFIELD_SET(__GXData->WORD_0x178, 24, 4, scaleT);
        GX_BITFIELD_SET(__GXData->WORD_0x178, 0, 8, 0x25);
        WGPIPE.c = GX_FIFO_ACCESS_BP;
        WGPIPE.i = __GXData->WORD_0x178;
        break;
    case GX_INDTEXSTAGE1:
        GX_BITFIELD_SET(__GXData->WORD_0x178, 20, 4, scaleS);
        GX_BITFIELD_SET(__GXData->WORD_0x178, 16, 4, scaleT);
        GX_BITFIELD_SET(__GXData->WORD_0x178, 0, 8, 0x25);
        WGPIPE.c = GX_FIFO_ACCESS_BP;
        WGPIPE.i = __GXData->WORD_0x178;
        break;
    case GX_INDTEXSTAGE2:
        GX_BITFIELD_SET(__GXData->WORD_0x17C, 28, 4, scaleS);
        GX_BITFIELD_SET(__GXData->WORD_0x17C, 24, 4, scaleT);
        GX_BITFIELD_SET(__GXData->WORD_0x17C, 0, 8, 0x26);
        WGPIPE.c = GX_FIFO_ACCESS_BP;
        WGPIPE.i = __GXData->WORD_0x17C;
        break;
    case GX_INDTEXSTAGE3:
        GX_BITFIELD_SET(__GXData->WORD_0x17C, 20, 4, scaleS);
        GX_BITFIELD_SET(__GXData->WORD_0x17C, 16, 4, scaleT);
        GX_BITFIELD_SET(__GXData->WORD_0x17C, 0, 8, 0x26);
        WGPIPE.c = GX_FIFO_ACCESS_BP;
        WGPIPE.i = __GXData->WORD_0x17C;
        break;
    }

    __GXData->SHORTS_0x0[1] = 0;
}

void GXSetIndTexOrder(GXIndTexStageID stage, GXTexCoordID coord,
                      GXTexMapID map) {
    if (map == GX_TEXMAP_NULL) {
        map = GX_TEXMAP0;
    }

    if (coord == GX_TEXCOORD_NULL) {
        coord = GX_TEXCOORD0;
    }

    switch (stage) {
    case GX_INDTEXSTAGE0:
        GX_BITFIELD_SET(__GXData->WORD_0x170, 29, 3, map);
        GX_BITFIELD_SET(__GXData->WORD_0x170, 26, 3, coord);
        break;
    case GX_INDTEXSTAGE1:
        GX_BITFIELD_SET(__GXData->WORD_0x170, 23, 3, map);
        GX_BITFIELD_SET(__GXData->WORD_0x170, 20, 3, coord);
        break;
    case GX_INDTEXSTAGE2:
        GX_BITFIELD_SET(__GXData->WORD_0x170, 17, 3, map);
        GX_BITFIELD_SET(__GXData->WORD_0x170, 14, 3, coord);
        break;
    case GX_INDTEXSTAGE3:
        GX_BITFIELD_SET(__GXData->WORD_0x170, 11, 3, map);
        GX_BITFIELD_SET(__GXData->WORD_0x170, 8, 3, coord);
        break;
    }

    WGPIPE.c = GX_FIFO_ACCESS_BP;
    WGPIPE.i = __GXData->WORD_0x170;
    __GXData->dirtyFlags |= 0x3;
    __GXData->SHORTS_0x0[1] = 0;
}

void GXSetNumIndStages(u8 num) {
    GX_BITFIELD_SET(__GXData->WORD_0x254, 13, 3, num);
    __GXData->dirtyFlags |= 0x6;
}

void GXSetTevDirect(GXTevStageID stage) {
    GXSetTevIndirect(stage, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_NONE, GX_ITM_OFF,
                     GX_ITW_OFF, GX_ITW_OFF, FALSE, FALSE, GX_ITBA_OFF);
}

void __GXUpdateBPMask(void) {}

void __GXSetIndirectMask(u32 mask) {
    GX_BITFIELD_SET(__GXData->WORD_0x174, 24, 8, mask);
    WGPIPE.c = GX_FIFO_ACCESS_BP;
    WGPIPE.i = __GXData->WORD_0x174;
    __GXData->SHORTS_0x0[1] = 0;
}

void __GXFlushTextureState(void) {
    WGPIPE.c = GX_FIFO_ACCESS_BP;
    WGPIPE.i = __GXData->WORD_0x174;
    __GXData->SHORTS_0x0[1] = 0;
}

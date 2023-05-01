#include <revolution/GX.h>

void GXSetTevIndirect(GXTevStageID tevStage, GXIndTexStageID texStage,
                      GXIndTexFormat texFmt, GXIndTexBiasSel biasSel,
                      GXIndTexMtxID mtxId, GXIndTexWrap wrapS,
                      GXIndTexWrap wrapT, GXBool8 addPrev, GXBool8 utcLod,
                      GXIndTexAlphaSel alphaSel) {
    u32 cmd = 0;
    const u32 stage = tevStage + GX_MAX_TEVSTAGE;

    cmd = GX_BITSET(cmd, 30, 2, texStage);
    cmd = GX_BITSET(cmd, 28, 2, texFmt);
    cmd = GX_BITSET(cmd, 25, 3, biasSel);
    cmd = GX_BITSET(cmd, 23, 2, alphaSel);
    cmd = GX_BITSET(cmd, 19, 4, mtxId);
    cmd = GX_BITSET(cmd, 16, 3, wrapS);
    cmd = GX_BITSET(cmd, 13, 3, wrapT);
    cmd = GX_BITSET(cmd, 12, 1, utcLod);
    cmd = GX_BITSET(cmd, 11, 1, addPrev);
    cmd = GX_BITSET(cmd, 0, 8, stage);

    GX_WRITE_BP_CMD(cmd);
    __GXData->SHORTS_0x0[1] = 0;
}

void GXSetIndTexMtx(GXIndTexMtxID id, const f32 mtx[6], s8 scaleExp) {
    u32 relIndex;
    u32 cmd;

    switch (id) {
    case GX_ITM_0:
    case GX_ITM_1:
    case GX_ITM_2:
        relIndex = id - GX_ITM_0;
        break;
    case GX_ITM_S0:
    case GX_ITM_S1:
    case GX_ITM_S2:
        relIndex = id - GX_ITM_S0;
        break;
    case GX_ITM_T0:
    case GX_ITM_T1:
    case GX_ITM_T2:
        relIndex = id - GX_ITM_T0;
        break;
    case 4:
    case 8:
    default:
        relIndex = 0;
    }

    scaleExp += 0x11;

    cmd = 0;
    cmd = GX_BITSET(cmd, 21, 11, 1024.0f * mtx[0]);
    cmd = GX_BITSET(cmd, 10, 11, 1024.0f * mtx[3]);
    cmd = GX_BITSET(cmd, 8, 2, scaleExp);
    cmd = GX_BITSET(cmd, 0, 8, (relIndex * 4) - relIndex + GX_FIFO_BP_IND_MTXA);
    GX_WRITE_BP_CMD(cmd);

    cmd = 0;
    cmd = GX_BITSET(cmd, 21, 11, 1024.0f * mtx[1]);
    cmd = GX_BITSET(cmd, 10, 11, 1024.0f * mtx[4]);
    cmd = __rlwimi(cmd, scaleExp, 20, 8, 9);
    cmd = GX_BITSET(cmd, 0, 8, (relIndex * 4) - relIndex + GX_FIFO_BP_IND_MTXB);
    GX_WRITE_BP_CMD(cmd);

    cmd = 0;
    cmd = GX_BITSET(cmd, 21, 11, 1024.0f * mtx[2]);
    cmd = GX_BITSET(cmd, 10, 11, 1024.0f * mtx[5]);
    cmd = __rlwimi(cmd, scaleExp, 18, 8, 9);
    cmd = GX_BITSET(cmd, 0, 8, (relIndex * 4) - relIndex + GX_FIFO_BP_IND_MTXC);
    GX_WRITE_BP_CMD(cmd);

    __GXData->SHORTS_0x0[1] = 0;
}

void GXSetIndTexCoordScale(GXIndTexStageID stage, GXIndTexScale scaleS,
                           GXIndTexScale scaleT) {
    switch (stage) {
    case GX_INDTEXSTAGE0:
        __GXData->WORD_0x178 = GX_BITSET(__GXData->WORD_0x178, 28, 4, scaleS);
        __GXData->WORD_0x178 = GX_BITSET(__GXData->WORD_0x178, 24, 4, scaleT);
        __GXData->WORD_0x178 =
            GX_BITSET(__GXData->WORD_0x178, 0, 8, GX_FIFO_BP_RAS1_SS0);
        GX_WRITE_BP_CMD(__GXData->WORD_0x178);
        break;
    case GX_INDTEXSTAGE1:
        __GXData->WORD_0x178 = GX_BITSET(__GXData->WORD_0x178, 20, 4, scaleS);
        __GXData->WORD_0x178 = GX_BITSET(__GXData->WORD_0x178, 16, 4, scaleT);
        __GXData->WORD_0x178 =
            GX_BITSET(__GXData->WORD_0x178, 0, 8, GX_FIFO_BP_RAS1_SS0);
        GX_WRITE_BP_CMD(__GXData->WORD_0x178);
        break;
    case GX_INDTEXSTAGE2:
        __GXData->WORD_0x17C = GX_BITSET(__GXData->WORD_0x17C, 28, 4, scaleS);
        __GXData->WORD_0x17C = GX_BITSET(__GXData->WORD_0x17C, 24, 4, scaleT);
        __GXData->WORD_0x17C =
            GX_BITSET(__GXData->WORD_0x17C, 0, 8, GX_FIFO_BP_RAS1_SS1);
        GX_WRITE_BP_CMD(__GXData->WORD_0x17C);
        break;
    case GX_INDTEXSTAGE3:
        __GXData->WORD_0x17C = GX_BITSET(__GXData->WORD_0x17C, 20, 4, scaleS);
        __GXData->WORD_0x17C = GX_BITSET(__GXData->WORD_0x17C, 16, 4, scaleT);
        __GXData->WORD_0x17C =
            GX_BITSET(__GXData->WORD_0x17C, 0, 8, GX_FIFO_BP_RAS1_SS1);
        GX_WRITE_BP_CMD(__GXData->WORD_0x17C);
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
        __GXData->WORD_0x170 = GX_BITSET(__GXData->WORD_0x170, 29, 3, map);
        __GXData->WORD_0x170 = GX_BITSET(__GXData->WORD_0x170, 26, 3, coord);
        break;
    case GX_INDTEXSTAGE1:
        __GXData->WORD_0x170 = GX_BITSET(__GXData->WORD_0x170, 23, 3, map);
        __GXData->WORD_0x170 = GX_BITSET(__GXData->WORD_0x170, 20, 3, coord);
        break;
    case GX_INDTEXSTAGE2:
        __GXData->WORD_0x170 = GX_BITSET(__GXData->WORD_0x170, 17, 3, map);
        __GXData->WORD_0x170 = GX_BITSET(__GXData->WORD_0x170, 14, 3, coord);
        break;
    case GX_INDTEXSTAGE3:
        __GXData->WORD_0x170 = GX_BITSET(__GXData->WORD_0x170, 11, 3, map);
        __GXData->WORD_0x170 = GX_BITSET(__GXData->WORD_0x170, 8, 3, coord);
        break;
    }

    GX_WRITE_BP_CMD(__GXData->WORD_0x170);
    __GXData->dirtyFlags |= 0x3;
    __GXData->SHORTS_0x0[1] = 0;
}

void GXSetNumIndStages(u8 num) {
    __GXData->WORD_0x254 = GX_BITSET(__GXData->WORD_0x254, 13, 3, num);
    __GXData->dirtyFlags |= GX_DIRTY_BP_MASK | GX_DIRTY_GEN_MODE;
}

void GXSetTevDirect(GXTevStageID stage) {
    GXSetTevIndirect(stage, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_NONE, GX_ITM_OFF,
                     GX_ITW_OFF, GX_ITW_OFF, FALSE, FALSE, GX_ITBA_OFF);
}

void __GXUpdateBPMask(void) {}

void __GXSetIndirectMask(u32 mask) {
    __GXData->WORD_0x174 = GX_BITSET(__GXData->WORD_0x174, 24, 8, mask);
    GX_WRITE_BP_CMD(__GXData->WORD_0x174);
    __GXData->SHORTS_0x0[1] = 0;
}

void __GXFlushTextureState(void) {
    GX_WRITE_BP_CMD(__GXData->WORD_0x174);
    __GXData->SHORTS_0x0[1] = 0;
}

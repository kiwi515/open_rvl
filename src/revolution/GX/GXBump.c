#include <revolution/GX.h>

void GXSetTevIndirect(GXTevStageID tevStage, GXIndTexStageID texStage,
                      GXIndTexFormat texFmt, GXIndTexBiasSel biasSel,
                      GXIndTexMtxID mtxId, GXIndTexWrap wrapS,
                      GXIndTexWrap wrapT, GXBool addPrev, GXBool utcLod,
                      GXIndTexAlphaSel alphaSel) {
    u32 opcode = tevStage + GX_BP_REG_INDTEVSTAGE0;

    u32 cmd = 0;
    cmd = GX_BITSET(cmd, 30, 2, texStage);
    cmd = GX_BITSET(cmd, 28, 2, texFmt);
    cmd = GX_BITSET(cmd, 25, 3, biasSel);
    cmd = GX_BITSET(cmd, 23, 2, alphaSel);
    cmd = GX_BITSET(cmd, 19, 4, mtxId);
    cmd = GX_BITSET(cmd, 16, 3, wrapS);
    cmd = GX_BITSET(cmd, 13, 3, wrapT);
    cmd = GX_BITSET(cmd, 12, 1, utcLod);
    cmd = GX_BITSET(cmd, 11, 1, addPrev);
    GX_BP_SET_OPCODE(cmd, opcode);

    GX_LOAD_BP_REG(cmd);
    gxdt->xfWritten = FALSE;
}

void GXSetIndTexMtx(GXIndTexMtxID id, const f32 offset[2][3], s8 scaleExp) {
    u32 index;
    u32 cmd;

    switch (id) {
    case GX_ITM_0:
    case GX_ITM_1:
    case GX_ITM_2:
        index = id - GX_ITM_0;
        break;
    case GX_ITM_S0:
    case GX_ITM_S1:
    case GX_ITM_S2:
        index = id - GX_ITM_S0;
        break;
    case GX_ITM_T0:
    case GX_ITM_T1:
    case GX_ITM_T2:
        index = id - GX_ITM_T0;
        break;
    case 4:
    case 8:
    default:
        index = 0;
    }

    scaleExp += 17;

    cmd = 0;
    cmd = GX_BITSET(cmd, 21, 11, 1024.0f * offset[0][0]);
    cmd = GX_BITSET(cmd, 10, 11, 1024.0f * offset[1][0]);
    cmd = GX_BITSET(cmd, 8, 2, scaleExp);
    GX_BP_SET_OPCODE(cmd, index * 3 + GX_BP_REG_INDMTX0A);
    GX_LOAD_BP_REG(cmd);

    cmd = 0;
    cmd = GX_BITSET(cmd, 21, 11, 1024.0f * offset[0][1]);
    cmd = GX_BITSET(cmd, 10, 11, 1024.0f * offset[1][1]);
    // TODO: Match using GX_BITSET
    // cmd = GX_BITSET(cmd, 8, 2, scaleExp >> 2 & 3);
    cmd = __rlwimi(cmd, scaleExp, 20, 8, 9);
    GX_BP_SET_OPCODE(cmd, index * 3 + GX_BP_REG_INDMTX0B);
    GX_LOAD_BP_REG(cmd);

    cmd = 0;
    cmd = GX_BITSET(cmd, 21, 11, 1024.0f * offset[0][2]);
    cmd = GX_BITSET(cmd, 10, 11, 1024.0f * offset[1][2]);
    // TODO: Match using GX_BITSET
    // cmd = GX_BITSET(cmd, 8, 2, scaleExp >> 4 & 3);
    cmd = __rlwimi(cmd, scaleExp, 18, 8, 9);
    GX_BP_SET_OPCODE(cmd, index * 3 + GX_BP_REG_INDMTX0C);
    GX_LOAD_BP_REG(cmd);

    gxdt->xfWritten = FALSE;
}

void GXSetIndTexCoordScale(GXIndTexStageID stage, GXIndTexScale scaleS,
                           GXIndTexScale scaleT) {
    switch (stage) {
    case GX_INDTEXSTAGE0:
        gxdt->ras1_ss0 = GX_BITSET(gxdt->ras1_ss0, 28, 4, scaleS);
        gxdt->ras1_ss0 = GX_BITSET(gxdt->ras1_ss0, 24, 4, scaleT);
        GX_BP_SET_OPCODE(gxdt->ras1_ss0, GX_BP_REG_RAS1_SS0);
        GX_LOAD_BP_REG(gxdt->ras1_ss0);
        break;
    case GX_INDTEXSTAGE1:
        gxdt->ras1_ss0 = GX_BITSET(gxdt->ras1_ss0, 20, 4, scaleS);
        gxdt->ras1_ss0 = GX_BITSET(gxdt->ras1_ss0, 16, 4, scaleT);
        GX_BP_SET_OPCODE(gxdt->ras1_ss0, GX_BP_REG_RAS1_SS0);
        GX_LOAD_BP_REG(gxdt->ras1_ss0);
        break;
    case GX_INDTEXSTAGE2:
        gxdt->ras1_ss1 = GX_BITSET(gxdt->ras1_ss1, 28, 4, scaleS);
        gxdt->ras1_ss1 = GX_BITSET(gxdt->ras1_ss1, 24, 4, scaleT);
        GX_BP_SET_OPCODE(gxdt->ras1_ss1, GX_BP_REG_RAS1_SS1);
        GX_LOAD_BP_REG(gxdt->ras1_ss1);
        break;
    case GX_INDTEXSTAGE3:
        gxdt->ras1_ss1 = GX_BITSET(gxdt->ras1_ss1, 20, 4, scaleS);
        gxdt->ras1_ss1 = GX_BITSET(gxdt->ras1_ss1, 16, 4, scaleT);
        GX_BP_SET_OPCODE(gxdt->ras1_ss1, GX_BP_REG_RAS1_SS1);
        GX_LOAD_BP_REG(gxdt->ras1_ss1);
        break;
    }

    gxdt->xfWritten = FALSE;
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
        gxdt->ras1_iref = GX_BITSET(gxdt->ras1_iref, 29, 3, map);
        gxdt->ras1_iref = GX_BITSET(gxdt->ras1_iref, 26, 3, coord);
        break;
    case GX_INDTEXSTAGE1:
        gxdt->ras1_iref = GX_BITSET(gxdt->ras1_iref, 23, 3, map);
        gxdt->ras1_iref = GX_BITSET(gxdt->ras1_iref, 20, 3, coord);
        break;
    case GX_INDTEXSTAGE2:
        gxdt->ras1_iref = GX_BITSET(gxdt->ras1_iref, 17, 3, map);
        gxdt->ras1_iref = GX_BITSET(gxdt->ras1_iref, 14, 3, coord);
        break;
    case GX_INDTEXSTAGE3:
        gxdt->ras1_iref = GX_BITSET(gxdt->ras1_iref, 11, 3, map);
        gxdt->ras1_iref = GX_BITSET(gxdt->ras1_iref, 8, 3, coord);
        break;
    }

    GX_LOAD_BP_REG(gxdt->ras1_iref);
    gxdt->gxDirtyFlags |= GX_DIRTY_SU_TEX;
    gxdt->gxDirtyFlags |= GX_DIRTY_BP_MASK;
    gxdt->xfWritten = FALSE;
}

void GXSetNumIndStages(u8 num) {
    gxdt->genMode = GX_BITSET(gxdt->genMode, 13, 3, num);
    gxdt->gxDirtyFlags |= GX_DIRTY_BP_MASK;
    gxdt->gxDirtyFlags |= GX_DIRTY_GEN_MODE;
}

void GXSetTevDirect(GXTevStageID stage) {
    GXSetTevIndirect(stage, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_NONE, GX_ITM_OFF,
                     GX_ITW_OFF, GX_ITW_OFF, FALSE, FALSE, GX_ITBA_OFF);
}

void __GXUpdateBPMask(void) {}

void __GXSetIndirectMask(u32 mask) {
    gxdt->ind_imask = GX_BITSET(gxdt->ind_imask, 24, 8, mask);
    GX_LOAD_BP_REG(gxdt->ind_imask);
    gxdt->xfWritten = FALSE;
}

void __GXFlushTextureState(void) {
    GX_LOAD_BP_REG(gxdt->ind_imask);
    gxdt->xfWritten = FALSE;
}

#include <revolution/GX.h>

inline void __GXSetAmbMat(u32 dirtyFlags) {
    if (dirtyFlags & GX_DIRTY_AMB_COLOR0) {
        GX_LOAD_XF_REG(GX_XF_REG_AMBIENT0, *(u32*)&gxdt->ambColors[0]);
    }

    if (dirtyFlags & GX_DIRTY_AMB_COLOR1) {
        GX_LOAD_XF_REG(GX_XF_REG_AMBIENT1, *(u32*)&gxdt->ambColors[1]);
    }

    if (dirtyFlags & GX_DIRTY_MAT_COLOR0) {
        GX_LOAD_XF_REG(GX_XF_REG_MATERIAL0, *(u32*)&gxdt->matColors[0]);
    }

    if (dirtyFlags & GX_DIRTY_MAT_COLOR1) {
        GX_LOAD_XF_REG(GX_XF_REG_MATERIAL1, *(u32*)&gxdt->matColors[1]);
    }
}

inline void __GXSetLightChan(volatile s32 dirtyFlags) {
    int i;
    u32 bits;
    u32 addr = GX_XF_REG_COLOR0CNTRL;

    if (dirtyFlags & 0x1000000) {
        u32 colors = (gxdt->genMode & 0x70) >> 4;
        GX_LOAD_XF_REG(GX_XF_REG_NUMCOLORS, colors);
    }

    bits = dirtyFlags;
    bits = (bits >> 12) & 0xF;
    i = 0;
    for (; bits != 0; i++, addr++, bits >>= 1) {
        if (bits & 1) {
            GX_LOAD_XF_REG(addr, gxdt->colorControl[i]);
        }
    }
}

inline void __GXSetTexGen(volatile s32 dirtyFlags) {
    int i;
    u32 bits;
    u32 taddr = GX_XF_REG_TEX0;
    u32 dtaddr;

    if (dirtyFlags & 0x2000000) {
        u32 gens = gxdt->genMode & 0xF;
        GX_LOAD_XF_REG(GX_XF_REG_NUMTEX, gens);
    }

    bits = dirtyFlags;
    bits = (bits >> 16) & 0xFF;
    i = 0;
    for (; bits != 0; taddr++, i++, bits >>= 1) {
        dtaddr = taddr + GX_XF_REG_DUALTEX0 - GX_XF_REG_TEX0;
        if (bits & 1) {
            GX_LOAD_XF_REG(taddr, gxdt->texRegs[i]);
            GX_LOAD_XF_REG(dtaddr, gxdt->dualTexRegs[i]);
        }
    }
}

void __GXSetDirtyState(void) {
    u32 tempFlags;
    u32 dirtyFlags = gxdt->dirtyFlags;

    if (dirtyFlags & GX_DIRTY_SU_TEX) {
        __GXSetSUTexRegs();
    }

    if (dirtyFlags & GX_DIRTY_BP_MASK) {
        __GXUpdateBPMask();
    }

    if (dirtyFlags & GX_DIRTY_GEN_MODE) {
        __GXSetGenMode();
    }

    if (dirtyFlags & GX_DIRTY_VCD) {
        __GXSetVCD();
    }

    if (dirtyFlags & GX_DIRTY_VAT) {
        __GXSetVAT();
    }

    if (dirtyFlags & GX_DIRTY_VLIM) {
        __GXCalculateVLim();
    }

    dirtyFlags &= ~0xFF;
    if (dirtyFlags) {
        tempFlags = dirtyFlags & GX_AMB_MAT_MASK;
        if (tempFlags != 0) {
            __GXSetAmbMat(tempFlags);
        }

        tempFlags = dirtyFlags & GX_LIGHT_CHAN_MASK;
        if (tempFlags != 0) {
            __GXSetLightChan(tempFlags);
        }

        tempFlags = dirtyFlags & GX_TEX_GEN_MASK;
        if (tempFlags != 0) {
            __GXSetTexGen(tempFlags);
        }

        tempFlags = dirtyFlags & GX_DIRTY_MTX_IDX;
        if (tempFlags != 0) {
            __GXSetMatrixIndex(0);
            __GXSetMatrixIndex(5);
        }

        tempFlags = dirtyFlags & GX_DIRTY_VIEWPORT;
        if (tempFlags != 0) {
            __GXSetViewport();
        }

        tempFlags = dirtyFlags & GX_DIRTY_PROJECTION;
        if (tempFlags != 0) {
            __GXSetProjection();
        }

        gxdt->xfWritten = TRUE;
    }

    gxdt->dirtyFlags = 0;
}

void GXBegin(GXPrimitive prim, GXVtxFmtIdx fmt, u16 verts) {
    if (gxdt->dirtyFlags != 0) {
        __GXSetDirtyState();
    }

    if (gxdt->WORD_0x0 == 0) {
        __GXSendFlushPrim();
    }

    WGPIPE.c = fmt | prim;
    WGPIPE.s = verts;
}

void __GXSendFlushPrim(void) {
    u32 i;
    u32 sz = gxdt->SHORT_0x4 * gxdt->SHORT_0x6;

    WGPIPE.uc = GX_TRIANGLESTRIP;
    WGPIPE.us = gxdt->SHORT_0x4;

    for (i = 0; i < sz; i += 4) {
        WGPIPE.i = 0;
    }

    gxdt->xfWritten = TRUE;
}

void GXSetLineWidth(u8 width, u32 offset) {
    gxdt->linePtWidth = GX_BITSET(gxdt->linePtWidth, 24, 8, width);
    gxdt->linePtWidth = GX_BITSET(gxdt->linePtWidth, 13, 3, offset);
    GX_LOAD_BP_REG(gxdt->linePtWidth);
    gxdt->xfWritten = FALSE;
}

void GXSetPointSize(u8 size, u32 offset) {
    gxdt->linePtWidth = GX_BITSET(gxdt->linePtWidth, 16, 8, size);
    gxdt->linePtWidth = GX_BITSET(gxdt->linePtWidth, 10, 3, offset);
    GX_LOAD_BP_REG(gxdt->linePtWidth);
    gxdt->xfWritten = FALSE;
}

void GXEnableTexOffsets(GXTexCoordID coordId, GXBool8 lineOfs,
                        GXBool8 pointOfs) {
    gxdt->txcRegs[coordId] = GX_BITSET(gxdt->txcRegs[coordId], 13, 1, lineOfs);
    gxdt->txcRegs[coordId] = GX_BITSET(gxdt->txcRegs[coordId], 12, 1, pointOfs);
    GX_LOAD_BP_REG(gxdt->txcRegs[coordId]);
    gxdt->xfWritten = FALSE;
}

void GXSetCullMode(GXCullMode mode) {
    GXCullMode bits = (GXCullMode)(mode << 1 & 2 | mode >> 1 & 1);
    gxdt->genMode = GX_BITSET(gxdt->genMode, 16, 2, bits);
    gxdt->dirtyFlags |= GX_DIRTY_GEN_MODE;
}

void GXGetCullMode(GXCullMode* out) {
    s32 bits = 0;
    bits |= (s32)(gxdt->genMode >> 14 & 2) >> 1;
    bits |= (s32)(gxdt->genMode >> 13 & 2);
    *out = (GXCullMode)bits;
}

void GXSetCoPlanar(GXBool8 coplanar) {
    gxdt->genMode = GX_BITSET(gxdt->genMode, 12, 1, coplanar);
    GX_LOAD_BP_REG(GX_BP_REG_SS_MASK << 24 | 0x80000)
    GX_LOAD_BP_REG(gxdt->genMode);
}

void __GXSetGenMode(void) {
    GX_LOAD_BP_REG(gxdt->genMode);
    gxdt->xfWritten = FALSE;
}

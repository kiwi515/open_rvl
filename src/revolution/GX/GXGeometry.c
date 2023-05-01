#include <revolution/GX.h>

inline void __GXSetAmbMat(u32 dirtyFlags) {
    if (dirtyFlags & GX_DIRTY_AMB_COLOR0) {
        GX_WRITE_XF_CMD(GX_FIFO_XF_SETCHAN0_AMBCOLOR,
                        *(u32*)&__GXData->ambColors[0]);
    }

    if (dirtyFlags & GX_DIRTY_AMB_COLOR1) {
        GX_WRITE_XF_CMD(GX_FIFO_XF_SETCHAN1_AMBCOLOR,
                        *(u32*)&__GXData->ambColors[1]);
    }

    if (dirtyFlags & GX_DIRTY_MAT_COLOR0) {
        GX_WRITE_XF_CMD(GX_FIFO_XF_SETCHAN0_MATCOLOR,
                        *(u32*)&__GXData->matColors[0]);
    }

    if (dirtyFlags & GX_DIRTY_MAT_COLOR1) {
        GX_WRITE_XF_CMD(GX_FIFO_XF_SETCHAN1_MATCOLOR,
                        *(u32*)&__GXData->matColors[1]);
    }
}

inline void __GXSetLightChan(volatile s32 dirtyFlags) {
    int i;
    u32 bits;
    u32 addr = GX_FIFO_XF_SETCHAN0_COLOR;

    if (dirtyFlags & 0x1000000) {
        u32 chans = (__GXData->WORD_0x254 & 0x70) >> 4;
        GX_WRITE_XF_CMD(GX_FIFO_XF_SETNUMCHAN, chans);
    }

    bits = dirtyFlags;
    bits = (bits >> 12) & 0xF;
    i = 0;
    for (; bits != 0; i++, addr++, bits >>= 1) {
        if (bits & 0x1) {
            GX_WRITE_XF_CMD(addr, __GXData->WORDS_0xB8[i]);
        }
    }
}

inline void __GXSetTexGen(volatile s32 dirtyFlags) {
    int i;
    u32 bits;
    u32 addr = GX_FIFO_XF_SETTEXMTXINFO;
    u32 addr2;

    if (dirtyFlags & 0x2000000) {
        u32 gens = __GXData->WORD_0x254 & 0xF;
        GX_WRITE_XF_CMD(GX_FIFO_XF_SETNUMTEXGENS, gens);
    }

    bits = dirtyFlags;
    bits = (bits >> 16) & 0xFF;
    i = 0;
    for (; bits != 0; addr++, i++, bits >>= 1) {
        addr2 = addr + 0x10;
        if (bits & 0x1) {
            GX_WRITE_XF_CMD(addr, __GXData->WORDS_0xC8[i]);
            GX_WRITE_XF_CMD(addr2, __GXData->WORDS_0xE8[i]);
        }
    }
}

void __GXSetDirtyState(void) {
    u32 tempFlags;
    u32 dirtyFlags = __GXData->dirtyFlags;

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

        __GXData->SHORTS_0x0[1] = 1;
    }

    __GXData->dirtyFlags = 0;
}

void GXBegin(GXPrimitive prim, UNKWORD fmtIndex, u16 num) {
    if (__GXData->dirtyFlags != 0) {
        __GXSetDirtyState();
    }

    if (__GXData->WORD_0x0 == 0) {
        __GXSendFlushPrim();
    }

    WGPIPE.c = fmtIndex | prim;
    WGPIPE.s = num;
}

void __GXSendFlushPrim(void) {
    u32 i;
    u32 sz = __GXData->SHORT_0x4 * __GXData->SHORT_0x6;

    WGPIPE.uc = GX_TRIANGLESTRIP;
    WGPIPE.us = __GXData->SHORT_0x4;

    for (i = 0; i < sz; i += 4) {
        WGPIPE.i = 0;
    }

    __GXData->SHORTS_0x0[1] = 1;
}

void GXSetLineWidth(u8 width, UNKWORD arg1) {
    __GXData->WORD_0x7C = GX_BITSET(__GXData->WORD_0x7C, 24, 8, width);
    __GXData->WORD_0x7C = GX_BITSET(__GXData->WORD_0x7C, 13, 3, arg1);
    GX_WRITE_BP_CMD(__GXData->WORD_0x7C);
    __GXData->SHORTS_0x0[1] = 0;
}

void GXSetPointSize(u8 size, UNKWORD arg1) {
    __GXData->WORD_0x7C = GX_BITSET(__GXData->WORD_0x7C, 16, 8, size);
    __GXData->WORD_0x7C = GX_BITSET(__GXData->WORD_0x7C, 10, 3, arg1);
    GX_WRITE_BP_CMD(__GXData->WORD_0x7C);
    __GXData->SHORTS_0x0[1] = 0;
}

void GXEnableTexOffsets(UNKWORD coordId, GXBool8 arg1, GXBool8 arg2) {
    __GXData->WORDS_0x108[coordId] =
        GX_BITSET(__GXData->WORDS_0x108[coordId], 13, 1, arg1);
    __GXData->WORDS_0x108[coordId] =
        GX_BITSET(__GXData->WORDS_0x108[coordId], 12, 1, arg2);
    GX_WRITE_BP_CMD(__GXData->WORDS_0x108[coordId]);
    __GXData->SHORTS_0x0[1] = 0;
}

void GXSetCullMode(GXCullMode mode) {
    GXCullMode bits = (GXCullMode)(mode << 1 & 2 | mode >> 1 & 1);
    __GXData->WORD_0x254 = GX_BITSET(__GXData->WORD_0x254, 16, 2, bits);
    __GXData->dirtyFlags |= GX_DIRTY_GEN_MODE;
}

void GXGetCullMode(GXCullMode* out) {
    GXCullMode bits = (GXCullMode)__GXData->WORD_0x254;
    *out = (GXCullMode)(((bits >> 14 & 2) >> 1) & ~2 | bits >> 13 & 2);
}

void GXSetCoPlanar(GXBool8 enable) {
    __GXData->WORD_0x254 = GX_BITSET(__GXData->WORD_0x254, 12, 1, enable);
    GX_WRITE_BP_CMD(0xFE080000);
    GX_WRITE_BP_CMD(__GXData->WORD_0x254);
}

void __GXSetGenMode(void) {
    GX_WRITE_BP_CMD(__GXData->WORD_0x254);
    __GXData->SHORTS_0x0[1] = 0;
}

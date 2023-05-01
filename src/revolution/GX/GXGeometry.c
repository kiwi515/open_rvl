#include <revolution/GX.h>

#ifndef NON_MATCHING
#error __GXSetAmbMat has not yet been matched.
#endif
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

#ifndef NON_MATCHING
#error __GXSetLightChan has not yet been matched.
#endif
inline void __GXSetLightChan(u32 dirtyFlags) {
    int i;
    u32 bit;
    u32 addr = GX_FIFO_XF_SETCHAN0_COLOR;

    if (dirtyFlags & 0x1000000) {
        u32 chans = (__GXData->WORD_0x254 & 0x70) >> 4;
        GX_WRITE_XF_CMD(GX_FIFO_XF_SETNUMCHAN, chans);
    }

    bit = dirtyFlags & 0xF000 >> 12;
    i = 0;
    for (; bit != 0; i++, addr++, bit >>= 1) {
        if (bit & 0x1) {
            GX_WRITE_XF_CMD(addr, __GXData->WORDS_0xB8[i]);
        }
    }
}

#ifndef NON_MATCHING
#error __GXSetTexGen has not yet been matched.
#endif
inline void __GXSetTexGen(u32 dirtyFlags) {
    int i;
    u32 bit;
    u32 addr = GX_FIFO_XF_SETTEXMTXINFO;

    if (dirtyFlags & 0x2000000) {
        u32 gens = __GXData->WORD_0x254 & 0xF;
        GX_WRITE_XF_CMD(GX_FIFO_XF_SETNUMTEXGENS, gens);
    }

    bit = dirtyFlags & 0x2FF0000 >> 16;
    i = 0;
    for (; bit != 0; addr++, i++, bit >>= 1) {
        if (bit & 0x1) {
            GX_WRITE_XF_CMD(addr, __GXData->WORDS_0xC8[i]);
            GX_WRITE_XF_CMD(addr, __GXData->WORDS_0xE8[i]);
        }
    }
}

#ifndef NON_MATCHING
#error __GXSetDirtyState has not yet been matched. (https://decomp.me/scratch/qKk9k)
#endif
void __GXSetDirtyState(void) {
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

    if (dirtyFlags & ~0xFF) {
        if (dirtyFlags & GX_AMB_MAT_MASK) {
            __GXSetAmbMat(dirtyFlags & GX_AMB_MAT_MASK);
        }
        if (dirtyFlags & GX_LIGHT_CHAN_MASK) {
            __GXSetLightChan(dirtyFlags & GX_LIGHT_CHAN_MASK);
        }
        if (dirtyFlags & GX_TEX_GEN_MASK) {
            __GXSetTexGen(dirtyFlags & GX_TEX_GEN_MASK);
        }

        if (dirtyFlags & GX_DIRTY_MTX_IDX) {
            __GXSetMatrixIndex(0);
            __GXSetMatrixIndex(5);
        }
        if (dirtyFlags & GX_DIRTY_VIEWPORT) {
            __GXSetViewport();
        }
        if (dirtyFlags & GX_DIRTY_PROJECTION) {
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

    WGPIPE.c = GX_TRIANGLESTRIP;
    WGPIPE.us = __GXData->SHORT_0x4;

    for (i = 0; i < sz; i += 4) {
        WGPIPE.i = 0;
    }

    __GXData->SHORTS_0x0[1] = 1;
}

void GXSetLineWidth(u8 width, UNKWORD r5) {
    __GXData->WORD_0x7C = GX_BITSET(__GXData->WORD_0x7C, 24, 8, width);
    __GXData->WORD_0x7C = GX_BITSET(__GXData->WORD_0x7C, 13, 3, r5);
    GX_WRITE_BP_CMD(__GXData->WORD_0x7C);
    __GXData->SHORTS_0x0[1] = 0;
}

void GXSetPointSize(u8 size, UNKWORD r5) {
    __GXData->WORD_0x7C = GX_BITSET(__GXData->WORD_0x7C, 16, 8, size);
    __GXData->WORD_0x7C = GX_BITSET(__GXData->WORD_0x7C, 10, 3, r5);
    GX_WRITE_BP_CMD(__GXData->WORD_0x7C);
    __GXData->SHORTS_0x0[1] = 0;
}

void GXEnableTexOffsets(UNKWORD coordId, GXBool8 r4, GXBool8 r5) {
    __GXData->WORDS_0x108[coordId] =
        GX_BITSET(__GXData->WORDS_0x108[coordId], 13, 1, r4);
    __GXData->WORDS_0x108[coordId] =
        GX_BITSET(__GXData->WORDS_0x108[coordId], 12, 1, r5);
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

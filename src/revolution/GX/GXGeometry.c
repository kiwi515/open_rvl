#include <revolution/GX/GXAttr.h>
#include <revolution/GX/GXBump.h>
#include <revolution/GX/GXGeometry.h>
#include <revolution/GX/GXInit.h>
#include <revolution/GX/GXTexture.h>
#include <revolution/GX/GXTransform.h>

#ifndef NON_MATCHING
#error __GXSetAmbMat has not yet been matched.
#endif
inline void __GXSetAmbMat(u32 flags) {
    if (flags & GX_DIRTY_AMB_COLOR0) {
        WGPIPE.c = 0x10;
        WGPIPE.i = 0x100A;
        WGPIPE.i = *(u32*)&__GXData->ambColors[0];
    }
    if (flags & GX_DIRTY_AMB_COLOR1) {
        WGPIPE.c = 0x10;
        WGPIPE.i = 0x100B;
        WGPIPE.i = *(u32*)&__GXData->ambColors[1];
    }
    if (flags & GX_DIRTY_MAT_COLOR0) {
        WGPIPE.c = 0x10;
        WGPIPE.i = 0x100C;
        WGPIPE.i = *(u32*)&__GXData->matColors[0];
    }
    if (flags & GX_DIRTY_MAT_COLOR1) {
        WGPIPE.c = 0x10;
        WGPIPE.i = 0x100D;
        WGPIPE.i = *(u32*)&__GXData->matColors[1];
    }
}

#ifndef NON_MATCHING
#error __GXSetLightChan has not yet been matched.
#endif
inline void __GXSetLightChan(u32 flags) {
    int i;
    u32 bit;
    u32 val = 0x100E;

    if (flags & 0x1000000) {
        u32 r5 = (__GXData->WORD_0x254 & 0x70) >> 4;
        WGPIPE.c = 0x10;
        WGPIPE.i = 0x1009;
        WGPIPE.i = r5;
    }

    bit = flags & 0xF000 >> 12;
    i = 0;
    for (; bit != 0; i++, val++, bit >>= 1) {
        if (bit & 0x1) {
            WGPIPE.c = 0x10;
            WGPIPE.i = val;
            WGPIPE.i = __GXData->WORDS_0xB8[i];
        }
    }
}

#ifndef NON_MATCHING
#error __GXSetTexGen has not yet been matched.
#endif
inline void __GXSetTexGen(u32 flags) {
    int i;
    u32 bit;
    u32 val = 0x1040;

    if (flags & 0x2000000) {
        u32 r5 = __GXData->WORD_0x254 & 0xF;
        WGPIPE.c = 0x10;
        WGPIPE.i = 0x103F;
        WGPIPE.i = r5;
    }

    bit = flags & 0x2FF0000 >> 16;
    i = 0;
    for (; bit != 0; val++, i++, bit >>= 1) {
        u32 r5 = val + 0x10;

        if (bit & 0x1) {
            WGPIPE.c = 0x10;
            WGPIPE.i = val;
            WGPIPE.i = __GXData->WORDS_0xC8[i];

            WGPIPE.c = 0x10;
            WGPIPE.i = r5;
            WGPIPE.i = __GXData->WORDS_0xE8[i];
        }
    }
}

#ifndef NON_MATCHING
#error __GXSetDirtyState has not yet been matched. (https://decomp.me/scratch/qKk9k)
#endif
void __GXSetDirtyState(void) {
    u32 flags = __GXData->dirtyFlags;

    if (flags & GX_DIRTY_SU_TEX) {
        __GXSetSUTexRegs();
    }
    if (flags & GX_DIRTY_BP_MASK) {
        __GXUpdateBPMask();
    }
    if (flags & GX_DIRTY_GEN_MODE) {
        __GXSetGenMode();
    }
    if (flags & GX_DIRTY_VCD) {
        __GXSetVCD();
    }
    if (flags & GX_DIRTY_VAT) {
        __GXSetVAT();
    }
    if (flags & GX_DIRTY_VLIM) {
        __GXCalculateVLim();
    }

    if (flags & ~0xFF) {
        if (flags & GX_AMB_MAT_MASK) {
            __GXSetAmbMat(flags & GX_AMB_MAT_MASK);
        }
        if (flags & GX_LIGHT_CHAN_MASK) {
            __GXSetLightChan(flags & GX_LIGHT_CHAN_MASK);
        }
        if (flags & GX_TEX_GEN_MASK) {
            __GXSetTexGen(flags & GX_TEX_GEN_MASK);
        }

        if (flags & GX_DIRTY_MTX_IDX) {
            __GXSetMatrixIndex(0);
            __GXSetMatrixIndex(5);
        }
        if (flags & GX_DIRTY_VIEWPORT) {
            __GXSetViewport();
        }
        if (flags & GX_DIRTY_PROJECTION) {
            __GXSetProjection();
        }

        __GXData->SHORTS_0x0[1] = 1;
    }
    __GXData->dirtyFlags = 0;
}

void GXBegin(GXPrimitive prim, UNKWORD fmtIndex, u16 r5) {
    if (__GXData->dirtyFlags != 0) {
        __GXSetDirtyState();
    }

    if (__GXData->WORD_0x0 == 0) {
        __GXSendFlushPrim();
    }

    WGPIPE.c = fmtIndex | prim;
    WGPIPE.s = r5;
}

void __GXSendFlushPrim(void) {
    u32 i;
    u32 sz = __GXData->SHORT_0x4 * __GXData->SHORT_0x6;

    WGPIPE.c = 0x98;
    WGPIPE.us = __GXData->SHORT_0x4;

    for (i = 0; i < sz; i += 4) {
        WGPIPE.i = 0;
    }

    __GXData->SHORTS_0x0[1] = 1;
}

void GXSetLineWidth(u8 width, UNKWORD r5) {
    GX_BITFIELD_SET(__GXData->WORD_0x7C, 24, 8, width);
    GX_BITFIELD_SET(__GXData->WORD_0x7C, 13, 3, r5);
    WGPIPE.c = 0x61;
    WGPIPE.i = __GXData->WORD_0x7C;
    __GXData->SHORTS_0x0[1] = 0;
}

void GXSetPointSize(u8 size, UNKWORD r5) {
    GX_BITFIELD_SET(__GXData->WORD_0x7C, 16, 8, size);
    GX_BITFIELD_SET(__GXData->WORD_0x7C, 10, 3, r5);
    WGPIPE.c = 0x61;
    WGPIPE.i = __GXData->WORD_0x7C;
    __GXData->SHORTS_0x0[1] = 0;
}

void GXEnableTexOffsets(UNKWORD coordId, u8 r4, u8 r5) {
    GX_BITFIELD_SET(__GXData->WORDS_0x108[coordId], 13, 1, r4);
    GX_BITFIELD_SET(__GXData->WORDS_0x108[coordId], 12, 1, r5);
    WGPIPE.c = 0x61;
    WGPIPE.i = __GXData->WORDS_0x108[coordId];
    __GXData->SHORTS_0x0[1] = 0;
}

void GXSetCullMode(GXCullMode mode) {
    GXCullMode mode2 = (GXCullMode)(mode << 1 & 2 | mode >> 1 & 1);
    GX_BITFIELD_SET(__GXData->WORD_0x254, 16, 2, mode2);
    __GXData->dirtyFlags |= GX_DIRTY_GEN_MODE;
}

void GXGetCullMode(GXCullMode* out) {
    u32 temp_r4 = __GXData->WORD_0x254;
    *out =
        (GXCullMode)(((s32)(temp_r4 >> 14 & 2) >> 1) & ~2 | temp_r4 >> 13 & 2);
}

void GXSetCoPlanar(u8 enable) {
    GX_BITFIELD_SET(__GXData->WORD_0x254, 12, 1, enable);
    WGPIPE.c = 0x61;
    WGPIPE.i = 0xFE080000;
    WGPIPE.c = 0x61;
    WGPIPE.i = __GXData->WORD_0x254;
}

void __GXSetGenMode(void) {
    WGPIPE.c = 0x61;
    WGPIPE.i = __GXData->WORD_0x254;
    __GXData->SHORTS_0x0[1] = 0;
}

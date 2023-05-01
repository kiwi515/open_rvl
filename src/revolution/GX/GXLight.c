#include <math.h>
#include <revolution/GX.h>

void GXInitLightAttn(GXLightObj* light, f32 aa, f32 ab, f32 ac, f32 ka, f32 kb,
                     f32 kc) {
    GXLightObjImpl* impl = (GXLightObjImpl*)light;

    impl->aa = aa;
    impl->ab = ab;
    impl->ac = ac;
    impl->ka = ka;
    impl->kb = kb;
    impl->kc = kc;
}

void GXInitLightAttnA(GXLightObj* light, f32 a, f32 b, f32 c) {
    GXLightObjImpl* impl = (GXLightObjImpl*)light;

    impl->aa = a;
    impl->ab = b;
    impl->ac = c;
}

void GXInitLightAttnK(GXLightObj* light, f32 a, f32 b, f32 c) {
    GXLightObjImpl* impl = (GXLightObjImpl*)light;

    impl->ka = a;
    impl->kb = b;
    impl->kc = c;
}

void GXInitLightSpot(GXLightObj* light, f32 angle, GXSpotFn fn) {
    GXLightObjImpl* impl = (GXLightObjImpl*)light;
    f32 a, b, c, d;
    f32 rad;

    if (angle <= 0.0f || angle > 90.0f) {
        fn = GX_SP_OFF;
    }

    rad = cosf(M_PI * angle / 180.0f);

    switch (fn) {
    case GX_SP_FLAT:
        a = -1000.0f * rad;
        b = 1000.0f;
        c = 0.0f;
        break;
    case GX_SP_COS:
        b = 1.0f / (1.0f - rad);
        a = -rad * b;
        c = 0.0f;
        break;
    case GX_SP_COS2:
        c = 1.0f / (1.0f - rad);
        a = 0.0f;
        b = -rad * c;
        break;
    case GX_SP_SHARP:
        d = 1.0f / ((1.0f - rad) * (1.0f - rad));
        a = (rad * (rad - 2.0f)) * d;
        b = 2.0f * d;
        c = -d;
        break;
    case GX_SP_RING1:
        d = 1.0f / ((1.0f - rad) * (1.0f - rad));
        c = -4.0f * d;
        a = c * rad;
        b = 4.0f * (1.0f + rad) * d;
        break;
    case GX_SP_RING2:
        d = 1.0f / ((1.0f - rad) * (1.0f - rad));
        a = 1.0f - 2.0f * rad * rad * d;
        b = 4.0f * rad * d;
        c = -2.0f * d;
        break;
    case GX_SP_OFF:
    default:
        a = 1.0f;
        b = 0.0f;
        c = 0.0f;
        break;
    }

    impl->aa = a;
    impl->ab = b;
    impl->ac = c;
}

void GXInitLightDistAttn(GXLightObj* light, f32 distance, f32 brightness,
                         GXDistAttnFn fn) {
    GXLightObjImpl* impl = (GXLightObjImpl*)light;
    f32 a, b, c;

    if (distance < 0.0f) {
        fn = GX_DA_OFF;
    }

    if (brightness <= 0.0f || brightness >= 1.0f) {
        fn = GX_DA_OFF;
    }

    switch (fn) {
    case GX_DA_GENTLE:
        a = 1.0f;
        b = (1.0f - brightness) / (brightness * distance);
        c = 0.0f;
        break;
    case GX_DA_MEDIUM:
        a = 1.0f;
        b = (0.5f * (1.0f - brightness)) / (brightness * distance);
        c = (0.5f * (1.0f - brightness)) / (distance * (brightness * distance));
        break;
    case GX_DA_STEEP:
        a = 1.0f;
        b = 0.0f;
        c = (1.0f - brightness) / (distance * (brightness * distance));
        break;
    case GX_DA_OFF:
    default:
        a = 1.0f;
        b = 0.0f;
        c = 0.0f;
        break;
    }

    impl->ka = a;
    impl->kb = b;
    impl->kc = c;
}

void GXInitLightPos(GXLightObj* light, f32 x, f32 y, f32 z) {
    GXLightObjImpl* impl = (GXLightObjImpl*)light;

    impl->posX = x;
    impl->posY = y;
    impl->posZ = z;
}

void GXGetLightPos(const GXLightObj* light, f32* x, f32* y, f32* z) {
    const GXLightObjImpl* impl = (GXLightObjImpl*)light;

    *x = impl->posX;
    *y = impl->posY;
    *z = impl->posZ;
}

void GXInitLightDir(GXLightObj* light, f32 x, f32 y, f32 z) {
    GXLightObjImpl* impl = (GXLightObjImpl*)light;

    impl->dirX = -x;
    impl->dirY = -y;
    impl->dirZ = -z;
}

void GXGetLightDir(const GXLightObj* light, f32* x, f32* y, f32* z) {
    const GXLightObjImpl* impl = (GXLightObjImpl*)light;

    *x = -impl->dirX;
    *y = -impl->dirY;
    *z = -impl->dirZ;
}

void GXInitSpecularDir(GXLightObj* light, f32 x, f32 y, f32 z) {
    GXLightObjImpl* impl = (GXLightObjImpl*)light;
    f32 dirX, dirY, dirZ;
    f32 mag;

    dirX = -x;
    dirY = -y;
    dirZ = 1.0f + -z;

    mag = dirX * dirX + dirY * dirY + dirZ * dirZ;
    if (mag != 0.0f) {
        mag = 1.0f / sqrtf(mag);
    }

    impl->dirX = dirX * mag;
    impl->dirY = dirY * mag;
    impl->dirZ = dirZ * mag;

    impl->posX = -999999999999999999.0f * x;
    impl->posY = -999999999999999999.0f * y;
    impl->posZ = -999999999999999999.0f * z;
}

void GXInitLightColor(GXLightObj* light, GXColor color) {
    GXLightObjImpl* impl = (GXLightObjImpl*)light;
    *(u32*)&impl->color = *(u32*)&color;
}

// TODO: This inline is fake, and also is a fake match (r6 hardcoded)
inline void CopyLightObj(register const GXLightObjImpl* src,
                         register volatile void* dst) {
    register u32 color;

    // clang-format off
    asm volatile {
        lwz color, src->color
        xor r6, r6, r6 // make zero
        psq_l f5, GXLightObjImpl.aa(src), 0, 0
        psq_l f4, GXLightObjImpl.ac(src), 0, 0
        psq_l f3, GXLightObjImpl.kb(src), 0, 0
        psq_l f2, GXLightObjImpl.posX(src), 0, 0
        psq_l f1, GXLightObjImpl.posZ(src), 0, 0
        psq_l f0, GXLightObjImpl.dirY(src), 0, 0

        stw r6, 0(dst)
        stw r6, 0(dst)
        stw r6, 0(dst)
            
        stw color, 0(dst)
        psq_st f5, 0(dst), 0, 0
        psq_st f4, 0(dst), 0, 0
        psq_st f3, 0(dst), 0, 0
        psq_st f2, 0(dst), 0, 0
        psq_st f1, 0(dst), 0, 0
        psq_st f0, 0(dst), 0, 0
    }
    // clang-format on
}

void GXLoadLightObjImm(const GXLightObj* light, GXLightID id) {
    const GXLightObjImpl* impl;
    u32 value;
    u32 num;

    impl = (GXLightObjImpl*)light;
    num = 31 - __cntlzw(id);
    num = (num % 8) << 4;

    value = num + 0x600;
    value |= 0xF0000;

    WGPIPE.c = GX_FIFO_LOAD_XF_REG;
    WGPIPE.i = value;
    CopyLightObj(impl, &WGPIPE);

    __GXData->SHORTS_0x0[1] = 1;
}

void GXLoadLightObjIndx(u32 index, GXLightID id) {
    u32 value;
    u32 num;

    value = 0;
    num = 31 - __cntlzw(id);
    num = (num % 8) << 4;

    value = GX_BITSET(value, 20, 12, num + 0x600);
    value |= 0xF000;
    value = GX_BITSET(value, 0, 16, index);

    WGPIPE.c = GX_FIFO_LOAD_INDX_D;
    WGPIPE.i = value;

    __GXData->SHORTS_0x0[1] = 1;
}

void GXSetChanAmbColor(GXChannelID chan, GXColor color) {
    u32 ambColor;
    u32 colorId;

    switch (chan) {
    case GX_COLOR0:
        ambColor = GX_BITSET_TRUNC(*(u32*)&__GXData->ambColors[0], 0, 24,
                                   *(u32*)&color);
        colorId = 0;
        break;
    case GX_COLOR1:
        ambColor = GX_BITSET_TRUNC(*(u32*)&__GXData->ambColors[1], 0, 24,
                                   *(u32*)&color);
        colorId = 1;
        break;
    case GX_ALPHA0:
        ambColor =
            GX_BITSET_TRUNC(*(u32*)&__GXData->ambColors[0], 24, 8, color.a);
        colorId = 0;
        break;
    case GX_ALPHA1:
        ambColor =
            GX_BITSET_TRUNC(*(u32*)&__GXData->ambColors[1], 24, 8, color.a);
        colorId = 1;
        break;
    case GX_COLOR0A0:
        ambColor = *(u32*)&color;
        colorId = 0;
        break;
    case GX_COLOR1A1:
        ambColor = *(u32*)&color;
        colorId = 1;
        break;
    default:
        return;
    }

    __GXData->dirtyFlags |= GX_DIRTY_AMB_COLOR0 << colorId;
    *(u32*)&__GXData->ambColors[colorId] = ambColor;
}

void GXSetChanMatColor(GXChannelID chan, GXColor color) {
    u32 matColor;
    u32 colorId;

    switch (chan) {
    case GX_COLOR0:
        matColor = GX_BITSET_TRUNC(*(u32*)&__GXData->matColors[0], 0, 24,
                                   *(u32*)&color);
        colorId = 0;
        break;
    case GX_COLOR1:
        matColor = GX_BITSET_TRUNC(*(u32*)&__GXData->matColors[1], 0, 24,
                                   *(u32*)&color);
        colorId = 1;
        break;
    case GX_ALPHA0:
        matColor =
            GX_BITSET_TRUNC(*(u32*)&__GXData->matColors[0], 24, 8, color.a);
        colorId = 0;
        break;
    case GX_ALPHA1:
        matColor =
            GX_BITSET_TRUNC(*(u32*)&__GXData->matColors[1], 24, 8, color.a);
        colorId = 1;
        break;
    case GX_COLOR0A0:
        matColor = *(u32*)&color;
        colorId = 0;
        break;
    case GX_COLOR1A1:
        matColor = *(u32*)&color;
        colorId = 1;
        break;
    default:
        return;
    }

    __GXData->dirtyFlags |= GX_DIRTY_MAT_COLOR0 << colorId;
    *(u32*)&__GXData->matColors[colorId] = matColor;
}

void GXSetNumChans(u8 num) {
    __GXData->WORD_0x254 = GX_BITSET(__GXData->WORD_0x254, 25, 3, num);
    __GXData->dirtyFlags |= 0x1000000;
    __GXData->dirtyFlags |= GX_DIRTY_GEN_MODE;
}

void GXSetChanCtrl(GXChannelID chan, GXBool8 enable, GXColorSrc ambSrc,
                   GXColorSrc matSrc, GXLightID lightMask, GXDiffuseFn diffFn,
                   GXAttnFn attnFn) {
    u32 field = 0;
    const u32 idx = chan & 3;

    field = GX_BITSET(field, 30, 1, enable);
    field = GX_BITSET(field, 31, 1, matSrc);
    field = GX_BITSET(field, 25, 1, ambSrc);
    field = GX_BITSET(field, 23, 2, attnFn == GX_AF_SPEC ? GX_DF_NONE : diffFn);
    field = GX_BITSET(field, 22, 1, attnFn != GX_AF_NONE);
    field = GX_BITSET(field, 21, 1, attnFn != GX_AF_SPEC);
    field = GX_BITSET(field, 26, 4, (u32)lightMask);
    field = GX_BITSET(field, 17, 4, (u32)lightMask >> 4);

    __GXData->WORDS_0xB8[idx] = field;
    __GXData->dirtyFlags |= (0x1000 << (idx));

    if (chan == GX_COLOR0A0) {
        __GXData->WORD_0xC0 = field;
        __GXData->dirtyFlags |= 0x5000;
    } else if (chan == GX_COLOR1A1) {
        __GXData->WORD_0xC4 = field;
        __GXData->dirtyFlags |= 0xA000;
    }
}

#include "GXLight.h"
#include "GXInit.h"

#include <math.h>

void GXInitLightAttn(GXLightObj* light, f32 aa, f32 ab, f32 ac, f32 ka, f32 kb,
                     f32 kc) {
    light->aa = aa;
    light->ab = ab;
    light->ac = ac;
    light->ka = ka;
    light->kb = kb;
    light->kc = kc;
}

void GXInitLightAttnA(GXLightObj* light, f32 a, f32 b, f32 c) {
    light->aa = a;
    light->ab = b;
    light->ac = c;
}

void GXInitLightAttnK(GXLightObj* light, f32 a, f32 b, f32 c) {
    light->ka = a;
    light->kb = b;
    light->kc = c;
}

void GXInitLightSpot(GXLightObj* light, f32 angle, GXSpotFn fn) {
    f32 rad;
    f32 a, b, c, d;

    if (angle <= 0.0f || angle > 90.0f) {
        fn = GX_SPOT_FN_0;
    }

    rad = cosf((3.1415927f * angle) / 180.0f);

    switch (fn) {
    case GX_SPOT_FN_1:
        a = -1000.0f * rad;
        b = 1000.0f;
        c = 0.0f;
        break;
    case GX_SPOT_FN_2:
        b = 1.0f / (1.0f - rad);
        a = -rad * b;
        c = 0.0f;
        break;
    case GX_SPOT_FN_3:
        c = 1.0f / (1.0f - rad);
        a = 0.0f;
        b = -rad * c;
        break;
    case GX_SPOT_FN_4:
        d = 1.0f / ((1.0f - rad) * (1.0f - rad));
        a = (rad * (rad - 2.0f)) * d;
        b = 2.0f * d;
        c = -d;
        break;
    case GX_SPOT_FN_5:
        d = 1.0f / ((1.0f - rad) * (1.0f - rad));
        c = -4.0f * d;
        a = c * rad;
        b = 4.0f * (1.0f + rad) * d;
        break;
    case GX_SPOT_FN_6:
        d = 1.0f / ((1.0f - rad) * (1.0f - rad));
        a = 1.0f - 2.0f * rad * rad * d;
        b = 4.0f * rad * d;
        c = -2.0f * d;
        break;
    case GX_SPOT_FN_0:
    default:
        a = 1.0f;
        b = 0.0f;
        c = 0.0f;
        break;
    }

    light->aa = a;
    light->ab = b;
    light->ac = c;
}

void GXInitLightDistAttn(GXLightObj* light, f32 f1, f32 f2, GXDistAttnFn fn) {
    f32 a, b, c;

    if (f1 < 0.0f) {
        fn = GX_DIST_ATTN_FN_0;
    }

    if (f2 <= 0.0f || f2 >= 1.0f) {
        fn = GX_DIST_ATTN_FN_0;
    }

    switch (fn) {
    case GX_DIST_ATTN_FN_1:
        a = 1.0f;
        b = (1.0f - f2) / (f2 * f1);
        c = 0.0f;
        break;
    case GX_DIST_ATTN_FN_2:
        a = 1.0f;
        b = (0.5f * (1.0f - f2)) / (f2 * f1);
        c = (0.5f * (1.0f - f2)) / (f1 * (f2 * f1));
        break;
    case GX_DIST_ATTN_FN_3:
        a = 1.0f;
        b = 0.0f;
        c = (1.0f - f2) / (f1 * (f2 * f1));
        break;
    case GX_DIST_ATTN_FN_0:
    default:
        a = 1.0f;
        b = 0.0f;
        c = 0.0f;
        break;
    }

    light->ka = a;
    light->kb = b;
    light->kc = c;
}

void GXInitLightPos(GXLightObj* light, f32 x, f32 y, f32 z) {
    light->posX = x;
    light->posY = y;
    light->posZ = z;
}

void GXGetLightPos(const GXLightObj* light, f32* x, f32* y, f32* z) {
    *x = light->posX;
    *y = light->posY;
    *z = light->posZ;
}

void GXInitLightDir(GXLightObj* light, f32 x, f32 y, f32 z) {
    light->dirX = -x;
    light->dirY = -y;
    light->dirZ = -z;
}

void GXGetLightDir(const GXLightObj* light, f32* x, f32* y, f32* z) {
    *x = -light->dirX;
    *y = -light->dirY;
    *z = -light->dirZ;
}

void GXInitSpecularDir(GXLightObj* light, f32 x, f32 y, f32 z) {
    f32 dirX, dirY, dirZ;
    f32 mag;

    dirX = -x;
    dirY = -y;
    dirZ = 1.0f + -z;

    mag = dirX * dirX + dirY * dirY + dirZ * dirZ;
    if (mag != 0.0f) {
        mag = 1.0f / sqrtf(mag);
    }

    light->dirX = dirX * mag;
    light->dirY = dirY * mag;
    light->dirZ = dirZ * mag;

    light->posX = -9.9999998E17f * x;
    light->posY = -9.9999998E17f * y;
    light->posZ = -9.9999998E17f * z;
}

void GXInitLightColor(GXLightObj* light, GXColor color) {
    *(u32*)&light->color = *(u32*)&color;
}

asm void GXLoadLightObjImm(register GXLightObj* light, register u32 id) {
    // clang-format off
    nofralloc

    cntlzw r0, id
    lis r4, WGPIPE@ha
    subfic r5, r0, 0x1f
    li r0, 0x10
    rlwinm r5, r5, 4, 0x19, 0x1b
    stb r0, WGPIPE@l(r4)
    addi r0, r5, 0x600
    oris r0, r0, 0xf
    stw r0, WGPIPE@l(r4)
    lwz r0, 0xc(r3)
    xor r6, r6, r6
    psq_l f5, 16(light), 0, 0
    psq_l f4, 24(light), 0, 0
    psq_l f3, 32(light), 0, 0
    psq_l f2, 40(light), 0, 0
    psq_l f1, 48(light), 0, 0
    psq_l f0, 56(light), 0, 0
    stwu r6, WGPIPE@l(r4)
    stw r6, 0(r4)
    stw r6, 0(r4)
    stw r0, 0(r4)
    psq_st f5, 0(r4), 0, 0
    psq_st f4, 0(r4), 0, 0
    psq_st f3, 0(r4), 0, 0
    psq_st f2, 0(r4), 0, 0
    psq_st f1, 0(r4), 0, 0
    psq_st f0, 0(r4), 0, 0

    li r0, 1
    lwz r3, __GXData
    sth r0, 2(r3)

    blr
    // clang-format on
}

asm void GXLoadLightObjIndx(register u32 index, register u32 id) {
    // clang-format off
    nofralloc

    cntlzw r0, id
    lis r5, WGPIPE@ha
    subfic r4, r0, 0x1f
    li r6, 0
    li r0, 0x38
    rlwinm r4, r4, 4, 0x19, 0x1b
    stb r0, WGPIPE@l(r5)
    addi r0, r4, 0x600
    lwz r4, __GXData
    rlwimi r6, r0, 0, 0x14, 0x1f
    ori r6, r6, 0xf000
    li r0, 1
    rlwimi r6, index, 0x10, 0, 0xf
    stw r6, WGPIPE@l(r5)
    sth r0, 2(r4)

    blr
    // clang-format on
}

void GXSetChanAmbColor(GXChannelID chan, GXColor color) {
    u32 ambColor;
    u32 colorId;

    switch (chan) {
    case GX_CHAN_RGB_0:
        ambColor =
            __rlwimi(*(u32*)&__GXData->ambColors[0], *(u32*)&color, 0, 0, 23);
        colorId = 0;
        break;
    case GX_CHAN_RGB_1:
        ambColor =
            __rlwimi(*(u32*)&__GXData->ambColors[1], *(u32*)&color, 0, 0, 23);
        colorId = 1;
        break;
    case GX_CHAN_ALPHA_0:
        ambColor = __rlwimi(*(u32*)&__GXData->ambColors[0], color.a, 0, 24, 31);
        colorId = 0;
        break;
    case GX_CHAN_ALPHA_1:
        ambColor = __rlwimi(*(u32*)&__GXData->ambColors[1], color.a, 0, 24, 31);
        colorId = 1;
        break;
    case GX_CHAN_RGBA_0:
        ambColor = *(u32*)&color;
        colorId = 0;
        break;
    case GX_CHAN_RGBA_1:
        ambColor = *(u32*)&color;
        colorId = 1;
        break;
    default:
        return;
    }

    __GXData->dirtyFlags |= 0x100 << colorId;
    *(u32*)&__GXData->ambColors[colorId] = ambColor;
}

void GXSetChanMatColor(GXChannelID chan, GXColor color) {
    u32 matColor;
    u32 colorId;

    switch (chan) {
    case GX_CHAN_RGB_0:
        matColor =
            __rlwimi(*(u32*)&__GXData->matColors[0], *(u32*)&color, 0, 0, 23);
        colorId = 0;
        break;
    case GX_CHAN_RGB_1:
        matColor =
            __rlwimi(*(u32*)&__GXData->matColors[1], *(u32*)&color, 0, 0, 23);
        colorId = 1;
        break;
    case GX_CHAN_ALPHA_0:
        matColor = __rlwimi(*(u32*)&__GXData->matColors[0], color.a, 0, 24, 31);
        colorId = 0;
        break;
    case GX_CHAN_ALPHA_1:
        matColor = __rlwimi(*(u32*)&__GXData->matColors[1], color.a, 0, 24, 31);
        colorId = 1;
        break;
    case GX_CHAN_RGBA_0:
        matColor = *(u32*)&color;
        colorId = 0;
        break;
    case GX_CHAN_RGBA_1:
        matColor = *(u32*)&color;
        colorId = 1;
        break;
    default:
        return;
    }

    __GXData->dirtyFlags |= 0x400 << colorId;
    *(u32*)&__GXData->matColors[colorId] = matColor;
}

void GXSetNumChans(u8 num) {
    GX_BITFIELD_SET(__GXData->WORD_0x254, 25, 3, num);
    __GXData->dirtyFlags |= 0x1000000;
    __GXData->dirtyFlags |= 0x4;
}

void GXSetChanCtrl(GXChannelID chan, u8 r4, GXColorSrc src0, GXColorSrc src1,
                   GXLightID light, GXDiffuseFn diffFn, GXAttnFn attnFn) {
    u32 field = 0;
    const u32 idx = chan & 3;

    GX_BITFIELD_SET(field, 30, 1, r4);
    GX_BITFIELD_SET(field, 31, 1, src1);
    GX_BITFIELD_SET(field, 25, 1, src0);
    GX_BITFIELD_SET(field, 23, 2,
                    (attnFn == GX_ATTN_FN_0) ? GX_DIFFUSE_FN_0 : diffFn);
    GX_BITFIELD_SET(field, 22, 1, attnFn != GX_ATTN_FN_2);
    GX_BITFIELD_SET(field, 21, 1, attnFn != GX_ATTN_FN_0);
    GX_BITFIELD_SET(field, 26, 4, (u32)light);
    GX_BITFIELD_SET(field, 17, 4, (u32)light >> 4);

    __GXData->WORDS_0xB8[idx] = field;
    __GXData->dirtyFlags |= (0x1000 << (idx));

    if (chan == GX_CHAN_RGBA_0) {
        __GXData->WORD_0xC0 = field;
        __GXData->dirtyFlags |= 0x5000;
    } else if (chan == GX_CHAN_RGBA_1) {
        __GXData->WORD_0xC4 = field;
        __GXData->dirtyFlags |= 0xA000;
    }
}

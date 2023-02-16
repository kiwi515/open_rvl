#include "RFL_Model.h"
#include "RFL_MakeTex.h"
#include "RFL_NANDLoader.h"

#include <OS.h>
#include <TRK/__mem.h>

#include <GX/GXAttr.h>
#include <GX/GXBump.h>
#include <GX/GXDisplayList.h>
#include <GX/GXGeometry.h>
#include <GX/GXMisc.h>
#include <GX/GXPixel.h>
#include <GX/GXTev.h>
#include <GX/GXTransform.h>
#include <GX/GXVert.h>

const RFLDrawCoreSetting cDefaultDrawCoreSetting2Tev = {
    .txcGenNum = 1,
    .txcID = GX_TEXCOORD0,
    .texMapID = GX_TEXMAP_0,
    .tevStageNum = 2,
    .tevSwapTable = 0,
    .tevKColorID = GX_KCOLOR0,
    .tevOutRegID = GX_TEVPREV,
    .mtxId = 0,
    .BYTE_0x20 = 0
    // TODO: UNK_0x21
};

const RFLDrawCoreSetting cDefaultDrawCoreSetting1Tev = {
    .txcGenNum = 1,
    .txcID = GX_TEXCOORD0,
    .texMapID = GX_TEXMAP_0,
    .tevStageNum = 1,
    .tevSwapTable = 0,
    .tevKColorID = GX_KCOLOR0,
    .tevOutRegID = GX_TEVPREV,
    .mtxId = 0,
    .BYTE_0x20 = 0
    // TODO: UNK_0x21
};

static const GXColor cFacelineColor[RFL_FACELINE_MAX] = {
    {240, 216, 196, 255}, //!< RFL_FACE_COLOR_0
    {255, 188, 128, 255}, //!< RFL_FACE_COLOR_1
    {216, 136, 80, 255},  //!< RFL_FACE_COLOR_2
    {255, 176, 144, 255}, //!< RFL_FACE_COLOR_3
    {152, 80, 48, 255},   //!< RFL_FACE_COLOR_4
    {82, 46, 28, 255},    //!< RFL_FACE_COLOR_5
};

static const GXColor cHairColor[RFL_HAIR_COLOR_MAX] = {
    {30, 26, 24, 255},    //!< RFL_HAIR_COLOR_0
    {56, 32, 21, 255},    //!< RFL_HAIR_COLOR_1
    {85, 38, 23, 255},    //!< RFL_HAIR_COLOR_2
    {112, 64, 36, 255},   //!< RFL_HAIR_COLOR_3
    {114, 114, 120, 255}, //!< RFL_HAIR_COLOR_4
    {73, 54, 26, 255},    //!< RFL_HAIR_COLOR_5
    {122, 89, 40, 255},   //!< RFL_HAIR_COLOR_6
    {193, 159, 100, 255}, //!< RFL_HAIR_COLOR_7
};

static const GXColor cBeardColor[RFL_BEARD_COLOR_MAX] = {
    {30, 26, 24, 255},    //!< RFL_BEARD_COLOR_0
    {56, 32, 21, 255},    //!< RFL_BEARD_COLOR_1
    {85, 38, 23, 255},    //!< RFL_BEARD_COLOR_2
    {112, 64, 36, 255},   //!< RFL_BEARD_COLOR_3
    {114, 114, 120, 255}, //!< RFL_BEARD_COLOR_4
    {73, 54, 26, 255},    //!< RFL_BEARD_COLOR_5
    {122, 89, 40, 255},   //!< RFL_BEARD_COLOR_6
    {193, 159, 100, 255}, //!< RFL_BEARD_COLOR_7
};

static const GXColor cGlassColor[RFL_GLASSES_COLOR_MAX] = {
    {16, 16, 16, 255},  //!< RFL_GLASSES_COLOR_BLACK
    {96, 56, 16, 255},  //!< RFL_GLASSES_COLOR_BROWN
    {152, 24, 16, 255}, //!< RFL_GLASSES_COLOR_RED
    {32, 48, 96, 255},  //!< RFL_GLASSES_COLOR_BLUE
    {144, 88, 0, 255},  //!< RFL_GLASSES_COLOR_ORANGE
    {96, 88, 80, 255},  //!< RFL_GLASSES_COLOR_GREY
};

static const GXColor cFavoriteColor[RFL_COLOR_MAX] = {
    {184, 64, 48, 255},   //!< RFL_COLOR_RED
    {240, 120, 40, 255},  //!< RFL_COLOR_ORANGE
    {248, 216, 32, 255},  //!< RFL_COLOR_YELLOW
    {128, 200, 40, 255},  //!< RFL_COLOR_LIME
    {0, 116, 40, 255},    //!< RFL_COLOR_GREEN
    {32, 72, 152, 255},   //!< RFL_COLOR_BLUE
    {64, 160, 216, 255},  //!< RFL_COLOR_AQUA
    {232, 96, 120, 255},  //!< RFL_COLOR_PINK
    {112, 44, 168, 255},  //!< RFL_COLOR_PURPLE
    {72, 56, 24, 255},    //!< RFL_COLOR_BROWN
    {224, 224, 224, 255}, //!< RFL_COLOR_WHITE
    {24, 24, 20, 255},    //!< RFL_COLOR_BLACK
};

static const GXColor cWhite = {255, 255, 255, 255};

#ifndef NON_MATCHING
#error RFLSetCoordinate has not yet been matched.
#endif
void RFLSetCoordinate(RFLCoordinateType t1, RFLCoordinateType t2) {
    ;
    ;
}

u32 RFLiGetExpressionNum(u32 exprFlags) {
    int i;
    u32 num = 0;

    for (i = 0; i < RFL_EXPR_MAX; i++) {
        if (exprFlags & (1 << i)) {
            num++;
        }
    }

    return num;
}

u32 RFLGetModelBufferSize(RFLResolution res, u32 expr) {
    const u32 exprNum = RFLiGetExpressionNum(expr);
    const s32 texSize = RFLiGetMaskBufSize(res);
    return ROUND_UP(exprNum * sizeof(GXTexObj), 32) + sizeof(RFLCharModelRes) +
           ROUND_UP(texSize * exprNum, 32);
}

RFLResult RFLInitCharModel(RFLCharModel* model, RFLDataSource src,
                           RFLMiddleDB* db, u16 id, void* work,
                           RFLResolution res, u32 expressions) {
    RFLCharInfo info;

    RFLResult result = RFLiPickupCharInfo(&info, src, db, id);
    if (result == RFL_RESULT_OK) {
        RFLiInitCharModel(model, &info, work, res, expressions);
    }

    return result;
}

RFLResult RFLiInitCharModel(RFLCharModel* model, RFLCharInfo* info, void* work,
                            RFLResolution res, u32 expressions) {
    RFLCharModel* model2;
    u32 maskSize;
    void* maskImages[RFL_EXPR_MAX];
    u32 maskRes;
    GXTexObj* exprTexObj;
    void* image;
    BOOL setExpr;
    f32 maxLod;
    u8 mipmap;
    int i;

    // Actually exists in debug ver. asm.
    // Perhaps model is actually a work buffer; however, asserts name it
    // "charModel", and g3d::ScnRfl passes in a RFLCharModel*.
    model2 = (RFLCharModel*)model;

    model2->resolution = res;
    maskSize = RFLiGetMaskBufSize(res);
    maskRes = RFLiGetMaxMaskRsl(res);
    model2->res = work;

    // Expression texobjs
    exprTexObj =
        (GXTexObj*)ROUND_UP_PTR((char*)work + sizeof(RFLCharModelRes), 32);
    for (i = 0; i < RFL_EXPR_MAX; i++) {
        if (expressions & (1 << i)) {
            model2->maskTexObj[i] = exprTexObj;
            exprTexObj++;
        } else {
            model2->maskTexObj[i] = NULL;
        }
    }

    // Expression images
    image = ROUND_UP_PTR(exprTexObj, 32);
    for (i = 0; i < RFL_EXPR_MAX; i++) {
        if (expressions & (1 << i)) {
            maskImages[i] = image;
            image = ((char*)image + maskSize);
        } else {
            maskImages[i] = NULL;
        }
    }

    RFLiInitCharModelRes(model2->res, info);

    setExpr = FALSE;
    mipmap = FALSE;
    maxLod = 0.0f;

    switch (res) {
    case RFL_RSL_96:
        maxLod = 1.0f;
        mipmap = TRUE;
        break;
    case RFL_RSL_224:
        maxLod = 2.0f;
        mipmap = TRUE;
        break;
    case RFL_RSL_480:
        maxLod = 3.0f;
        mipmap = TRUE;
        break;
    }

    for (i = 0; i < RFL_EXPR_MAX; i++) {
        if (model2->maskTexObj[i] != NULL) {
            GXInitTexObj(model2->maskTexObj[i], maskImages[i], maskRes, maskRes,
                         GX_TF_RGB5A3, GX_CLAMP, GX_CLAMP, mipmap);
            if (mipmap) {
                GXInitTexObjLOD(model2->maskTexObj[i], GX_LIN_MIP_LIN,
                                GX_LINEAR, 0.0f, maxLod, 0.0f, 0, 0,
                                GX_ANISO_1);
            } else {
                GXInitTexObjLOD(model2->maskTexObj[i], GX_LINEAR, GX_LINEAR,
                                0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
            }
            if (!setExpr) {
                model2->expression = (RFLExpression)i;
                setExpr = TRUE;
            }
        }
    }

    RFLiMakeTexture(info, maskImages, res);
}

void RFLSetMtx(RFLCharModel* model, Mtx modelView) {
    PSMTXCopy(modelView, model->viewMtx);
    PSMTXInvXpose(modelView, model->nrmMtx);
}

void RFLSetExpression(RFLCharModel* model, RFLExpression expr) {
    model->expression = expr;
}

RFLExpression RFLGetExpression(RFLCharModel* model) {
    return model->expression;
}

GXColor RFLGetFavoriteColor(RFLFavoriteColor color) {
    return cFavoriteColor[color];
}

GXColor RFLiGetFacelineColor(RFLCharInfo* info) {
    RFLFacelineColor color = RFL_FACE_COLOR_0;
    if ((info->SHORT_0x0 >> 10 & 7) < (u32)RFL_FACELINE_MAX) {
        color = (RFLFacelineColor)(info->SHORT_0x0 >> 10 & 7);
    }
    return cFacelineColor[color];
}

void RFLLoadDrawSetting(const RFLDrawSetting* setting) {
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_NEVER, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
    GXSetZMode(1, GX_LEQUAL, 1);
    GXSetZCompLoc(setting->compLoc);
    GXSetColorUpdate(1);
    GXSetAlphaUpdate(1);
    GXSetDither(0);
    GXSetDstAlpha(0, 0);

    if (setting->BOOL_0x0) {
        GXSetTevDirect(GX_TEVSTAGE1);
        GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_INVALID, GX_TEXMAP_NULL,
                      GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC,
                        GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_TEV_SCALE_0, 1,
                        GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
                        GX_CA_APREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_TEV_SCALE_0, 1,
                        GX_TEVPREV);

        RFLLoadMaterialSetting(&cDefaultDrawCoreSetting2Tev);
        RFLLoadVertexSetting(&cDefaultDrawCoreSetting2Tev);
        GXSetNumChans(1);

        GXSetChanCtrl(GX_COLOR0, 1, GX_SRC_REG, GX_SRC_REG, setting->lightID,
                      setting->diffuse, setting->attn);
        GXSetChanCtrl(GX_ALPHA0, 0, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
                      GX_DF_NONE, GX_AF_NONE);
        GXSetChanAmbColor(GX_COLOR0, setting->ambColor);
        GXSetChanMatColor(GX_COLOR0, cWhite);
    } else {
        RFLLoadMaterialSetting(&cDefaultDrawCoreSetting1Tev);
        RFLLoadVertexSetting(&cDefaultDrawCoreSetting1Tev);
        GXSetNumChans(0);
    }
}

void RFLDrawOpa(const RFLCharModel* model) {
    RFLDrawOpaCore(model, &cDefaultDrawCoreSetting2Tev);
}

void RFLDrawXlu(const RFLCharModel* model) {
    RFLDrawXluCore(model, &cDefaultDrawCoreSetting2Tev);
}

void RFLLoadVertexSetting(const RFLDrawCoreSetting* setting) {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_VA_TEX1MTXIDX);
    GXSetVtxDesc(GX_VA_NRM, GX_VA_TEX1MTXIDX);
    GXSetVtxDesc(GX_VA_TEX0, GX_VA_TEX1MTXIDX);
    GXSetVtxAttrFmt(0, GX_VA_POS, 1, 3, 8);
    GXSetVtxAttrFmt(0, GX_VA_NRM, 0, 3, 14);
    GXSetVtxAttrFmt(0, GX_VA_TEX0, 1, 3, 13);
    GXSetNumTexGens(setting->txcGenNum);
}

void RFLLoadMaterialSetting(const RFLDrawCoreSetting* setting) {
    GXSetTevSwapModeTable(setting->tevSwapTable, GX_CH_RED, GX_CH_GREEN,
                          GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(setting->tevSwapTable + 1, GX_CH_RED, GX_CH_ALPHA,
                          GX_CH_BLUE, GX_CH_GREEN);
    GXSetNumTevStages(setting->tevStageNum);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_TEV_SCALE_0, 1,
                    setting->tevOutRegID);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_TEV_SCALE_0, 1,
                    setting->tevOutRegID);
    GXSetTevKColorSel(GX_TEVSTAGE0,
                      (GXTevKColorSel)(setting->tevKColorID + 12));
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_8_8);
}

void RFLDrawOpaCore(const RFLCharModel* model,
                    const RFLDrawCoreSetting* setting) {
    RFLCharModelRes* res = model->res;

    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
                    GX_CA_KONST);

    // Copy-paste error?
    GXSetTevSwapMode(GX_TEVSTAGE0, setting->tevSwapTable,
                     setting->tevSwapTable);
    GXSetTevSwapMode(GX_TEVSTAGE0, setting->tevSwapTable,
                     setting->tevSwapTable);

    GXSetCullMode(setting->BYTE_0x20 ? GX_CULL_FRONT : GX_CULL_BACK);

    GXLoadPosMtxImm(model->viewMtx, setting->mtxId);
    GXLoadNrmMtxImm(model->nrmMtx, setting->mtxId);
    GXSetCurrentMtx(setting->mtxId);

    GXSetTexCoordGen(setting->txcID, GX_TG_MTX2x4, GX_TG_POS, 0x3C);
    GXSetVtxDesc(GX_VA_TEX0, GX_VA_PNMTXIDX);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_INVALID, GX_TEXMAP_NULL,
                  GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
                    GX_CC_KONST);

    if (res->beardDlSize > 0) {
        GXSetTevKColor(setting->tevKColorID, cBeardColor[res->beardColor]);
        GXSetArray(GX_VA_POS, res->beardVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->beardVtxNrm, 6);
        GXCallDisplayList(res->beardDl, res->beardDlSize);
    }

    GXSetTevKColor(setting->tevKColorID, cFacelineColor[res->facelineColor]);

    if (res->noseDlSize > 0) {
        GXSetArray(GX_VA_POS, res->noseVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->noseVtxNrm, 6);
        GXCallDisplayList(res->noseDl, res->noseDlSize);
    }

    if (res->WORD_0x824C != 0) {
        GXSetCullMode(setting->BYTE_0x20 ? GX_CULL_BACK : GX_CULL_FRONT);
    }

    if (res->foreheadDlSize > 0) {
        GXSetArray(GX_VA_POS, res->foreheadVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->foreheadVtxNrm, 6);
        GXCallDisplayList(res->foreheadDl, res->foreheadDlSize);
    }

    if (res->hairDlSize > 0) {
        GXSetTevKColor(setting->tevKColorID, cHairColor[res->hairColor]);
        GXSetArray(GX_VA_POS, res->hairVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->hairVtxNrm, 6);
        GXCallDisplayList(res->hairDl, res->hairDlSize);
    }

    GXSetTevOrder(GX_TEVSTAGE0, setting->txcID, setting->texMapID,
                  GX_COLOR_NULL);
    GXSetTexCoordGen(setting->txcID, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C);
    GXSetVtxDesc(GX_VA_TEX0, GX_VA_TEX1MTXIDX);

    if (res->capDlSize > 0) {
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_KONST, GX_CC_TEXC,
                        GX_CC_KONST);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_TEV_SCALE_3, 1,
                        setting->tevOutRegID);
        GXSetTevKColor(setting->tevKColorID,
                       cFavoriteColor[res->favoriteColor]);

        GXLoadTexObj(&res->capTexObj, setting->texMapID);
        GXSetArray(GX_VA_POS, res->capVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->capVtxNrm, 6);
        GXSetArray(GX_VA_TEX0, res->capVtxTxc, 4);
        GXCallDisplayList(res->capDl, res->capDlSize);

        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_TEV_SCALE_0, 1,
                        setting->tevOutRegID);
    }

    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_KONST, GX_CC_TEXC, GX_CC_TEXA,
                    GX_CC_ZERO);
    GXSetTevKColor(setting->tevKColorID, cFacelineColor[res->facelineColor]);
    GXSetTevSwapMode(GX_TEVSTAGE0, setting->tevSwapTable,
                     (GXTevSwapSel)(setting->tevSwapTable + 1));

    if (res->WORD_0x824C != 0) {
        GXSetCullMode(setting->BYTE_0x20 ? GX_CULL_FRONT : GX_CULL_BACK);
    }

    GXLoadTexObj(&res->mascaraTexObj, setting->texMapID);
    GXSetArray(GX_VA_POS, res->faceVtxPos, 6);
    GXSetArray(GX_VA_NRM, res->faceVtxNrm, 6);
    GXSetArray(GX_VA_TEX0, res->faceVtxTxc, 4);
    GXCallDisplayList(res->faceDl, res->faceDlSize);
}

void RFLDrawXluCore(const RFLCharModel* model,
                    const RFLDrawCoreSetting* setting) {
    RFLCharModelRes* res = model->res;

    GXSetTevOrder(GX_TEVSTAGE0, setting->txcID, setting->texMapID,
                  GX_COLOR_NULL);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
                    GX_CA_TEXA);
    GXSetTevSwapMode(GX_TEVSTAGE0, setting->tevSwapTable,
                     setting->tevSwapTable);

    GXLoadPosMtxImm(model->viewMtx, setting->mtxId);
    GXLoadNrmMtxImm(model->nrmMtx, setting->mtxId);
    GXSetCurrentMtx(setting->mtxId);

    GXSetTexCoordGen(setting->txcID, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
                    GX_CC_TEXC);
    GXSetCullMode(setting->BYTE_0x20 ? GX_CULL_FRONT : GX_CULL_BACK);

    GXLoadTexObj(model->maskTexObj[model->expression], setting->texMapID);
    GXSetArray(GX_VA_POS, res->maskVtxPos, 6);
    GXSetArray(GX_VA_NRM, res->maskVtxNrm, 6);
    GXSetArray(GX_VA_TEX0, res->maskVtxTxc, 4);
    GXCallDisplayList(res->maskDl, res->maskDlSize);

    if (res->noselineDlSize > 0) {
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
                        GX_CC_ZERO);
        GXLoadTexObj(&res->noseTexObj, setting->texMapID);
        GXSetArray(GX_VA_POS, res->noselineVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->noselineVtxNrm, 6);
        GXSetArray(GX_VA_TEX0, res->noselineVtxTxc, 4);
        GXCallDisplayList(res->noselineDl, res->noselineDlSize);
    }

    if (res->glassesDlSize > 0) {
        GXSetTevKColor(setting->tevKColorID, cGlassColor[res->glassesColor]);
        GXSetCullMode(GX_CULL_NONE);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_KONST, GX_CC_TEXC,
                        GX_CC_ZERO);
        GXLoadTexObj(&res->glassesTexObj, setting->texMapID);
        GXSetArray(GX_VA_POS, res->glassesVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->glassesVtxNrm, 6);
        GXSetArray(GX_VA_TEX0, res->glassesVtxTxc, 4);
        GXCallDisplayList(res->glassesDl, res->glassesDlSize);
    }
}

#ifndef NON_MATCHING
#error RFLiInitCharModelRes has not yet been matched.
#endif
void RFLiInitCharModelRes(RFLCharModelRes* res, RFLCharInfo* info) {
    ;
    ;
}

#ifndef NON_MATCHING
#error RFLiInitShapeRes has not yet been matched. (https://decomp.me/scratch/eYP1o)
#endif
/**
 * https://wiki.tockdom.com/wiki/RFL_Res.dat_(File_Format)
 * See "Model format"
 * The format changes based on the model type, so the code is a little messy.
 */
void RFLiInitShapeRes(RFLShape* shape) {
    static const u32 csHeader[RFL_PART_SHP_MAX] = {
        'nose', 'frhd', 'face', 'hair', 'cap_', 'berd', 'nsln', 'mask', 'glas'};

    u8 indices;
    s32 vtxPosSize;
    s16* vtxPtr;
    s32 vtxNrmSize;
    s32 x2;
    GXPrimitive prim;
    int i;
    int j;
    s32 x3;
    s32 x1;
    s32 x4;
    u8 dlSize;
    s16* nrmPtr;

    BOOL noTxc = (((u32)shape->part) <= RFL_PART_SHP_BEARD) &&
                 (((1 << shape->part) & 0x2B) != 0);
    u8* res = (u8*)RFLiAlloc32(RFLiGetShapeSize(shape->part, shape->file));
    RFLiLoadShape(shape->part, shape->file, res);

    res += sizeof(u32);

    if (shape->part == RFL_PART_SHP_FACE) {
        memcpy(shape->noseTrans, res, sizeof(Vec));
        res += sizeof(Vec);
        memcpy(shape->beardTrans, res, sizeof(Vec));
        res += sizeof(Vec);
        memcpy(shape->hairTrans, res, sizeof(Vec));
        res += sizeof(Vec);
    }

    if (*(u16*)res == 0) {
        shape->numVtxPos = 0;
        shape->numVtxNrm = 0;
        shape->numVtxTxc = 0;
        shape->dlSize = 0;
        RFLiFree(res);
        return;
    }

    shape->numVtxPos = *(u16*)res;
    vtxPosSize = shape->numVtxPos * (sizeof(s16[3]));
    res += sizeof(u16);
    vtxPtr = (s16*)res;

    if (shape->BOOL_0x8 != 0) {
        x1 = 256.0f * shape->FLOAT_0x30;
        x2 = 256.0f * shape->posTrans->x;
        x3 = 256.0f * shape->posTrans->y;
        x4 = 256.0f * shape->posTrans->z;
        for (i = 0; i < shape->numVtxPos; i++) {
            const s16 coord[3] = {
                (shape->BOOL_0xC != 0) ? (s16)((((-vtxPtr[0]) * x1) >> 8) + x2)
                                       : (s16)(((vtxPtr[0] * x1) >> 8) + x2),
                ((vtxPtr[1] * x1) >> 8) + x3, ((vtxPtr[2] * x1) >> 8) + x4};
            RFLiTransformCoordinate(shape->vtxPosBuf + (i * 3), coord);
            vtxPtr += 3;
        }

    } else if (shape->BOOL_0xC != 0) {
        for (i = 0; i < shape->numVtxPos; i++) {
            const s16 coord[3] = {-vtxPtr[0], vtxPtr[1], vtxPtr[2]};
            RFLiTransformCoordinate(shape->vtxPosBuf + (i * 3), coord);
            vtxPtr += 3;
        }

    } else {
        for (i = 0; i < shape->numVtxPos; i++) {
            RFLiTransformCoordinate(shape->vtxPosBuf + (i * 3), vtxPtr);
            vtxPtr += 3;
        }
    }

    res += vtxPosSize;
    shape->numVtxNrm = *(u16*)res;
    res += sizeof(u16);
    vtxNrmSize = shape->numVtxNrm * sizeof(s16[3]);
    nrmPtr = (s16*)res;

    if (shape->BOOL_0xC != 0) {
        for (i = 0; i < shape->numVtxNrm; i++) {
            const s16 coord[3] = {-nrmPtr[0], nrmPtr[1], nrmPtr[2]};
            RFLiTransformCoordinate(shape->vtxNrmBuf + (i * 3), coord);
            nrmPtr += 3;
        }

    } else {
        for (i = 0; i < shape->numVtxNrm; i++) {
            RFLiTransformCoordinate(shape->vtxNrmBuf + (i * 3), nrmPtr);
            nrmPtr += 3;
        }
    }

    res += vtxNrmSize;

    if (noTxc) {
        shape->numVtxTxc = 0;
    } else {
        shape->numVtxTxc = *(u16*)res;
        const size_t len = shape->numVtxTxc * sizeof(s16[2]);
        res += sizeof(u16);
        memcpy(shape->vtxTxcBuf, (u16*)res, len);
        res += len;
    }

    dlSize = *(u8*)res;
    res += sizeof(u8);
    DCInvalidateRange(shape->dlBuf, shape->dlBufSize);
    GXBeginDisplayList(shape->dlBuf, shape->dlBufSize);
    for (i = 0; i < dlSize; i++) {
        indices = *(u8*)res;
        res += sizeof(u8);
        prim = (GXPrimitive)(*(u8*)res);
        res += sizeof(u8);
        GXBegin(prim, 0, indices);
        for (j = 0; j < indices; j++) {
            GXPosition1x8(*(u8*)res);
            res += sizeof(u8);
            GXNormal1x8(*(u8*)res);
            res += sizeof(u8);

            if (!noTxc) {
                GXTexCoord1x8(*(u8*)res);
                res += sizeof(u8);
            }
        }

        GXEnd();
    }

    shape->dlSize = GXEndDisplayList();
    RFLiFree(res);
}

void RFLiInitTexRes(GXTexObj* texObj, RFLPartShpTex part, u16 file,
                    void* buffer) {
    u32 texSize;
    RFLTexHeader* tex;

    tex = (RFLTexHeader*)RFLiAlloc32(RFLiGetShpTexSize(part, file));
    RFLiLoadShpTexture(part, file, tex);

    switch (part) {
    case RFL_PART_SHP_TEX_MASCARA:
        texSize = tex->height * tex->width * 2;
        break;
    case RFL_PART_SHP_TEX_CAP:
    case RFL_PART_SHP_TEX_NOSE:
        texSize = tex->height * tex->width / 2;
        break;
    case RFL_PART_SHP_TEX_GLASSES:
        texSize = tex->height * tex->width;
        break;
    }

    memcpy(buffer, RFLiGetTexImage(tex), texSize);
    GXInitTexObj(texObj, buffer, tex->width, tex->height, tex->format,
                 tex->wrapS, tex->wrapT, 0);
    GXInitTexObjLOD(texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, 0, 0,
                    GX_ANISO_1);
    RFLiFree(tex);
}

void RFLiTransformCoordinate(s16* to, const s16* from) {
    to[coordinateData.BYTE_0x2] =
        (coordinateData.WORD_0xC != 0) ? -from[0] : from[0];
    to[coordinateData.BYTE_0x0] =
        (coordinateData.WORD_0x4 != 0) ? -from[1] : from[1];
    to[coordinateData.BYTE_0x1] =
        (coordinateData.WORD_0x8 != 0) ? -from[2] : from[2];
}

void RFLDrawShape(const RFLCharModel* model) {
    GXCullMode cullMode;
    RFLCharModelRes* res = model->res;

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_VA_TEX1MTXIDX);
    GXSetVtxDesc(GX_VA_NRM, GX_VA_TEX1MTXIDX);
    GXSetVtxAttrFmt(0, GX_VA_POS, 1, 3, 8);
    GXSetVtxAttrFmt(0, GX_VA_NRM, 0, 3, 14);
    GXSetVtxAttrFmt(0, GX_VA_TEX0, 1, 3, 13);

    GXLoadPosMtxImm(model->viewMtx, 0);
    GXLoadNrmMtxImm(model->nrmMtx, 0);
    GXSetCurrentMtx(0);

    if (res->beardDlSize > 0) {
        GXSetArray(GX_VA_POS, res->beardVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->beardVtxNrm, 6);
        GXCallDisplayList(res->beardDl, res->beardDlSize);
    }

    if (res->noseDlSize > 0) {
        GXSetArray(GX_VA_POS, res->noseVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->noseVtxNrm, 6);
        GXCallDisplayList(res->noseDl, res->noseDlSize);
    }

    if (res->WORD_0x824C > 0) {
        GXGetCullMode(&cullMode);
        switch (cullMode) {
        case GX_CULL_BACK:
            GXSetCullMode(GX_CULL_FRONT);
            break;
        case GX_CULL_FRONT:
            GXSetCullMode(GX_CULL_BACK);
            break;
        }
    }

    if (res->foreheadDlSize > 0) {
        GXSetArray(GX_VA_POS, res->foreheadVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->foreheadVtxNrm, 6);
        GXCallDisplayList(res->foreheadDl, res->foreheadDlSize);
    }

    if (res->hairDlSize > 0) {
        GXSetArray(GX_VA_POS, res->hairVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->hairVtxNrm, 6);
        GXCallDisplayList(res->hairDl, res->hairDlSize);
    }

    GXSetVtxDesc(GX_VA_TEX0, GX_VA_TEX1MTXIDX);
    if (res->capDlSize > 0) {
        GXSetArray(GX_VA_POS, res->capVtxPos, 6);
        GXSetArray(GX_VA_NRM, res->capVtxNrm, 6);
        GXSetArray(GX_VA_TEX0, res->capVtxTxc, 4);
        GXCallDisplayList(res->capDl, res->capDlSize);
    }

    if (res->WORD_0x824C > 0) {
        switch (cullMode) {
        case GX_CULL_BACK:
            GXSetCullMode(GX_CULL_BACK);
            break;
        case GX_CULL_FRONT:
            GXSetCullMode(GX_CULL_FRONT);
            break;
        }
    }

    GXSetArray(GX_VA_POS, res->faceVtxPos, 6);
    GXSetArray(GX_VA_NRM, res->faceVtxNrm, 6);
    GXSetArray(GX_VA_TEX0, res->faceVtxTxc, 4);
    GXCallDisplayList(res->faceDl, res->faceDlSize);
}

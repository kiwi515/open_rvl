#ifndef RVL_SDK_RFL_MODEL_H
#define RVL_SDK_RFL_MODEL_H
#include <revolution/GX/GXLight.h>
#include <revolution/GX/GXTexture.h>
#include <revolution/MTX/mtx.h>
#include <revolution/RFL/RFL_DataUtility.h>
#include <revolution/RFL/RFL_MiddleDatabase.h>
#include <revolution/RFL/RFL_System.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RFL_COORDINATE_TYPE_0,
} RFLCoordinateType;

typedef enum {
    RFL_EXPR_0,
    RFL_EXPR_1,
    RFL_EXPR_2,
    RFL_EXPR_3,
    RFL_EXPR_4,
    RFL_EXPR_5,
    RFL_EXPR_6,

    RFL_EXPR_MAX
} RFLExpression;

typedef enum {
    RFL_FACE_COLOR_0,
    RFL_FACE_COLOR_1,
    RFL_FACE_COLOR_2,
    RFL_FACE_COLOR_3,
    RFL_FACE_COLOR_4,
    RFL_FACE_COLOR_5,

    RFL_FACELINE_MAX
} RFLFacelineColor;

typedef enum {
    RFL_HAIR_COLOR_0,
    RFL_HAIR_COLOR_1,
    RFL_HAIR_COLOR_2,
    RFL_HAIR_COLOR_3,
    RFL_HAIR_COLOR_4,
    RFL_HAIR_COLOR_5,
    RFL_HAIR_COLOR_6,
    RFL_HAIR_COLOR_7,

    RFL_HAIR_COLOR_MAX
} RFLHairColor;

typedef enum {
    RFL_BEARD_COLOR_0,
    RFL_BEARD_COLOR_1,
    RFL_BEARD_COLOR_2,
    RFL_BEARD_COLOR_3,
    RFL_BEARD_COLOR_4,
    RFL_BEARD_COLOR_5,
    RFL_BEARD_COLOR_6,
    RFL_BEARD_COLOR_7,

    RFL_BEARD_COLOR_MAX
} RFLBeardColor;

typedef enum {
    RFL_GLASSES_COLOR_BLACK,
    RFL_GLASSES_COLOR_BROWN,
    RFL_GLASSES_COLOR_RED,
    RFL_GLASSES_COLOR_BLUE,
    RFL_GLASSES_COLOR_ORANGE,
    RFL_GLASSES_COLOR_GREY,

    RFL_GLASSES_COLOR_MAX
} RFLGlassesColor;

typedef enum {
    RFL_COLOR_RED,
    RFL_COLOR_ORANGE,
    RFL_COLOR_YELLOW,
    RFL_COLOR_LIME,
    RFL_COLOR_GREEN,
    RFL_COLOR_BLUE,
    RFL_COLOR_AQUA,
    RFL_COLOR_PINK,
    RFL_COLOR_PURPLE,
    RFL_COLOR_BROWN,
    RFL_COLOR_WHITE,
    RFL_COLOR_BLACK,

    RFL_COLOR_MAX
} RFLFavoriteColor;

typedef enum {
    RFL_RSL_64 = 64,
    RFL_RSL_96 = 96,
    RFL_RSL_128 = 128,
    RFL_RSL_224 = 224,
    RFL_RSL_256 = 256,
    RFL_RSL_480 = 480
} RFLResolution;

typedef struct RFLCoordinateData {
    u8 BYTE_0x0;
    u8 BYTE_0x1;
    u8 BYTE_0x2;
    u8 BYTE_0x3;
    u32 WORD_0x4;
    u32 WORD_0x8;
    u32 WORD_0xC;
} RFLCoordinateData;

typedef struct RFLCharModelRes {
    u8 noseDl[0xC0];     // at 0x0
    u8 capDl[0x560];     // at 0xC0
    u8 faceDl[0x2E0];    // at 0x620
    u8 beardDl[0x160];   // at 0x900
    u8 noselineDl[0x60]; // at 0xA60
    u8 maskDl[0x380];    // at 0xAC0
    u8 glassesDl[0x40];  // at 0xE40

    u8 mascaraTex[0x4000]; // at 0xE80
    u8 capTex[0x400];      // at 0x4E80
    u8 noseTex[0x400];     // at 0x5280
    u8 glassesTex[0x1000]; // at 0x5680

    u8 noseVtxPos[23 * sizeof(s16[3])]; // at 0x6680
    u8 noseVtxNrm[23 * sizeof(s16[3])]; // at 0x670A

    u8 capVtxPos[173 * sizeof(s16[3])]; // at 0x6794
    u8 capVtxNrm[246 * sizeof(s16[3])]; // at 0x6BA2
    u8 capVtxTxc[95 * sizeof(s16[2])];  // at 0x7166

    u8 faceVtxPos[66 * sizeof(s16[3])];  // at 0x72E2
    u8 faceVtxNrm[66 * sizeof(s16[3])];  // at 0x746E
    u8 faceVtxTxc[115 * sizeof(s16[2])]; // at 0x75FA

    u8 beardVtxPos[40 * sizeof(s16[3])]; // at 0x77C6
    u8 beardVtxNrm[68 * sizeof(s16[3])]; // at 0x78B6

    u8 noselineVtxPos[6 * sizeof(s16[3])]; // at 0x7A4E
    u8 noselineVtxNrm[2 * sizeof(s16[3])]; // at 0x7A72
    u8 noselineVtxTxc[7 * sizeof(s16[2])]; // at 0x7A7E

    u8 maskVtxPos[88 * sizeof(s16[3])];  // at 0x7A9A
    u8 maskVtxNrm[86 * sizeof(s16[3])];  // at 0x7CAA
    u8 maskVtxTxc[176 * sizeof(s16[2])]; // at 0x7EAE

    u8 glassesVtxPos[4 * sizeof(s16[3])]; // at 0x816E
    u8 glassesVtxNrm[1 * sizeof(s16[3])]; // at 0x8186
    u8 glassesVtxTxc[4 * sizeof(s16[2])]; // at 0x818C

    GXTexObj mascaraTexObj; // at 0x819C
    GXTexObj capTexObj;     // at 0x81BC
    GXTexObj noseTexObj;    // at 0x81DC
    GXTexObj glassesTexObj; // at 0x81FC

    s16* hairVtxPos;     // at 0x821C
    s16* hairVtxNrm;     // at 0x8220
    u8* hairDl;          // at 0x8224
    s16* foreheadVtxPos; // at 0x8228
    s16* foreheadVtxNrm; // at 0x822C
    u8* foreheadDl;      // at 0x8230

    u16 noseDlSize;     // at 0x8234
    u16 faceDlSize;     // at 0x8236
    u16 hairDlSize;     // at 0x8238
    u16 capDlSize;      // at 0x823A
    u16 foreheadDlSize; // at 0x823C
    u16 beardDlSize;    // at 0x823E
    u16 noselineDlSize; // at 0x8240
    u16 maskDlSize;     // at 0x8242
    u16 glassesDlSize;  // at 0x8244

    u8 facelineColor; // at 0x8246
    u8 hairColor;     // at 0x8247
    u8 beardColor;    // at 0x8248
    u8 glassesColor;  // at 0x8249
    u8 favoriteColor; // at 0x824A

    u8 BYTE_0x824B;
    u32 WORD_0x824C;
    char UNK_0x8250[0x8260 - 0x8250];
} RFLCharModelRes;

typedef struct RFLCharModel {
    Mtx viewMtx;                        // at 0x0
    Mtx nrmMtx;                         // at 0x30
    RFLExpression expression;           // at 0x60
    RFLResolution resolution;           // at 0x64
    RFLCharModelRes* res;               // at 0x68
    GXTexObj* maskTexObj[RFL_EXPR_MAX]; // at 0x6C
} RFLCharModel;

typedef struct RFLShape {
    RFLPartShp part; // at 0x0
    u16 file;        // at 0x4
    BOOL BOOL_0x8;
    BOOL BOOL_0xC;
    s16* vtxPosBuf;    // at 0x10
    s16* vtxNrmBuf;    // at 0x14
    s16* vtxTxcBuf;    // at 0x18
    void* dlBuf;       // at 0x1C
    u16 vtxPosBufSize; // at 0x20
    u16 vtxNrmBufSize; // at 0x22
    u16 vtxTxcBufSize; // at 0x24
    u16 dlBufSize;     // at 0x26
    u16 numVtxPos;     // at 0x28
    u16 numVtxNrm;     // at 0x2A
    u16 numVtxTxc;     // at 0x2C
    u16 dlSize;        // at 0x2E
    f32 FLOAT_0x30;
    Vec* posTrans;   // at 0x34
    Vec* noseTrans;  // at 0x38
    Vec* beardTrans; // at 0x3C
    Vec* hairTrans;  // at 0x40
} RFLShape;

typedef struct RFLDrawSetting {
    u8 BOOL_0x0;
    GXLightID lightID;   // at 0x4
    GXDiffuseFn diffuse; // at 0x8
    GXAttnFn attn;       // at 0xC
    GXColor ambColor;    // at 0x10
    u8 compLoc;          // at 0x14
} RFLDrawSetting;

/**
 * Documentation from:
 * https://wiki.tockdom.com/wiki/BTI_(File_Format)
 */
typedef struct RFLTexHeader {
    u8 format;       // at 0x0
    u8 alpha;        // at 0x1
    u16 width;       // at 0x2
    u16 height;      // at 0x4
    u8 wrapS;        // at 0x6
    u8 wrapT;        // at 0x7
    u16 paletteFmt;  // at 0x8
    u16 numPalettes; // at 0xA
    u32 paletteOfs;  // at 0xC
    char UNK_0x10[0x4];
    u8 magFilt; // at 0x14
    u8 minFilt; // at 0x15
    char UNK_0x16[0x2];
    u8 numImages; // at 0x18
    char UNK_0x19[0x3];
    u32 imageOfs; // at 0x1C
} RFLTexHeader;

typedef struct RFLDrawCoreSetting {
    u8 txcGenNum;              // at 0x0
    GXTexCoordID txcID;        // at 0x4
    GXTexMapID texMapID;       // at 0x8
    u8 tevStageNum;            // at 0xC
    GXTevSwapSel tevSwapTable; // at 0x10
    GXTevKColorID tevKColorID; // at 0x14
    GXTevRegID tevOutRegID;    // at 0x18
    u32 mtxId;                 // at 0x1C
    u8 BYTE_0x20;
    char UNK_0x21[0x3];
} RFLDrawCoreSetting;

extern RFLCoordinateData coordinateData;

static inline void RFLiSetCoordinateData(const RFLCoordinateData* data) {
    coordinateData = *data;
}

static inline const void* RFLiGetTexImage(const RFLTexHeader* tex) {
    return (char*)tex + tex->imageOfs;
}

void RFLSetCoordinate(RFLCoordinateType, RFLCoordinateType);
u32 RFLiGetExpressionNum(u32);
u32 RFLGetModelBufferSize(RFLResolution, u32);
RFLResult RFLInitCharModel(RFLCharModel*, RFLDataSource, RFLMiddleDB*, u16,
                           void*, RFLResolution, u32);
RFLResult RFLiInitCharModel(RFLCharModel*, RFLCharInfo*, void*, RFLResolution,
                            u32);
void RFLSetMtx(RFLCharModel*, Mtx);
void RFLSetExpression(RFLCharModel*, RFLExpression);
RFLExpression RFLGetExpression(RFLCharModel*);
GXColor RFLGetFavoriteColor(RFLFavoriteColor);
GXColor RFLiGetFacelineColor(RFLCharInfo*);
void RFLLoadDrawSetting(const RFLDrawSetting*);
void RFLDrawOpa(const RFLCharModel*);
void RFLDrawXlu(const RFLCharModel*);
void RFLLoadVertexSetting(const RFLDrawCoreSetting*);
void RFLLoadMaterialSetting(const RFLDrawCoreSetting*);
void RFLDrawOpaCore(const RFLCharModel*, const RFLDrawCoreSetting*);
void RFLDrawXluCore(const RFLCharModel*, const RFLDrawCoreSetting*);
void RFLiInitCharModelRes(RFLCharModelRes*, RFLCharInfo*);
void RFLiInitShapeRes(RFLShape*);
void RFLiInitTexRes(GXTexObj*, RFLPartShpTex, u16, void*);
void RFLiTransformCoordinate(s16*, const s16*);
void RFLDrawShape(const RFLCharModel*);

#ifdef __cplusplus
}
#endif
#endif

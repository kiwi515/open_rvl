#ifndef RVL_FACE_LIBRARY_INTERNAL_MODEL_H
#define RVL_FACE_LIBRARY_INTERNAL_MODEL_H
#include <RVLFaceLib/RFL_Model.h>
#include <RVLFaceLib/RFLi_Types.h>
#include <revolution/GX.h>
#include <revolution/MTX.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct RFLiCoordinateData {
    u8 uOff;   // at 0x0
    u8 fOff;   // at 0x1
    u8 rOff;   // at 0x2
    BOOL uRev; // at 0x4
    BOOL fRev; // at 0x8
    BOOL rRev; // at 0xC
} RFLiCoordinateData;

typedef struct RFLiCharModelRes {
    u8 noseDl[0xC0];     // at 0x0
    u8 capDl[0x560];     // at 0xC0
    u8 faceDl[0x2E0];    // at 0x620
    u8 beardDl[0x160];   // at 0x900
    u8 noselineDl[0x60]; // at 0xA60
    u8 maskDl[0x380];    // at 0xAC0
    u8 glassesDl[0x40];  // at 0xE40

    u8 faceTex[0x4000];    // at 0xE80
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

    GXTexObj faceTexObj;    // at 0x819C
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

    BOOL flipHair; // at 0x824C
} RFLiCharModelRes;

typedef struct RFLiShapeRes {
    RFLiPartsShp part; // at 0x0
    u16 file;          // at 0x4
    BOOL transform;    // at 0x8
    BOOL flipX;        // at 0xC
    s16* vtxPosBuf;    // at 0x10
    s16* vtxNrmBuf;    // at 0x14
    s16* vtxTxcBuf;    // at 0x18
    u8* dlBuf;         // at 0x1C
    u16 vtxPosBufSize; // at 0x20
    u16 vtxNrmBufSize; // at 0x22
    u16 vtxTxcBufSize; // at 0x24
    u16 dlBufSize;     // at 0x26
    u16 numVtxPos;     // at 0x28
    u16 numVtxNrm;     // at 0x2A
    u16 numVtxTxc;     // at 0x2C
    u16 dlSize;        // at 0x2E
    f32 posScale;
    Vec* posTrans;   // at 0x34
    Vec* noseTrans;  // at 0x38
    Vec* beardTrans; // at 0x3C
    Vec* hairTrans;  // at 0x40
} RFLiShapeRes;

typedef struct RFLiCharModel {
    Mtx posMtx;                       // at 0x0
    Mtx nrmMtx;                       // at 0x30
    RFLExpression expression;         // at 0x60
    RFLResolution resolution;         // at 0x64
    RFLiCharModelRes* res;            // at 0x68
    GXTexObj* maskTexObj[RFLExp_Max]; // at 0x6C
} RFLiCharModel;

typedef struct RFLiCharInfo {
    union {
        struct {
            u16 type : 3;
            u16 color : 3;
            u16 texture : 4;
            u16 padding : 6;
        };
        u16 rawdata;
    } faceline; // at 0x0

    union {
        struct {
            u16 type : 7;
            u16 color : 3;
            u16 flip : 1;
            u16 padding : 5;
        };
        u16 rawdata;
    } hair; // at 0x2

    union {
        struct {
            u32 type : 6;
            u32 color : 3;
            u32 scale : 4;
            u32 rotate : 5;
            u32 x : 4;
            u32 y : 5;
            u32 padding : 5;
        };
        u32 rawdata;
    } eye; // at 0x4

    union {
        struct {
            u32 type : 5;
            u32 color : 3;
            u32 scale : 4;
            u32 rotate : 5;
            u32 x : 4;
            u32 y : 5;
            u32 padding : 6;
        };
        u32 rawdata;
    } eyebrow; // at 0x8

    union {
        struct {
            u16 type : 4;
            u16 scale : 4;
            u16 y : 5;
            u16 padding : 3;
        };
        u16 rawdata;
    } nose; // at 0xC

    union {
        struct {
            u16 type : 5;
            u16 color : 2;
            u16 scale : 4;
            u16 y : 5;
        };
        u16 rawdata;
    } mouth; // at 0xE

    union {
        struct {
            u16 mustache : 2;
            u16 type : 2;
            u16 color : 3;
            u16 scale : 4;
            u16 y : 5;
        };
        u16 rawdata;
    } beard; // at 0x10

    union {
        struct {
            u16 type : 4;
            u16 color : 3;
            u16 scale : 4;
            u16 y : 5;
        };
        u16 rawdata;
    } glass; // at 0x12

    union {
        struct {
            u16 type : 1;
            u16 scale : 4;
            u16 x : 5;
            u16 y : 5;
            u16 padding : 1;
        };
        u16 rawdata;
    } mole; // at 0x14

    struct {
        u8 height;
        u8 build;
    } body; // at 0x16

    struct {
        wchar_t name[RFL_NAME_LEN + 1];
        wchar_t creator[RFL_CREATOR_LEN + 1];
        u16 sex : 1;
        u16 bmonth : 4;
        u16 bday : 5;
        u16 color : 4;
        u16 favorite : 1;
        u16 localOnly : 1;
    } personal; // at 0x18

    struct {
        u8 data[RFL_CREATEID_LEN];
    } createID; // at 0x46
} RFLiCharInfo;

extern RFLiCoordinateData coordinateData;

static inline void RFLiSetCoordinateData(const RFLiCoordinateData* data) {
    coordinateData = *data;
}

u32 RFLiGetExpressionNum(u32 exprFlags);
void RFLiInitCharModel(RFLCharModel* model, RFLiCharInfo* info, void* work,
                       RFLResolution res, u32 exprFlags);
GXColor RFLiGetFacelineColor(RFLiCharInfo* info);
void RFLiInitCharModelRes(RFLiCharModelRes* res, RFLiCharInfo* info);
void RFLiInitShapeRes(RFLiShapeRes* shape);
void RFLiInitTexRes(GXTexObj* texObj, RFLiPartsShpTex part, u16 file,
                    void* buffer);
void RFLiTransformCoordinate(s16* to, const s16* from);

#ifdef __cplusplus
}
#endif
#endif

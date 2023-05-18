#ifndef RVL_SDK_GX_HARDWARE_BP_H
#define RVL_SDK_GX_HARDWARE_BP_H
#include <revolution/GX/GXTypes.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *
 *
 * GX Blitting Processor (BP)
 *
 *
 *****************************************************************************/

/**
 * BP registers
 */
typedef enum {
    GX_BP_REG_GENMODE = 0x0,
    GX_BP_REG_DISPCOPYFILTER0 = 0x1,
    GX_BP_REG_DISPCOPYFILTER1 = 0x2,
    GX_BP_REG_DISPCOPYFILTER2 = 0x3,
    GX_BP_REG_DISPCOPYFILTER3 = 0x4,
    GX_BP_REG_INDMTX0A = 0x6,
    GX_BP_REG_INDMTX0B = 0x7,
    GX_BP_REG_INDMTX0C = 0x8,
    GX_BP_REG_INDMTX1A = 0x9,
    GX_BP_REG_INDMTX1B = 0xA,
    GX_BP_REG_INDMTX1C = 0xB,
    GX_BP_REG_INDMTX2A = 0xC,
    GX_BP_REG_INDMTX2B = 0xD,
    GX_BP_REG_INDMTX2C = 0xE,
    GX_BP_REG_INDIMASK = 0xF,
    GX_BP_REG_INDTEVSTAGE0 = 0x10,
    GX_BP_REG_INDTEVSTAGE1 = 0x11,
    GX_BP_REG_INDTEVSTAGE2 = 0x12,
    GX_BP_REG_INDTEVSTAGE3 = 0x13,
    GX_BP_REG_INDTEVSTAGE4 = 0x14,
    GX_BP_REG_INDTEVSTAGE5 = 0x15,
    GX_BP_REG_INDTEVSTAGE6 = 0x16,
    GX_BP_REG_INDTEVSTAGE7 = 0x17,
    GX_BP_REG_INDTEVSTAGE8 = 0x18,
    GX_BP_REG_INDTEVSTAGE9 = 0x19,
    GX_BP_REG_INDTEVSTAGE10 = 0x1A,
    GX_BP_REG_INDTEVSTAGE11 = 0x1B,
    GX_BP_REG_INDTEVSTAGE12 = 0x1C,
    GX_BP_REG_INDTEVSTAGE13 = 0x1D,
    GX_BP_REG_INDTEVSTAGE14 = 0x1E,
    GX_BP_REG_INDTEVSTAGE15 = 0x1F,
    GX_BP_REG_SCISSORTL = 0x20,
    GX_BP_REG_SCISSORBR = 0x21,
    GX_BP_REG_LINEPTWIDTH = 0x22,
    GX_BP_REG_PERF0TRI = 0x23,
    GX_BP_REG_PERF0QUAD = 0x24,
    GX_BP_REG_RAS1_SS0 = 0x25,
    GX_BP_REG_RAS1_SS1 = 0x26,
    GX_BP_REG_RAS1_IREF = 0x27,
    GX_BP_REG_RAS1_TREF0 = 0x28,
    GX_BP_REG_RAS1_TREF1 = 0x29,
    GX_BP_REG_RAS1_TREF2 = 0x2A,
    GX_BP_REG_RAS1_TREF3 = 0x2B,
    GX_BP_REG_RAS1_TREF4 = 0x2C,
    GX_BP_REG_RAS1_TREF5 = 0x2D,
    GX_BP_REG_RAS1_TREF6 = 0x2E,
    GX_BP_REG_RAS1_TREF7 = 0x2F,
    GX_BP_REG_SU_SSIZE0 = 0x30,
    GX_BP_REG_SU_TSIZE0 = 0x31,
    GX_BP_REG_SU_SSIZE1 = 0x32,
    GX_BP_REG_SU_TSIZE1 = 0x33,
    GX_BP_REG_SU_SSIZE2 = 0x34,
    GX_BP_REG_SU_TSIZE2 = 0x35,
    GX_BP_REG_SU_SSIZE3 = 0x36,
    GX_BP_REG_SU_TSIZE3 = 0x37,
    GX_BP_REG_SU_SSIZE4 = 0x38,
    GX_BP_REG_SU_TSIZE4 = 0x39,
    GX_BP_REG_SU_SSIZE5 = 0x3A,
    GX_BP_REG_SU_TSIZE5 = 0x3B,
    GX_BP_REG_SU_SSIZE6 = 0x3C,
    GX_BP_REG_SU_TSIZE6 = 0x3D,
    GX_BP_REG_SU_SSIZE7 = 0x3E,
    GX_BP_REG_SU_TSIZE7 = 0x3F,
    GX_BP_REG_ZMODE = 0x40,
    GX_BP_REG_BLENDMODE = 0x41,
    GX_BP_REG_DSTALPHA = 0x42,
    GX_BP_REG_ZCONTROL = 0x43,
    GX_BP_REG_FIELDMASK = 0x44,
    GX_BP_REG_DRAWDONE = 0x45,
    GX_BP_REG_PETOKEN = 0x47,
    GX_BP_REG_PETOKENINT = 0x48,
    GX_BP_REG_TEXCOPYSRCXY = 0x49,
    GX_BP_REG_TEXCOPYSRCWH = 0x4A,
    GX_BP_REG_TEXCOPYDST = 0x4B,
    GX_BP_REG_DISPCOPYSTRIDE = 0x4D,
    GX_BP_REG_DISPCOPYSCALEY = 0x4E,
    GX_BP_REG_COPYCLEARAR = 0x4F,
    GX_BP_REG_COPYCLEARGB = 0x50,
    GX_BP_REG_COPYCLEARZ = 0x51,
    GX_BP_REG_COPYFILTER0 = 0x53,
    GX_BP_REG_COPYFILTER1 = 0x54,
    GX_BP_REG_BOUNDINGBOX0 = 0x55,
    GX_BP_REG_BOUNDINGBOX1 = 0x56,
    GX_BP_REG_SCISSOROFFSET = 0x59,
    GX_BP_REG_TMEMPRELOADADDR = 0x60,
    GX_BP_REG_TMEMPRELOADEVEN = 0x61,
    GX_BP_REG_TMEMPRELOADODD = 0x62,
    GX_BP_REG_TMEMPRELOADMODE = 0x63,
    GX_BP_REG_TMEMTLUTSRC = 0x64,
    GX_BP_REG_TMEMTLUTDST = 0x65,
    GX_BP_REG_TMEMTEXINVALIDATE = 0x66,
    GX_BP_REG_PERF1 = 0x67,
    GX_BP_REG_FIELDMODE = 0x68,
    GX_BP_REG_SETMODE0_TEX0 = 0x80,
    GX_BP_REG_SETMODE0_TEX1 = 0x81,
    GX_BP_REG_SETMODE0_TEX2 = 0x82,
    GX_BP_REG_SETMODE0_TEX3 = 0x83,
    GX_BP_REG_SETMODE1_TEX0 = 0x84,
    GX_BP_REG_SETMODE1_TEX1 = 0x85,
    GX_BP_REG_SETMODE1_TEX2 = 0x86,
    GX_BP_REG_SETMODE1_TEX3 = 0x87,
    GX_BP_REG_SETIMAGE0_TEX0 = 0x88,
    GX_BP_REG_SETIMAGE0_TEX1 = 0x89,
    GX_BP_REG_SETIMAGE0_TEX2 = 0x8A,
    GX_BP_REG_SETIMAGE0_TEX3 = 0x8B,
    GX_BP_REG_SETIMAGE1_TEX0 = 0x8C,
    GX_BP_REG_SETIMAGE1_TEX1 = 0x8D,
    GX_BP_REG_SETIMAGE1_TEX2 = 0x8E,
    GX_BP_REG_SETIMAGE1_TEX3 = 0x8F,
    GX_BP_REG_SETIMAGE2_TEX0 = 0x90,
    GX_BP_REG_SETIMAGE2_TEX1 = 0x91,
    GX_BP_REG_SETIMAGE2_TEX2 = 0x92,
    GX_BP_REG_SETIMAGE2_TEX3 = 0x93,
    GX_BP_REG_SETIMAGE3_TEX0 = 0x94,
    GX_BP_REG_SETIMAGE3_TEX1 = 0x95,
    GX_BP_REG_SETIMAGE3_TEX2 = 0x96,
    GX_BP_REG_SETIMAGE3_TEX3 = 0x97,
    GX_BP_REG_SETTLUT_TEX0 = 0x98,
    GX_BP_REG_SETTLUT_TEX1 = 0x99,
    GX_BP_REG_SETTLUT_TEX2 = 0x9A,
    GX_BP_REG_SETTLUT_TEX3 = 0x9B,
    GX_BP_REG_SETMODE0_TEX4 = 0xA0,
    GX_BP_REG_SETMODE0_TEX5 = 0xA1,
    GX_BP_REG_SETMODE0_TEX6 = 0xA2,
    GX_BP_REG_SETMODE0_TEX7 = 0xA3,
    GX_BP_REG_SETMODE1_TEX4 = 0xA4,
    GX_BP_REG_SETMODE1_TEX5 = 0xA5,
    GX_BP_REG_SETMODE1_TEX6 = 0xA6,
    GX_BP_REG_SETMODE1_TEX7 = 0xA7,
    GX_BP_REG_SETIMAGE0_TEX4 = 0xA8,
    GX_BP_REG_SETIMAGE0_TEX5 = 0xA9,
    GX_BP_REG_SETIMAGE0_TEX6 = 0xAA,
    GX_BP_REG_SETIMAGE0_TEX7 = 0xAB,
    GX_BP_REG_SETIMAGE1_TEX4 = 0xAC,
    GX_BP_REG_SETIMAGE1_TEX5 = 0xAD,
    GX_BP_REG_SETIMAGE1_TEX6 = 0xAE,
    GX_BP_REG_SETIMAGE1_TEX7 = 0xAF,
    GX_BP_REG_SETIMAGE2_TEX4 = 0xB0,
    GX_BP_REG_SETIMAGE2_TEX5 = 0xB1,
    GX_BP_REG_SETIMAGE2_TEX6 = 0xB2,
    GX_BP_REG_SETIMAGE2_TEX7 = 0xB3,
    GX_BP_REG_SETIMAGE3_TEX4 = 0xB4,
    GX_BP_REG_SETIMAGE3_TEX5 = 0xB5,
    GX_BP_REG_SETIMAGE3_TEX6 = 0xB6,
    GX_BP_REG_SETIMAGE3_TEX7 = 0xB7,
    GX_BP_REG_SETTLUT_TEX4 = 0xB8,
    GX_BP_REG_SETTLUT_TEX5 = 0xB9,
    GX_BP_REG_SETTLUT_TEX6 = 0xBA,
    GX_BP_REG_SETTLUT_TEX7 = 0xBB,
    GX_BP_REG_TEVCOLORCOMBINER0 = 0xC0,
    GX_BP_REG_TEVALPHACOMBINER0 = 0xC1,
    GX_BP_REG_TEVCOLORCOMBINER1 = 0xC2,
    GX_BP_REG_TEVALPHACOMBINER1 = 0xC3,
    GX_BP_REG_TEVCOLORCOMBINER2 = 0xC4,
    GX_BP_REG_TEVALPHACOMBINER2 = 0xC5,
    GX_BP_REG_TEVCOLORCOMBINER3 = 0xC6,
    GX_BP_REG_TEVALPHACOMBINER3 = 0xC7,
    GX_BP_REG_TEVCOLORCOMBINER4 = 0xC8,
    GX_BP_REG_TEVALPHACOMBINER4 = 0xC9,
    GX_BP_REG_TEVCOLORCOMBINER5 = 0xCA,
    GX_BP_REG_TEVALPHACOMBINER5 = 0xCB,
    GX_BP_REG_TEVCOLORCOMBINER6 = 0xCC,
    GX_BP_REG_TEVALPHACOMBINER6 = 0xCD,
    GX_BP_REG_TEVCOLORCOMBINER7 = 0xCE,
    GX_BP_REG_TEVALPHACOMBINER7 = 0xCF,
    GX_BP_REG_TEVCOLORCOMBINER8 = 0xD0,
    GX_BP_REG_TEVALPHACOMBINER8 = 0xD1,
    GX_BP_REG_TEVCOLORCOMBINER9 = 0xD2,
    GX_BP_REG_TEVALPHACOMBINER9 = 0xD3,
    GX_BP_REG_TEVCOLORCOMBINER10 = 0xD4,
    GX_BP_REG_TEVALPHACOMBINER10 = 0xD5,
    GX_BP_REG_TEVCOLORCOMBINER11 = 0xD6,
    GX_BP_REG_TEVALPHACOMBINER11 = 0xD7,
    GX_BP_REG_TEVCOLORCOMBINER12 = 0xD8,
    GX_BP_REG_TEVALPHACOMBINER12 = 0xD9,
    GX_BP_REG_TEVCOLORCOMBINER13 = 0xDA,
    GX_BP_REG_TEVALPHACOMBINER13 = 0xDB,
    GX_BP_REG_TEVCOLORCOMBINER14 = 0xDC,
    GX_BP_REG_TEVALPHACOMBINER14 = 0xDD,
    GX_BP_REG_TEVCOLORCOMBINER15 = 0xDE,
    GX_BP_REG_TEVALPHACOMBINER15 = 0xDF,
    GX_BP_REG_TEVREG0LO = 0xE0,
    GX_BP_REG_TEVREG0HI = 0xE1,
    GX_BP_REG_TEVREG1LO = 0xE2,
    GX_BP_REG_TEVREG1HI = 0xE3,
    GX_BP_REG_TEVREG2LO = 0xE4,
    GX_BP_REG_TEVREG2HI = 0xE5,
    GX_BP_REG_TEVREG3LO = 0xE6,
    GX_BP_REG_TEVREG3HI = 0xE7,
    GX_BP_REG_FOGRANGE = 0xE8,
    GX_BP_REG_FOGPARAM0 = 0xEE,
    GX_BP_REG_FOGPARAM1 = 0xEF,
    GX_BP_REG_FOGPARAM2 = 0xF0,
    GX_BP_REG_FOGPARAM3 = 0xF1,
    GX_BP_REG_FOGCOLOR = 0xF2,
    GX_BP_REG_ALPHACOMPARE = 0xF3,
    GX_BP_REG_ZTEXTURE0 = 0xF4,
    GX_BP_REG_ZTEXTURE1 = 0xF5,
    GX_BP_REG_TEVKSEL0 = 0xF6,
    GX_BP_REG_TEVKSEL1 = 0xF7,
    GX_BP_REG_TEVKSEL2 = 0xF8,
    GX_BP_REG_TEVKSEL3 = 0xF9,
    GX_BP_REG_TEVKSEL4 = 0xFA,
    GX_BP_REG_TEVKSEL5 = 0xFB,
    GX_BP_REG_TEVKSEL6 = 0xFC,
    GX_BP_REG_TEVKSEL7 = 0xFD,
    GX_BP_REG_SSMASK = 0xFE,
} GX_BP_REG;

/**
 * BP register 0x0 - GenMode
 */
// NUMTEX [28:31] (4) - Active texture count
#define GX_BP_GENMODE_NUMTEX_ST 28
#define GX_BP_GENMODE_NUMTEX_END 31
#define GX_BP_GENMODE_NUMTEX_SZ 4
#define GX_BP_GENMODE_NUMTEX_MASK (((1 << 4) - 1) << 31 - 31)
#define GX_BP_GET_GENMODE_NUMTEX(reg) GX_BITGET(reg, 28, 4)
#define GX_BP_SET_GENMODE_NUMTEX(reg, x) ((reg) = GX_BITSET(reg, 28, 4, x))
// NUMCOLORS [25:27] (3) - Color/channel count
#define GX_BP_GENMODE_NUMCOLORS_ST 25
#define GX_BP_GENMODE_NUMCOLORS_END 27
#define GX_BP_GENMODE_NUMCOLORS_SZ 3
#define GX_BP_GENMODE_NUMCOLORS_MASK (((1 << 3) - 1) << 31 - 27)
#define GX_BP_GET_GENMODE_NUMCOLORS(reg) GX_BITGET(reg, 25, 3)
#define GX_BP_SET_GENMODE_NUMCOLORS(reg, x) ((reg) = GX_BITSET(reg, 25, 3, x))
// CULLMODE [16:17] (2) - Cull mode
#define GX_BP_GENMODE_CULLMODE_ST 16
#define GX_BP_GENMODE_CULLMODE_END 17
#define GX_BP_GENMODE_CULLMODE_SZ 2
#define GX_BP_GENMODE_CULLMODE_MASK (((1 << 2) - 1) << 31 - 17)
#define GX_BP_GET_GENMODE_CULLMODE(reg) GX_BITGET(reg, 16, 2)
#define GX_BP_SET_GENMODE_CULLMODE(reg, x) ((reg) = GX_BITSET(reg, 16, 2, x))
// NUMINDSTAGES [13:15] (3) - Indirect stage count
#define GX_BP_GENMODE_NUMINDSTAGES_ST 13
#define GX_BP_GENMODE_NUMINDSTAGES_END 15
#define GX_BP_GENMODE_NUMINDSTAGES_SZ 3
#define GX_BP_GENMODE_NUMINDSTAGES_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_GENMODE_NUMINDSTAGES(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_GENMODE_NUMINDSTAGES(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 13, 3, x))
// COPLANAR [12:12] (1) - Toggle co-planar ("Z freeze" according to Dolphin)
#define GX_BP_GENMODE_COPLANAR_ST 12
#define GX_BP_GENMODE_COPLANAR_END 12
#define GX_BP_GENMODE_COPLANAR_SZ 1
#define GX_BP_GENMODE_COPLANAR_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_GENMODE_COPLANAR(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_GENMODE_COPLANAR(reg, x) ((reg) = GX_BITSET(reg, 12, 1, x))

/**
 * BP register 0x6 - IndMtx0A
 */
// M00 [21:31] (11) - Texture offset matrix #0 [0][0]
#define GX_BP_INDMTX0A_M00_ST 21
#define GX_BP_INDMTX0A_M00_END 31
#define GX_BP_INDMTX0A_M00_SZ 11
#define GX_BP_INDMTX0A_M00_MASK (((1 << 11) - 1) << 31 - 31)
#define GX_BP_GET_INDMTX0A_M00(reg) GX_BITGET(reg, 21, 11)
#define GX_BP_SET_INDMTX0A_M00(reg, x) ((reg) = GX_BITSET(reg, 21, 11, x))
// M10 [10:20] (11) - Texture offset matrix #0 [1][0]
#define GX_BP_INDMTX0A_M10_ST 10
#define GX_BP_INDMTX0A_M10_END 20
#define GX_BP_INDMTX0A_M10_SZ 11
#define GX_BP_INDMTX0A_M10_MASK (((1 << 11) - 1) << 31 - 20)
#define GX_BP_GET_INDMTX0A_M10(reg) GX_BITGET(reg, 10, 11)
#define GX_BP_SET_INDMTX0A_M10(reg, x) ((reg) = GX_BITSET(reg, 10, 11, x))
// EXP [8:9] (2) - Bits 0-1 of scaling exponent #0 (2^x)
#define GX_BP_INDMTX0A_EXP_ST 8
#define GX_BP_INDMTX0A_EXP_END 9
#define GX_BP_INDMTX0A_EXP_SZ 2
#define GX_BP_INDMTX0A_EXP_MASK (((1 << 2) - 1) << 31 - 9)
#define GX_BP_GET_INDMTX0A_EXP(reg) GX_BITGET(reg, 8, 2)
#define GX_BP_SET_INDMTX0A_EXP(reg, x) ((reg) = GX_BITSET(reg, 8, 2, x))

/**
 * BP register 0x7 - IndMtx0B
 */
// M01 [21:31] (11) - Texture offset matrix #0 [0][1]
#define GX_BP_INDMTX0B_M01_ST 21
#define GX_BP_INDMTX0B_M01_END 31
#define GX_BP_INDMTX0B_M01_SZ 11
#define GX_BP_INDMTX0B_M01_MASK (((1 << 11) - 1) << 31 - 31)
#define GX_BP_GET_INDMTX0B_M01(reg) GX_BITGET(reg, 21, 11)
#define GX_BP_SET_INDMTX0B_M01(reg, x) ((reg) = GX_BITSET(reg, 21, 11, x))
// M11 [10:20] (11) - Texture offset matrix #0 [1][1]
#define GX_BP_INDMTX0B_M11_ST 10
#define GX_BP_INDMTX0B_M11_END 20
#define GX_BP_INDMTX0B_M11_SZ 11
#define GX_BP_INDMTX0B_M11_MASK (((1 << 11) - 1) << 31 - 20)
#define GX_BP_GET_INDMTX0B_M11(reg) GX_BITGET(reg, 10, 11)
#define GX_BP_SET_INDMTX0B_M11(reg, x) ((reg) = GX_BITSET(reg, 10, 11, x))
// EXP [8:9] (2) - Bits 2-3 of scaling exponent #0 (2^x)
#define GX_BP_INDMTX0B_EXP_ST 8
#define GX_BP_INDMTX0B_EXP_END 9
#define GX_BP_INDMTX0B_EXP_SZ 2
#define GX_BP_INDMTX0B_EXP_MASK (((1 << 2) - 1) << 31 - 9)
#define GX_BP_GET_INDMTX0B_EXP(reg) GX_BITGET(reg, 8, 2)
#define GX_BP_SET_INDMTX0B_EXP(reg, x) ((reg) = GX_BITSET(reg, 8, 2, x))

/**
 * BP register 0x8 - IndMtx0C
 */
// M02 [21:31] (11) - Texture offset matrix #0 [0][2]
#define GX_BP_INDMTX0C_M02_ST 21
#define GX_BP_INDMTX0C_M02_END 31
#define GX_BP_INDMTX0C_M02_SZ 11
#define GX_BP_INDMTX0C_M02_MASK (((1 << 11) - 1) << 31 - 31)
#define GX_BP_GET_INDMTX0C_M02(reg) GX_BITGET(reg, 21, 11)
#define GX_BP_SET_INDMTX0C_M02(reg, x) ((reg) = GX_BITSET(reg, 21, 11, x))
// M12 [10:20] (11) - Texture offset matrix #0 [1][2]
#define GX_BP_INDMTX0C_M12_ST 10
#define GX_BP_INDMTX0C_M12_END 20
#define GX_BP_INDMTX0C_M12_SZ 11
#define GX_BP_INDMTX0C_M12_MASK (((1 << 11) - 1) << 31 - 20)
#define GX_BP_GET_INDMTX0C_M12(reg) GX_BITGET(reg, 10, 11)
#define GX_BP_SET_INDMTX0C_M12(reg, x) ((reg) = GX_BITSET(reg, 10, 11, x))
// EXP [8:9] (2) - Bit 4 of scaling exponent #0 (2^x)
#define GX_BP_INDMTX0C_EXP_ST 8
#define GX_BP_INDMTX0C_EXP_END 9
#define GX_BP_INDMTX0C_EXP_SZ 2
#define GX_BP_INDMTX0C_EXP_MASK (((1 << 2) - 1) << 31 - 9)
#define GX_BP_GET_INDMTX0C_EXP(reg) GX_BITGET(reg, 8, 2)
#define GX_BP_SET_INDMTX0C_EXP(reg, x) ((reg) = GX_BITSET(reg, 8, 2, x))

/**
 * BP register 0x9 - IndMtx1A
 */
// M00 [21:31] (11) - Texture offset matrix #1 [0][0]
#define GX_BP_INDMTX1A_M00_ST 21
#define GX_BP_INDMTX1A_M00_END 31
#define GX_BP_INDMTX1A_M00_SZ 11
#define GX_BP_INDMTX1A_M00_MASK (((1 << 11) - 1) << 31 - 31)
#define GX_BP_GET_INDMTX1A_M00(reg) GX_BITGET(reg, 21, 11)
#define GX_BP_SET_INDMTX1A_M00(reg, x) ((reg) = GX_BITSET(reg, 21, 11, x))
// M10 [10:20] (11) - Texture offset matrix #1 [1][0]
#define GX_BP_INDMTX1A_M10_ST 10
#define GX_BP_INDMTX1A_M10_END 20
#define GX_BP_INDMTX1A_M10_SZ 11
#define GX_BP_INDMTX1A_M10_MASK (((1 << 11) - 1) << 31 - 20)
#define GX_BP_GET_INDMTX1A_M10(reg) GX_BITGET(reg, 10, 11)
#define GX_BP_SET_INDMTX1A_M10(reg, x) ((reg) = GX_BITSET(reg, 10, 11, x))
// EXP [8:9] (2) - Bits 0-1 of scaling exponent #1 (2^x)
#define GX_BP_INDMTX1A_EXP_ST 8
#define GX_BP_INDMTX1A_EXP_END 9
#define GX_BP_INDMTX1A_EXP_SZ 2
#define GX_BP_INDMTX1A_EXP_MASK (((1 << 2) - 1) << 31 - 9)
#define GX_BP_GET_INDMTX1A_EXP(reg) GX_BITGET(reg, 8, 2)
#define GX_BP_SET_INDMTX1A_EXP(reg, x) ((reg) = GX_BITSET(reg, 8, 2, x))

/**
 * BP register 0xA - IndMtx1B
 */
// M01 [21:31] (11) - Texture offset matrix #1 [0][1]
#define GX_BP_INDMTX1B_M01_ST 21
#define GX_BP_INDMTX1B_M01_END 31
#define GX_BP_INDMTX1B_M01_SZ 11
#define GX_BP_INDMTX1B_M01_MASK (((1 << 11) - 1) << 31 - 31)
#define GX_BP_GET_INDMTX1B_M01(reg) GX_BITGET(reg, 21, 11)
#define GX_BP_SET_INDMTX1B_M01(reg, x) ((reg) = GX_BITSET(reg, 21, 11, x))
// M11 [10:20] (11) - Texture offset matrix #1 [1][1]
#define GX_BP_INDMTX1B_M11_ST 10
#define GX_BP_INDMTX1B_M11_END 20
#define GX_BP_INDMTX1B_M11_SZ 11
#define GX_BP_INDMTX1B_M11_MASK (((1 << 11) - 1) << 31 - 20)
#define GX_BP_GET_INDMTX1B_M11(reg) GX_BITGET(reg, 10, 11)
#define GX_BP_SET_INDMTX1B_M11(reg, x) ((reg) = GX_BITSET(reg, 10, 11, x))
// EXP [8:9] (2) - Bits 2-3 of scaling exponent #1 (2^x)
#define GX_BP_INDMTX1B_EXP_ST 8
#define GX_BP_INDMTX1B_EXP_END 9
#define GX_BP_INDMTX1B_EXP_SZ 2
#define GX_BP_INDMTX1B_EXP_MASK (((1 << 2) - 1) << 31 - 9)
#define GX_BP_GET_INDMTX1B_EXP(reg) GX_BITGET(reg, 8, 2)
#define GX_BP_SET_INDMTX1B_EXP(reg, x) ((reg) = GX_BITSET(reg, 8, 2, x))

/**
 * BP register 0xB - IndMtx1C
 */
// M02 [21:31] (11) - Texture offset matrix #1 [0][2]
#define GX_BP_INDMTX1C_M02_ST 21
#define GX_BP_INDMTX1C_M02_END 31
#define GX_BP_INDMTX1C_M02_SZ 11
#define GX_BP_INDMTX1C_M02_MASK (((1 << 11) - 1) << 31 - 31)
#define GX_BP_GET_INDMTX1C_M02(reg) GX_BITGET(reg, 21, 11)
#define GX_BP_SET_INDMTX1C_M02(reg, x) ((reg) = GX_BITSET(reg, 21, 11, x))
// M12 [10:20] (11) - Texture offset matrix #1 [1][2]
#define GX_BP_INDMTX1C_M12_ST 10
#define GX_BP_INDMTX1C_M12_END 20
#define GX_BP_INDMTX1C_M12_SZ 11
#define GX_BP_INDMTX1C_M12_MASK (((1 << 11) - 1) << 31 - 20)
#define GX_BP_GET_INDMTX1C_M12(reg) GX_BITGET(reg, 10, 11)
#define GX_BP_SET_INDMTX1C_M12(reg, x) ((reg) = GX_BITSET(reg, 10, 11, x))
// EXP [8:9] (2) - Bit 4 of scaling exponent #1 (2^x)
#define GX_BP_INDMTX1C_EXP_ST 8
#define GX_BP_INDMTX1C_EXP_END 9
#define GX_BP_INDMTX1C_EXP_SZ 2
#define GX_BP_INDMTX1C_EXP_MASK (((1 << 2) - 1) << 31 - 9)
#define GX_BP_GET_INDMTX1C_EXP(reg) GX_BITGET(reg, 8, 2)
#define GX_BP_SET_INDMTX1C_EXP(reg, x) ((reg) = GX_BITSET(reg, 8, 2, x))

/**
 * BP register 0xC - IndMtx2A
 */
// M00 [21:31] (11) - Texture offset matrix #2 [0][0]
#define GX_BP_INDMTX2A_M00_ST 21
#define GX_BP_INDMTX2A_M00_END 31
#define GX_BP_INDMTX2A_M00_SZ 11
#define GX_BP_INDMTX2A_M00_MASK (((1 << 11) - 1) << 31 - 31)
#define GX_BP_GET_INDMTX2A_M00(reg) GX_BITGET(reg, 21, 11)
#define GX_BP_SET_INDMTX2A_M00(reg, x) ((reg) = GX_BITSET(reg, 21, 11, x))
// M10 [10:20] (11) - Texture offset matrix #2 [1][0]
#define GX_BP_INDMTX2A_M10_ST 10
#define GX_BP_INDMTX2A_M10_END 20
#define GX_BP_INDMTX2A_M10_SZ 11
#define GX_BP_INDMTX2A_M10_MASK (((1 << 11) - 1) << 31 - 20)
#define GX_BP_GET_INDMTX2A_M10(reg) GX_BITGET(reg, 10, 11)
#define GX_BP_SET_INDMTX2A_M10(reg, x) ((reg) = GX_BITSET(reg, 10, 11, x))
// EXP [8:9] (2) - Bits 0-1 of scaling exponent #2 (2^x)
#define GX_BP_INDMTX2A_EXP_ST 8
#define GX_BP_INDMTX2A_EXP_END 9
#define GX_BP_INDMTX2A_EXP_SZ 2
#define GX_BP_INDMTX2A_EXP_MASK (((1 << 2) - 1) << 31 - 9)
#define GX_BP_GET_INDMTX2A_EXP(reg) GX_BITGET(reg, 8, 2)
#define GX_BP_SET_INDMTX2A_EXP(reg, x) ((reg) = GX_BITSET(reg, 8, 2, x))

/**
 * BP register 0xD - IndMtx2B
 */
// M01 [21:31] (11) - Texture offset matrix #2 [0][1]
#define GX_BP_INDMTX2B_M01_ST 21
#define GX_BP_INDMTX2B_M01_END 31
#define GX_BP_INDMTX2B_M01_SZ 11
#define GX_BP_INDMTX2B_M01_MASK (((1 << 11) - 1) << 31 - 31)
#define GX_BP_GET_INDMTX2B_M01(reg) GX_BITGET(reg, 21, 11)
#define GX_BP_SET_INDMTX2B_M01(reg, x) ((reg) = GX_BITSET(reg, 21, 11, x))
// M11 [10:20] (11) - Texture offset matrix #2 [1][1]
#define GX_BP_INDMTX2B_M11_ST 10
#define GX_BP_INDMTX2B_M11_END 20
#define GX_BP_INDMTX2B_M11_SZ 11
#define GX_BP_INDMTX2B_M11_MASK (((1 << 11) - 1) << 31 - 20)
#define GX_BP_GET_INDMTX2B_M11(reg) GX_BITGET(reg, 10, 11)
#define GX_BP_SET_INDMTX2B_M11(reg, x) ((reg) = GX_BITSET(reg, 10, 11, x))
// EXP [8:9] (2) - Bits 2-3 of scaling exponent #2 (2^x)
#define GX_BP_INDMTX2B_EXP_ST 8
#define GX_BP_INDMTX2B_EXP_END 9
#define GX_BP_INDMTX2B_EXP_SZ 2
#define GX_BP_INDMTX2B_EXP_MASK (((1 << 2) - 1) << 31 - 9)
#define GX_BP_GET_INDMTX2B_EXP(reg) GX_BITGET(reg, 8, 2)
#define GX_BP_SET_INDMTX2B_EXP(reg, x) ((reg) = GX_BITSET(reg, 8, 2, x))

/**
 * BP register 0xE - IndMtx2C
 */
// M02 [21:31] (11) - Texture offset matrix #2 [0][2]
#define GX_BP_INDMTX2C_M02_ST 21
#define GX_BP_INDMTX2C_M02_END 31
#define GX_BP_INDMTX2C_M02_SZ 11
#define GX_BP_INDMTX2C_M02_MASK (((1 << 11) - 1) << 31 - 31)
#define GX_BP_GET_INDMTX2C_M02(reg) GX_BITGET(reg, 21, 11)
#define GX_BP_SET_INDMTX2C_M02(reg, x) ((reg) = GX_BITSET(reg, 21, 11, x))
// M12 [10:20] (11) - Texture offset matrix #2 [1][2]
#define GX_BP_INDMTX2C_M12_ST 10
#define GX_BP_INDMTX2C_M12_END 20
#define GX_BP_INDMTX2C_M12_SZ 11
#define GX_BP_INDMTX2C_M12_MASK (((1 << 11) - 1) << 31 - 20)
#define GX_BP_GET_INDMTX2C_M12(reg) GX_BITGET(reg, 10, 11)
#define GX_BP_SET_INDMTX2C_M12(reg, x) ((reg) = GX_BITSET(reg, 10, 11, x))
// EXP [8:9] (2) - Bit 4 of scaling exponent #2 (2^x)
#define GX_BP_INDMTX2C_EXP_ST 8
#define GX_BP_INDMTX2C_EXP_END 9
#define GX_BP_INDMTX2C_EXP_SZ 2
#define GX_BP_INDMTX2C_EXP_MASK (((1 << 2) - 1) << 31 - 9)
#define GX_BP_GET_INDMTX2C_EXP(reg) GX_BITGET(reg, 8, 2)
#define GX_BP_SET_INDMTX2C_EXP(reg, x) ((reg) = GX_BITSET(reg, 8, 2, x))

/**
 * BP register 0xF - IndIMask
 */
// IMASK [24:31] (8) - Indirect mask for textures
#define GX_BP_INDIMASK_IMASK_ST 24
#define GX_BP_INDIMASK_IMASK_END 31
#define GX_BP_INDIMASK_IMASK_SZ 8
#define GX_BP_INDIMASK_IMASK_MASK (((1 << 8) - 1) << 31 - 31)
#define GX_BP_GET_INDIMASK_IMASK(reg) GX_BITGET(reg, 24, 8)
#define GX_BP_SET_INDIMASK_IMASK(reg, x) ((reg) = GX_BITSET(reg, 24, 8, x))

/**
 * BP register 0x10 - IndTevStage0
 */
// STAGE [30:31] (2) - Indirect texture stage ID (stage 0)
#define GX_BP_INDTEVSTAGE0_STAGE_ST 30
#define GX_BP_INDTEVSTAGE0_STAGE_END 31
#define GX_BP_INDTEVSTAGE0_STAGE_SZ 2
#define GX_BP_INDTEVSTAGE0_STAGE_MASK (((1 << 2) - 1) << 31 - 31)
#define GX_BP_GET_INDTEVSTAGE0_STAGE(reg) GX_BITGET(reg, 30, 2)
#define GX_BP_SET_INDTEVSTAGE0_STAGE(reg, x) ((reg) = GX_BITSET(reg, 30, 2, x))
// FMT [28:29] (2) - Indirect texture format (stage 0)
#define GX_BP_INDTEVSTAGE0_FMT_ST 28
#define GX_BP_INDTEVSTAGE0_FMT_END 29
#define GX_BP_INDTEVSTAGE0_FMT_SZ 2
#define GX_BP_INDTEVSTAGE0_FMT_MASK (((1 << 2) - 1) << 31 - 29)
#define GX_BP_GET_INDTEVSTAGE0_FMT(reg) GX_BITGET(reg, 28, 2)
#define GX_BP_SET_INDTEVSTAGE0_FMT(reg, x) ((reg) = GX_BITSET(reg, 28, 2, x))
// BIAS [25:27] (3) - Indirect texture bias (stage 0)
#define GX_BP_INDTEVSTAGE0_BIAS_ST 25
#define GX_BP_INDTEVSTAGE0_BIAS_END 27
#define GX_BP_INDTEVSTAGE0_BIAS_SZ 3
#define GX_BP_INDTEVSTAGE0_BIAS_MASK (((1 << 3) - 1) << 31 - 27)
#define GX_BP_GET_INDTEVSTAGE0_BIAS(reg) GX_BITGET(reg, 25, 3)
#define GX_BP_SET_INDTEVSTAGE0_BIAS(reg, x) ((reg) = GX_BITSET(reg, 25, 3, x))
// ALPHA [23:24] (2) - Indirect texture alpha (stage 0)
#define GX_BP_INDTEVSTAGE0_ALPHA_ST 23
#define GX_BP_INDTEVSTAGE0_ALPHA_END 24
#define GX_BP_INDTEVSTAGE0_ALPHA_SZ 2
#define GX_BP_INDTEVSTAGE0_ALPHA_MASK (((1 << 2) - 1) << 31 - 24)
#define GX_BP_GET_INDTEVSTAGE0_ALPHA(reg) GX_BITGET(reg, 23, 2)
#define GX_BP_SET_INDTEVSTAGE0_ALPHA(reg, x) ((reg) = GX_BITSET(reg, 23, 2, x))
// MTX [19:22] (4) - Indirect texture matrix (stage 0)
#define GX_BP_INDTEVSTAGE0_MTX_ST 19
#define GX_BP_INDTEVSTAGE0_MTX_END 22
#define GX_BP_INDTEVSTAGE0_MTX_SZ 4
#define GX_BP_INDTEVSTAGE0_MTX_MASK (((1 << 4) - 1) << 31 - 22)
#define GX_BP_GET_INDTEVSTAGE0_MTX(reg) GX_BITGET(reg, 19, 4)
#define GX_BP_SET_INDTEVSTAGE0_MTX(reg, x) ((reg) = GX_BITSET(reg, 19, 4, x))
// WRAPS [16:18] (3) - S component wrap factor (stage 0)
#define GX_BP_INDTEVSTAGE0_WRAPS_ST 16
#define GX_BP_INDTEVSTAGE0_WRAPS_END 18
#define GX_BP_INDTEVSTAGE0_WRAPS_SZ 3
#define GX_BP_INDTEVSTAGE0_WRAPS_MASK (((1 << 3) - 1) << 31 - 18)
#define GX_BP_GET_INDTEVSTAGE0_WRAPS(reg) GX_BITGET(reg, 16, 3)
#define GX_BP_SET_INDTEVSTAGE0_WRAPS(reg, x) ((reg) = GX_BITSET(reg, 16, 3, x))
// WRAPT [13:15] (3) - T component wrap factor (stage 0)
#define GX_BP_INDTEVSTAGE0_WRAPT_ST 13
#define GX_BP_INDTEVSTAGE0_WRAPT_END 15
#define GX_BP_INDTEVSTAGE0_WRAPT_SZ 3
#define GX_BP_INDTEVSTAGE0_WRAPT_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_INDTEVSTAGE0_WRAPT(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_INDTEVSTAGE0_WRAPT(reg, x) ((reg) = GX_BITSET(reg, 13, 3, x))
// UTCLOD [12:12] (1) - Whether to use unmodified texcoords for mipmaps (stage
// 0)
#define GX_BP_INDTEVSTAGE0_UTCLOD_ST 12
#define GX_BP_INDTEVSTAGE0_UTCLOD_END 12
#define GX_BP_INDTEVSTAGE0_UTCLOD_SZ 1
#define GX_BP_INDTEVSTAGE0_UTCLOD_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_INDTEVSTAGE0_UTCLOD(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_INDTEVSTAGE0_UTCLOD(reg, x) ((reg) = GX_BITSET(reg, 12, 1, x))
// ADDPREV [11:11] (1) - Whether to add in results from previous tev stage
// (stage 0)
#define GX_BP_INDTEVSTAGE0_ADDPREV_ST 11
#define GX_BP_INDTEVSTAGE0_ADDPREV_END 11
#define GX_BP_INDTEVSTAGE0_ADDPREV_SZ 1
#define GX_BP_INDTEVSTAGE0_ADDPREV_MASK (((1 << 1) - 1) << 31 - 11)
#define GX_BP_GET_INDTEVSTAGE0_ADDPREV(reg) GX_BITGET(reg, 11, 1)
#define GX_BP_SET_INDTEVSTAGE0_ADDPREV(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 11, 1, x))

/**
 * BP register 0x11 - IndTevStage1
 */
// STAGE [30:31] (2) - Indirect texture stage ID (stage 1)
#define GX_BP_INDTEVSTAGE1_STAGE_ST 30
#define GX_BP_INDTEVSTAGE1_STAGE_END 31
#define GX_BP_INDTEVSTAGE1_STAGE_SZ 2
#define GX_BP_INDTEVSTAGE1_STAGE_MASK (((1 << 2) - 1) << 31 - 31)
#define GX_BP_GET_INDTEVSTAGE1_STAGE(reg) GX_BITGET(reg, 30, 2)
#define GX_BP_SET_INDTEVSTAGE1_STAGE(reg, x) ((reg) = GX_BITSET(reg, 30, 2, x))
// FMT [28:29] (2) - Indirect texture format (stage 1)
#define GX_BP_INDTEVSTAGE1_FMT_ST 28
#define GX_BP_INDTEVSTAGE1_FMT_END 29
#define GX_BP_INDTEVSTAGE1_FMT_SZ 2
#define GX_BP_INDTEVSTAGE1_FMT_MASK (((1 << 2) - 1) << 31 - 29)
#define GX_BP_GET_INDTEVSTAGE1_FMT(reg) GX_BITGET(reg, 28, 2)
#define GX_BP_SET_INDTEVSTAGE1_FMT(reg, x) ((reg) = GX_BITSET(reg, 28, 2, x))
// BIAS [25:27] (3) - Indirect texture bias (stage 1)
#define GX_BP_INDTEVSTAGE1_BIAS_ST 25
#define GX_BP_INDTEVSTAGE1_BIAS_END 27
#define GX_BP_INDTEVSTAGE1_BIAS_SZ 3
#define GX_BP_INDTEVSTAGE1_BIAS_MASK (((1 << 3) - 1) << 31 - 27)
#define GX_BP_GET_INDTEVSTAGE1_BIAS(reg) GX_BITGET(reg, 25, 3)
#define GX_BP_SET_INDTEVSTAGE1_BIAS(reg, x) ((reg) = GX_BITSET(reg, 25, 3, x))
// ALPHA [23:24] (2) - Indirect texture alpha (stage 1)
#define GX_BP_INDTEVSTAGE1_ALPHA_ST 23
#define GX_BP_INDTEVSTAGE1_ALPHA_END 24
#define GX_BP_INDTEVSTAGE1_ALPHA_SZ 2
#define GX_BP_INDTEVSTAGE1_ALPHA_MASK (((1 << 2) - 1) << 31 - 24)
#define GX_BP_GET_INDTEVSTAGE1_ALPHA(reg) GX_BITGET(reg, 23, 2)
#define GX_BP_SET_INDTEVSTAGE1_ALPHA(reg, x) ((reg) = GX_BITSET(reg, 23, 2, x))
// MTX [19:22] (4) - Indirect texture matrix (stage 1)
#define GX_BP_INDTEVSTAGE1_MTX_ST 19
#define GX_BP_INDTEVSTAGE1_MTX_END 22
#define GX_BP_INDTEVSTAGE1_MTX_SZ 4
#define GX_BP_INDTEVSTAGE1_MTX_MASK (((1 << 4) - 1) << 31 - 22)
#define GX_BP_GET_INDTEVSTAGE1_MTX(reg) GX_BITGET(reg, 19, 4)
#define GX_BP_SET_INDTEVSTAGE1_MTX(reg, x) ((reg) = GX_BITSET(reg, 19, 4, x))
// WRAPS [16:18] (3) - S component wrap factor (stage 1)
#define GX_BP_INDTEVSTAGE1_WRAPS_ST 16
#define GX_BP_INDTEVSTAGE1_WRAPS_END 18
#define GX_BP_INDTEVSTAGE1_WRAPS_SZ 3
#define GX_BP_INDTEVSTAGE1_WRAPS_MASK (((1 << 3) - 1) << 31 - 18)
#define GX_BP_GET_INDTEVSTAGE1_WRAPS(reg) GX_BITGET(reg, 16, 3)
#define GX_BP_SET_INDTEVSTAGE1_WRAPS(reg, x) ((reg) = GX_BITSET(reg, 16, 3, x))
// WRAPT [13:15] (3) - T component wrap factor (stage 1)
#define GX_BP_INDTEVSTAGE1_WRAPT_ST 13
#define GX_BP_INDTEVSTAGE1_WRAPT_END 15
#define GX_BP_INDTEVSTAGE1_WRAPT_SZ 3
#define GX_BP_INDTEVSTAGE1_WRAPT_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_INDTEVSTAGE1_WRAPT(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_INDTEVSTAGE1_WRAPT(reg, x) ((reg) = GX_BITSET(reg, 13, 3, x))
// UTCLOD [12:12] (1) - Whether to use unmodified texcoords for mipmaps (stage
// 1)
#define GX_BP_INDTEVSTAGE1_UTCLOD_ST 12
#define GX_BP_INDTEVSTAGE1_UTCLOD_END 12
#define GX_BP_INDTEVSTAGE1_UTCLOD_SZ 1
#define GX_BP_INDTEVSTAGE1_UTCLOD_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_INDTEVSTAGE1_UTCLOD(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_INDTEVSTAGE1_UTCLOD(reg, x) ((reg) = GX_BITSET(reg, 12, 1, x))
// ADDPREV [11:11] (1) - Whether to add in results from previous tev stage
// (stage 1)
#define GX_BP_INDTEVSTAGE1_ADDPREV_ST 11
#define GX_BP_INDTEVSTAGE1_ADDPREV_END 11
#define GX_BP_INDTEVSTAGE1_ADDPREV_SZ 1
#define GX_BP_INDTEVSTAGE1_ADDPREV_MASK (((1 << 1) - 1) << 31 - 11)
#define GX_BP_GET_INDTEVSTAGE1_ADDPREV(reg) GX_BITGET(reg, 11, 1)
#define GX_BP_SET_INDTEVSTAGE1_ADDPREV(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 11, 1, x))

/**
 * BP register 0x12 - IndTevStage2
 */
// STAGE [30:31] (2) - Indirect texture stage ID (stage 2)
#define GX_BP_INDTEVSTAGE2_STAGE_ST 30
#define GX_BP_INDTEVSTAGE2_STAGE_END 31
#define GX_BP_INDTEVSTAGE2_STAGE_SZ 2
#define GX_BP_INDTEVSTAGE2_STAGE_MASK (((1 << 2) - 1) << 31 - 31)
#define GX_BP_GET_INDTEVSTAGE2_STAGE(reg) GX_BITGET(reg, 30, 2)
#define GX_BP_SET_INDTEVSTAGE2_STAGE(reg, x) ((reg) = GX_BITSET(reg, 30, 2, x))
// FMT [28:29] (2) - Indirect texture format (stage 2)
#define GX_BP_INDTEVSTAGE2_FMT_ST 28
#define GX_BP_INDTEVSTAGE2_FMT_END 29
#define GX_BP_INDTEVSTAGE2_FMT_SZ 2
#define GX_BP_INDTEVSTAGE2_FMT_MASK (((1 << 2) - 1) << 31 - 29)
#define GX_BP_GET_INDTEVSTAGE2_FMT(reg) GX_BITGET(reg, 28, 2)
#define GX_BP_SET_INDTEVSTAGE2_FMT(reg, x) ((reg) = GX_BITSET(reg, 28, 2, x))
// BIAS [25:27] (3) - Indirect texture bias (stage 2)
#define GX_BP_INDTEVSTAGE2_BIAS_ST 25
#define GX_BP_INDTEVSTAGE2_BIAS_END 27
#define GX_BP_INDTEVSTAGE2_BIAS_SZ 3
#define GX_BP_INDTEVSTAGE2_BIAS_MASK (((1 << 3) - 1) << 31 - 27)
#define GX_BP_GET_INDTEVSTAGE2_BIAS(reg) GX_BITGET(reg, 25, 3)
#define GX_BP_SET_INDTEVSTAGE2_BIAS(reg, x) ((reg) = GX_BITSET(reg, 25, 3, x))
// ALPHA [23:24] (2) - Indirect texture alpha (stage 2)
#define GX_BP_INDTEVSTAGE2_ALPHA_ST 23
#define GX_BP_INDTEVSTAGE2_ALPHA_END 24
#define GX_BP_INDTEVSTAGE2_ALPHA_SZ 2
#define GX_BP_INDTEVSTAGE2_ALPHA_MASK (((1 << 2) - 1) << 31 - 24)
#define GX_BP_GET_INDTEVSTAGE2_ALPHA(reg) GX_BITGET(reg, 23, 2)
#define GX_BP_SET_INDTEVSTAGE2_ALPHA(reg, x) ((reg) = GX_BITSET(reg, 23, 2, x))
// MTX [19:22] (4) - Indirect texture matrix (stage 2)
#define GX_BP_INDTEVSTAGE2_MTX_ST 19
#define GX_BP_INDTEVSTAGE2_MTX_END 22
#define GX_BP_INDTEVSTAGE2_MTX_SZ 4
#define GX_BP_INDTEVSTAGE2_MTX_MASK (((1 << 4) - 1) << 31 - 22)
#define GX_BP_GET_INDTEVSTAGE2_MTX(reg) GX_BITGET(reg, 19, 4)
#define GX_BP_SET_INDTEVSTAGE2_MTX(reg, x) ((reg) = GX_BITSET(reg, 19, 4, x))
// WRAPS [16:18] (3) - S component wrap factor (stage 2)
#define GX_BP_INDTEVSTAGE2_WRAPS_ST 16
#define GX_BP_INDTEVSTAGE2_WRAPS_END 18
#define GX_BP_INDTEVSTAGE2_WRAPS_SZ 3
#define GX_BP_INDTEVSTAGE2_WRAPS_MASK (((1 << 3) - 1) << 31 - 18)
#define GX_BP_GET_INDTEVSTAGE2_WRAPS(reg) GX_BITGET(reg, 16, 3)
#define GX_BP_SET_INDTEVSTAGE2_WRAPS(reg, x) ((reg) = GX_BITSET(reg, 16, 3, x))
// WRAPT [13:15] (3) - T component wrap factor (stage 2)
#define GX_BP_INDTEVSTAGE2_WRAPT_ST 13
#define GX_BP_INDTEVSTAGE2_WRAPT_END 15
#define GX_BP_INDTEVSTAGE2_WRAPT_SZ 3
#define GX_BP_INDTEVSTAGE2_WRAPT_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_INDTEVSTAGE2_WRAPT(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_INDTEVSTAGE2_WRAPT(reg, x) ((reg) = GX_BITSET(reg, 13, 3, x))
// UTCLOD [12:12] (1) - Whether to use unmodified texcoords for mipmaps (stage
// 2)
#define GX_BP_INDTEVSTAGE2_UTCLOD_ST 12
#define GX_BP_INDTEVSTAGE2_UTCLOD_END 12
#define GX_BP_INDTEVSTAGE2_UTCLOD_SZ 1
#define GX_BP_INDTEVSTAGE2_UTCLOD_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_INDTEVSTAGE2_UTCLOD(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_INDTEVSTAGE2_UTCLOD(reg, x) ((reg) = GX_BITSET(reg, 12, 1, x))
// ADDPREV [11:11] (1) - Whether to add in results from previous tev stage
// (stage 2)
#define GX_BP_INDTEVSTAGE2_ADDPREV_ST 11
#define GX_BP_INDTEVSTAGE2_ADDPREV_END 11
#define GX_BP_INDTEVSTAGE2_ADDPREV_SZ 1
#define GX_BP_INDTEVSTAGE2_ADDPREV_MASK (((1 << 1) - 1) << 31 - 11)
#define GX_BP_GET_INDTEVSTAGE2_ADDPREV(reg) GX_BITGET(reg, 11, 1)
#define GX_BP_SET_INDTEVSTAGE2_ADDPREV(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 11, 1, x))

/**
 * BP register 0x13 - IndTevStage3
 */
// STAGE [30:31] (2) - Indirect texture stage ID (stage 3)
#define GX_BP_INDTEVSTAGE3_STAGE_ST 30
#define GX_BP_INDTEVSTAGE3_STAGE_END 31
#define GX_BP_INDTEVSTAGE3_STAGE_SZ 2
#define GX_BP_INDTEVSTAGE3_STAGE_MASK (((1 << 2) - 1) << 31 - 31)
#define GX_BP_GET_INDTEVSTAGE3_STAGE(reg) GX_BITGET(reg, 30, 2)
#define GX_BP_SET_INDTEVSTAGE3_STAGE(reg, x) ((reg) = GX_BITSET(reg, 30, 2, x))
// FMT [28:29] (2) - Indirect texture format (stage 3)
#define GX_BP_INDTEVSTAGE3_FMT_ST 28
#define GX_BP_INDTEVSTAGE3_FMT_END 29
#define GX_BP_INDTEVSTAGE3_FMT_SZ 2
#define GX_BP_INDTEVSTAGE3_FMT_MASK (((1 << 2) - 1) << 31 - 29)
#define GX_BP_GET_INDTEVSTAGE3_FMT(reg) GX_BITGET(reg, 28, 2)
#define GX_BP_SET_INDTEVSTAGE3_FMT(reg, x) ((reg) = GX_BITSET(reg, 28, 2, x))
// BIAS [25:27] (3) - Indirect texture bias (stage 3)
#define GX_BP_INDTEVSTAGE3_BIAS_ST 25
#define GX_BP_INDTEVSTAGE3_BIAS_END 27
#define GX_BP_INDTEVSTAGE3_BIAS_SZ 3
#define GX_BP_INDTEVSTAGE3_BIAS_MASK (((1 << 3) - 1) << 31 - 27)
#define GX_BP_GET_INDTEVSTAGE3_BIAS(reg) GX_BITGET(reg, 25, 3)
#define GX_BP_SET_INDTEVSTAGE3_BIAS(reg, x) ((reg) = GX_BITSET(reg, 25, 3, x))
// ALPHA [23:24] (2) - Indirect texture alpha (stage 3)
#define GX_BP_INDTEVSTAGE3_ALPHA_ST 23
#define GX_BP_INDTEVSTAGE3_ALPHA_END 24
#define GX_BP_INDTEVSTAGE3_ALPHA_SZ 2
#define GX_BP_INDTEVSTAGE3_ALPHA_MASK (((1 << 2) - 1) << 31 - 24)
#define GX_BP_GET_INDTEVSTAGE3_ALPHA(reg) GX_BITGET(reg, 23, 2)
#define GX_BP_SET_INDTEVSTAGE3_ALPHA(reg, x) ((reg) = GX_BITSET(reg, 23, 2, x))
// MTX [19:22] (4) - Indirect texture matrix (stage 3)
#define GX_BP_INDTEVSTAGE3_MTX_ST 19
#define GX_BP_INDTEVSTAGE3_MTX_END 22
#define GX_BP_INDTEVSTAGE3_MTX_SZ 4
#define GX_BP_INDTEVSTAGE3_MTX_MASK (((1 << 4) - 1) << 31 - 22)
#define GX_BP_GET_INDTEVSTAGE3_MTX(reg) GX_BITGET(reg, 19, 4)
#define GX_BP_SET_INDTEVSTAGE3_MTX(reg, x) ((reg) = GX_BITSET(reg, 19, 4, x))
// WRAPS [16:18] (3) - S component wrap factor (stage 3)
#define GX_BP_INDTEVSTAGE3_WRAPS_ST 16
#define GX_BP_INDTEVSTAGE3_WRAPS_END 18
#define GX_BP_INDTEVSTAGE3_WRAPS_SZ 3
#define GX_BP_INDTEVSTAGE3_WRAPS_MASK (((1 << 3) - 1) << 31 - 18)
#define GX_BP_GET_INDTEVSTAGE3_WRAPS(reg) GX_BITGET(reg, 16, 3)
#define GX_BP_SET_INDTEVSTAGE3_WRAPS(reg, x) ((reg) = GX_BITSET(reg, 16, 3, x))
// WRAPT [13:15] (3) - T component wrap factor (stage 3)
#define GX_BP_INDTEVSTAGE3_WRAPT_ST 13
#define GX_BP_INDTEVSTAGE3_WRAPT_END 15
#define GX_BP_INDTEVSTAGE3_WRAPT_SZ 3
#define GX_BP_INDTEVSTAGE3_WRAPT_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_INDTEVSTAGE3_WRAPT(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_INDTEVSTAGE3_WRAPT(reg, x) ((reg) = GX_BITSET(reg, 13, 3, x))
// UTCLOD [12:12] (1) - Whether to use unmodified texcoords for mipmaps (stage
// 3)
#define GX_BP_INDTEVSTAGE3_UTCLOD_ST 12
#define GX_BP_INDTEVSTAGE3_UTCLOD_END 12
#define GX_BP_INDTEVSTAGE3_UTCLOD_SZ 1
#define GX_BP_INDTEVSTAGE3_UTCLOD_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_INDTEVSTAGE3_UTCLOD(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_INDTEVSTAGE3_UTCLOD(reg, x) ((reg) = GX_BITSET(reg, 12, 1, x))
// ADDPREV [11:11] (1) - Whether to add in results from previous tev stage
// (stage 3)
#define GX_BP_INDTEVSTAGE3_ADDPREV_ST 11
#define GX_BP_INDTEVSTAGE3_ADDPREV_END 11
#define GX_BP_INDTEVSTAGE3_ADDPREV_SZ 1
#define GX_BP_INDTEVSTAGE3_ADDPREV_MASK (((1 << 1) - 1) << 31 - 11)
#define GX_BP_GET_INDTEVSTAGE3_ADDPREV(reg) GX_BITGET(reg, 11, 1)
#define GX_BP_SET_INDTEVSTAGE3_ADDPREV(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 11, 1, x))

/**
 * BP register 0x14 - IndTevStage4
 */
// STAGE [30:31] (2) - Indirect texture stage ID (stage 4)
#define GX_BP_INDTEVSTAGE4_STAGE_ST 30
#define GX_BP_INDTEVSTAGE4_STAGE_END 31
#define GX_BP_INDTEVSTAGE4_STAGE_SZ 2
#define GX_BP_INDTEVSTAGE4_STAGE_MASK (((1 << 2) - 1) << 31 - 31)
#define GX_BP_GET_INDTEVSTAGE4_STAGE(reg) GX_BITGET(reg, 30, 2)
#define GX_BP_SET_INDTEVSTAGE4_STAGE(reg, x) ((reg) = GX_BITSET(reg, 30, 2, x))
// FMT [28:29] (2) - Indirect texture format (stage 4)
#define GX_BP_INDTEVSTAGE4_FMT_ST 28
#define GX_BP_INDTEVSTAGE4_FMT_END 29
#define GX_BP_INDTEVSTAGE4_FMT_SZ 2
#define GX_BP_INDTEVSTAGE4_FMT_MASK (((1 << 2) - 1) << 31 - 29)
#define GX_BP_GET_INDTEVSTAGE4_FMT(reg) GX_BITGET(reg, 28, 2)
#define GX_BP_SET_INDTEVSTAGE4_FMT(reg, x) ((reg) = GX_BITSET(reg, 28, 2, x))
// BIAS [25:27] (3) - Indirect texture bias (stage 4)
#define GX_BP_INDTEVSTAGE4_BIAS_ST 25
#define GX_BP_INDTEVSTAGE4_BIAS_END 27
#define GX_BP_INDTEVSTAGE4_BIAS_SZ 3
#define GX_BP_INDTEVSTAGE4_BIAS_MASK (((1 << 3) - 1) << 31 - 27)
#define GX_BP_GET_INDTEVSTAGE4_BIAS(reg) GX_BITGET(reg, 25, 3)
#define GX_BP_SET_INDTEVSTAGE4_BIAS(reg, x) ((reg) = GX_BITSET(reg, 25, 3, x))
// ALPHA [23:24] (2) - Indirect texture alpha (stage 4)
#define GX_BP_INDTEVSTAGE4_ALPHA_ST 23
#define GX_BP_INDTEVSTAGE4_ALPHA_END 24
#define GX_BP_INDTEVSTAGE4_ALPHA_SZ 2
#define GX_BP_INDTEVSTAGE4_ALPHA_MASK (((1 << 2) - 1) << 31 - 24)
#define GX_BP_GET_INDTEVSTAGE4_ALPHA(reg) GX_BITGET(reg, 23, 2)
#define GX_BP_SET_INDTEVSTAGE4_ALPHA(reg, x) ((reg) = GX_BITSET(reg, 23, 2, x))
// MTX [19:22] (4) - Indirect texture matrix (stage 4)
#define GX_BP_INDTEVSTAGE4_MTX_ST 19
#define GX_BP_INDTEVSTAGE4_MTX_END 22
#define GX_BP_INDTEVSTAGE4_MTX_SZ 4
#define GX_BP_INDTEVSTAGE4_MTX_MASK (((1 << 4) - 1) << 31 - 22)
#define GX_BP_GET_INDTEVSTAGE4_MTX(reg) GX_BITGET(reg, 19, 4)
#define GX_BP_SET_INDTEVSTAGE4_MTX(reg, x) ((reg) = GX_BITSET(reg, 19, 4, x))
// WRAPS [16:18] (3) - S component wrap factor (stage 4)
#define GX_BP_INDTEVSTAGE4_WRAPS_ST 16
#define GX_BP_INDTEVSTAGE4_WRAPS_END 18
#define GX_BP_INDTEVSTAGE4_WRAPS_SZ 3
#define GX_BP_INDTEVSTAGE4_WRAPS_MASK (((1 << 3) - 1) << 31 - 18)
#define GX_BP_GET_INDTEVSTAGE4_WRAPS(reg) GX_BITGET(reg, 16, 3)
#define GX_BP_SET_INDTEVSTAGE4_WRAPS(reg, x) ((reg) = GX_BITSET(reg, 16, 3, x))
// WRAPT [13:15] (3) - T component wrap factor (stage 4)
#define GX_BP_INDTEVSTAGE4_WRAPT_ST 13
#define GX_BP_INDTEVSTAGE4_WRAPT_END 15
#define GX_BP_INDTEVSTAGE4_WRAPT_SZ 3
#define GX_BP_INDTEVSTAGE4_WRAPT_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_INDTEVSTAGE4_WRAPT(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_INDTEVSTAGE4_WRAPT(reg, x) ((reg) = GX_BITSET(reg, 13, 3, x))
// UTCLOD [12:12] (1) - Whether to use unmodified texcoords for mipmaps (stage
// 4)
#define GX_BP_INDTEVSTAGE4_UTCLOD_ST 12
#define GX_BP_INDTEVSTAGE4_UTCLOD_END 12
#define GX_BP_INDTEVSTAGE4_UTCLOD_SZ 1
#define GX_BP_INDTEVSTAGE4_UTCLOD_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_INDTEVSTAGE4_UTCLOD(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_INDTEVSTAGE4_UTCLOD(reg, x) ((reg) = GX_BITSET(reg, 12, 1, x))
// ADDPREV [11:11] (1) - Whether to add in results from previous tev stage
// (stage 4)
#define GX_BP_INDTEVSTAGE4_ADDPREV_ST 11
#define GX_BP_INDTEVSTAGE4_ADDPREV_END 11
#define GX_BP_INDTEVSTAGE4_ADDPREV_SZ 1
#define GX_BP_INDTEVSTAGE4_ADDPREV_MASK (((1 << 1) - 1) << 31 - 11)
#define GX_BP_GET_INDTEVSTAGE4_ADDPREV(reg) GX_BITGET(reg, 11, 1)
#define GX_BP_SET_INDTEVSTAGE4_ADDPREV(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 11, 1, x))

/**
 * BP register 0x15 - IndTevStage5
 */
// STAGE [30:31] (2) - Indirect texture stage ID (stage 5)
#define GX_BP_INDTEVSTAGE5_STAGE_ST 30
#define GX_BP_INDTEVSTAGE5_STAGE_END 31
#define GX_BP_INDTEVSTAGE5_STAGE_SZ 2
#define GX_BP_INDTEVSTAGE5_STAGE_MASK (((1 << 2) - 1) << 31 - 31)
#define GX_BP_GET_INDTEVSTAGE5_STAGE(reg) GX_BITGET(reg, 30, 2)
#define GX_BP_SET_INDTEVSTAGE5_STAGE(reg, x) ((reg) = GX_BITSET(reg, 30, 2, x))
// FMT [28:29] (2) - Indirect texture format (stage 5)
#define GX_BP_INDTEVSTAGE5_FMT_ST 28
#define GX_BP_INDTEVSTAGE5_FMT_END 29
#define GX_BP_INDTEVSTAGE5_FMT_SZ 2
#define GX_BP_INDTEVSTAGE5_FMT_MASK (((1 << 2) - 1) << 31 - 29)
#define GX_BP_GET_INDTEVSTAGE5_FMT(reg) GX_BITGET(reg, 28, 2)
#define GX_BP_SET_INDTEVSTAGE5_FMT(reg, x) ((reg) = GX_BITSET(reg, 28, 2, x))
// BIAS [25:27] (3) - Indirect texture bias (stage 5)
#define GX_BP_INDTEVSTAGE5_BIAS_ST 25
#define GX_BP_INDTEVSTAGE5_BIAS_END 27
#define GX_BP_INDTEVSTAGE5_BIAS_SZ 3
#define GX_BP_INDTEVSTAGE5_BIAS_MASK (((1 << 3) - 1) << 31 - 27)
#define GX_BP_GET_INDTEVSTAGE5_BIAS(reg) GX_BITGET(reg, 25, 3)
#define GX_BP_SET_INDTEVSTAGE5_BIAS(reg, x) ((reg) = GX_BITSET(reg, 25, 3, x))
// ALPHA [23:24] (2) - Indirect texture alpha (stage 5)
#define GX_BP_INDTEVSTAGE5_ALPHA_ST 23
#define GX_BP_INDTEVSTAGE5_ALPHA_END 24
#define GX_BP_INDTEVSTAGE5_ALPHA_SZ 2
#define GX_BP_INDTEVSTAGE5_ALPHA_MASK (((1 << 2) - 1) << 31 - 24)
#define GX_BP_GET_INDTEVSTAGE5_ALPHA(reg) GX_BITGET(reg, 23, 2)
#define GX_BP_SET_INDTEVSTAGE5_ALPHA(reg, x) ((reg) = GX_BITSET(reg, 23, 2, x))
// MTX [19:22] (4) - Indirect texture matrix (stage 5)
#define GX_BP_INDTEVSTAGE5_MTX_ST 19
#define GX_BP_INDTEVSTAGE5_MTX_END 22
#define GX_BP_INDTEVSTAGE5_MTX_SZ 4
#define GX_BP_INDTEVSTAGE5_MTX_MASK (((1 << 4) - 1) << 31 - 22)
#define GX_BP_GET_INDTEVSTAGE5_MTX(reg) GX_BITGET(reg, 19, 4)
#define GX_BP_SET_INDTEVSTAGE5_MTX(reg, x) ((reg) = GX_BITSET(reg, 19, 4, x))
// WRAPS [16:18] (3) - S component wrap factor (stage 5)
#define GX_BP_INDTEVSTAGE5_WRAPS_ST 16
#define GX_BP_INDTEVSTAGE5_WRAPS_END 18
#define GX_BP_INDTEVSTAGE5_WRAPS_SZ 3
#define GX_BP_INDTEVSTAGE5_WRAPS_MASK (((1 << 3) - 1) << 31 - 18)
#define GX_BP_GET_INDTEVSTAGE5_WRAPS(reg) GX_BITGET(reg, 16, 3)
#define GX_BP_SET_INDTEVSTAGE5_WRAPS(reg, x) ((reg) = GX_BITSET(reg, 16, 3, x))
// WRAPT [13:15] (3) - T component wrap factor (stage 5)
#define GX_BP_INDTEVSTAGE5_WRAPT_ST 13
#define GX_BP_INDTEVSTAGE5_WRAPT_END 15
#define GX_BP_INDTEVSTAGE5_WRAPT_SZ 3
#define GX_BP_INDTEVSTAGE5_WRAPT_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_INDTEVSTAGE5_WRAPT(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_INDTEVSTAGE5_WRAPT(reg, x) ((reg) = GX_BITSET(reg, 13, 3, x))
// UTCLOD [12:12] (1) - Whether to use unmodified texcoords for mipmaps (stage
// 5)
#define GX_BP_INDTEVSTAGE5_UTCLOD_ST 12
#define GX_BP_INDTEVSTAGE5_UTCLOD_END 12
#define GX_BP_INDTEVSTAGE5_UTCLOD_SZ 1
#define GX_BP_INDTEVSTAGE5_UTCLOD_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_INDTEVSTAGE5_UTCLOD(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_INDTEVSTAGE5_UTCLOD(reg, x) ((reg) = GX_BITSET(reg, 12, 1, x))
// ADDPREV [11:11] (1) - Whether to add in results from previous tev stage
// (stage 5)
#define GX_BP_INDTEVSTAGE5_ADDPREV_ST 11
#define GX_BP_INDTEVSTAGE5_ADDPREV_END 11
#define GX_BP_INDTEVSTAGE5_ADDPREV_SZ 1
#define GX_BP_INDTEVSTAGE5_ADDPREV_MASK (((1 << 1) - 1) << 31 - 11)
#define GX_BP_GET_INDTEVSTAGE5_ADDPREV(reg) GX_BITGET(reg, 11, 1)
#define GX_BP_SET_INDTEVSTAGE5_ADDPREV(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 11, 1, x))

/**
 * BP register 0x16 - IndTevStage6
 */
// STAGE [30:31] (2) - Indirect texture stage ID (stage 6)
#define GX_BP_INDTEVSTAGE6_STAGE_ST 30
#define GX_BP_INDTEVSTAGE6_STAGE_END 31
#define GX_BP_INDTEVSTAGE6_STAGE_SZ 2
#define GX_BP_INDTEVSTAGE6_STAGE_MASK (((1 << 2) - 1) << 31 - 31)
#define GX_BP_GET_INDTEVSTAGE6_STAGE(reg) GX_BITGET(reg, 30, 2)
#define GX_BP_SET_INDTEVSTAGE6_STAGE(reg, x) ((reg) = GX_BITSET(reg, 30, 2, x))
// FMT [28:29] (2) - Indirect texture format (stage 6)
#define GX_BP_INDTEVSTAGE6_FMT_ST 28
#define GX_BP_INDTEVSTAGE6_FMT_END 29
#define GX_BP_INDTEVSTAGE6_FMT_SZ 2
#define GX_BP_INDTEVSTAGE6_FMT_MASK (((1 << 2) - 1) << 31 - 29)
#define GX_BP_GET_INDTEVSTAGE6_FMT(reg) GX_BITGET(reg, 28, 2)
#define GX_BP_SET_INDTEVSTAGE6_FMT(reg, x) ((reg) = GX_BITSET(reg, 28, 2, x))
// BIAS [25:27] (3) - Indirect texture bias (stage 6)
#define GX_BP_INDTEVSTAGE6_BIAS_ST 25
#define GX_BP_INDTEVSTAGE6_BIAS_END 27
#define GX_BP_INDTEVSTAGE6_BIAS_SZ 3
#define GX_BP_INDTEVSTAGE6_BIAS_MASK (((1 << 3) - 1) << 31 - 27)
#define GX_BP_GET_INDTEVSTAGE6_BIAS(reg) GX_BITGET(reg, 25, 3)
#define GX_BP_SET_INDTEVSTAGE6_BIAS(reg, x) ((reg) = GX_BITSET(reg, 25, 3, x))
// ALPHA [23:24] (2) - Indirect texture alpha (stage 6)
#define GX_BP_INDTEVSTAGE6_ALPHA_ST 23
#define GX_BP_INDTEVSTAGE6_ALPHA_END 24
#define GX_BP_INDTEVSTAGE6_ALPHA_SZ 2
#define GX_BP_INDTEVSTAGE6_ALPHA_MASK (((1 << 2) - 1) << 31 - 24)
#define GX_BP_GET_INDTEVSTAGE6_ALPHA(reg) GX_BITGET(reg, 23, 2)
#define GX_BP_SET_INDTEVSTAGE6_ALPHA(reg, x) ((reg) = GX_BITSET(reg, 23, 2, x))
// MTX [19:22] (4) - Indirect texture matrix (stage 6)
#define GX_BP_INDTEVSTAGE6_MTX_ST 19
#define GX_BP_INDTEVSTAGE6_MTX_END 22
#define GX_BP_INDTEVSTAGE6_MTX_SZ 4
#define GX_BP_INDTEVSTAGE6_MTX_MASK (((1 << 4) - 1) << 31 - 22)
#define GX_BP_GET_INDTEVSTAGE6_MTX(reg) GX_BITGET(reg, 19, 4)
#define GX_BP_SET_INDTEVSTAGE6_MTX(reg, x) ((reg) = GX_BITSET(reg, 19, 4, x))
// WRAPS [16:18] (3) - S component wrap factor (stage 6)
#define GX_BP_INDTEVSTAGE6_WRAPS_ST 16
#define GX_BP_INDTEVSTAGE6_WRAPS_END 18
#define GX_BP_INDTEVSTAGE6_WRAPS_SZ 3
#define GX_BP_INDTEVSTAGE6_WRAPS_MASK (((1 << 3) - 1) << 31 - 18)
#define GX_BP_GET_INDTEVSTAGE6_WRAPS(reg) GX_BITGET(reg, 16, 3)
#define GX_BP_SET_INDTEVSTAGE6_WRAPS(reg, x) ((reg) = GX_BITSET(reg, 16, 3, x))
// WRAPT [13:15] (3) - T component wrap factor (stage 6)
#define GX_BP_INDTEVSTAGE6_WRAPT_ST 13
#define GX_BP_INDTEVSTAGE6_WRAPT_END 15
#define GX_BP_INDTEVSTAGE6_WRAPT_SZ 3
#define GX_BP_INDTEVSTAGE6_WRAPT_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_INDTEVSTAGE6_WRAPT(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_INDTEVSTAGE6_WRAPT(reg, x) ((reg) = GX_BITSET(reg, 13, 3, x))
// UTCLOD [12:12] (1) - Whether to use unmodified texcoords for mipmaps (stage
// 6)
#define GX_BP_INDTEVSTAGE6_UTCLOD_ST 12
#define GX_BP_INDTEVSTAGE6_UTCLOD_END 12
#define GX_BP_INDTEVSTAGE6_UTCLOD_SZ 1
#define GX_BP_INDTEVSTAGE6_UTCLOD_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_INDTEVSTAGE6_UTCLOD(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_INDTEVSTAGE6_UTCLOD(reg, x) ((reg) = GX_BITSET(reg, 12, 1, x))
// ADDPREV [11:11] (1) - Whether to add in results from previous tev stage
// (stage 6)
#define GX_BP_INDTEVSTAGE6_ADDPREV_ST 11
#define GX_BP_INDTEVSTAGE6_ADDPREV_END 11
#define GX_BP_INDTEVSTAGE6_ADDPREV_SZ 1
#define GX_BP_INDTEVSTAGE6_ADDPREV_MASK (((1 << 1) - 1) << 31 - 11)
#define GX_BP_GET_INDTEVSTAGE6_ADDPREV(reg) GX_BITGET(reg, 11, 1)
#define GX_BP_SET_INDTEVSTAGE6_ADDPREV(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 11, 1, x))

/**
 * BP register 0x17 - IndTevStage7
 */
// STAGE [30:31] (2) - Indirect texture stage ID (stage 7)
#define GX_BP_INDTEVSTAGE7_STAGE_ST 30
#define GX_BP_INDTEVSTAGE7_STAGE_END 31
#define GX_BP_INDTEVSTAGE7_STAGE_SZ 2
#define GX_BP_INDTEVSTAGE7_STAGE_MASK (((1 << 2) - 1) << 31 - 31)
#define GX_BP_GET_INDTEVSTAGE7_STAGE(reg) GX_BITGET(reg, 30, 2)
#define GX_BP_SET_INDTEVSTAGE7_STAGE(reg, x) ((reg) = GX_BITSET(reg, 30, 2, x))
// FMT [28:29] (2) - Indirect texture format (stage 7)
#define GX_BP_INDTEVSTAGE7_FMT_ST 28
#define GX_BP_INDTEVSTAGE7_FMT_END 29
#define GX_BP_INDTEVSTAGE7_FMT_SZ 2
#define GX_BP_INDTEVSTAGE7_FMT_MASK (((1 << 2) - 1) << 31 - 29)
#define GX_BP_GET_INDTEVSTAGE7_FMT(reg) GX_BITGET(reg, 28, 2)
#define GX_BP_SET_INDTEVSTAGE7_FMT(reg, x) ((reg) = GX_BITSET(reg, 28, 2, x))
// BIAS [25:27] (3) - Indirect texture bias (stage 7)
#define GX_BP_INDTEVSTAGE7_BIAS_ST 25
#define GX_BP_INDTEVSTAGE7_BIAS_END 27
#define GX_BP_INDTEVSTAGE7_BIAS_SZ 3
#define GX_BP_INDTEVSTAGE7_BIAS_MASK (((1 << 3) - 1) << 31 - 27)
#define GX_BP_GET_INDTEVSTAGE7_BIAS(reg) GX_BITGET(reg, 25, 3)
#define GX_BP_SET_INDTEVSTAGE7_BIAS(reg, x) ((reg) = GX_BITSET(reg, 25, 3, x))
// ALPHA [23:24] (2) - Indirect texture alpha (stage 7)
#define GX_BP_INDTEVSTAGE7_ALPHA_ST 23
#define GX_BP_INDTEVSTAGE7_ALPHA_END 24
#define GX_BP_INDTEVSTAGE7_ALPHA_SZ 2
#define GX_BP_INDTEVSTAGE7_ALPHA_MASK (((1 << 2) - 1) << 31 - 24)
#define GX_BP_GET_INDTEVSTAGE7_ALPHA(reg) GX_BITGET(reg, 23, 2)
#define GX_BP_SET_INDTEVSTAGE7_ALPHA(reg, x) ((reg) = GX_BITSET(reg, 23, 2, x))
// MTX [19:22] (4) - Indirect texture matrix (stage 7)
#define GX_BP_INDTEVSTAGE7_MTX_ST 19
#define GX_BP_INDTEVSTAGE7_MTX_END 22
#define GX_BP_INDTEVSTAGE7_MTX_SZ 4
#define GX_BP_INDTEVSTAGE7_MTX_MASK (((1 << 4) - 1) << 31 - 22)
#define GX_BP_GET_INDTEVSTAGE7_MTX(reg) GX_BITGET(reg, 19, 4)
#define GX_BP_SET_INDTEVSTAGE7_MTX(reg, x) ((reg) = GX_BITSET(reg, 19, 4, x))
// WRAPS [16:18] (3) - S component wrap factor (stage 7)
#define GX_BP_INDTEVSTAGE7_WRAPS_ST 16
#define GX_BP_INDTEVSTAGE7_WRAPS_END 18
#define GX_BP_INDTEVSTAGE7_WRAPS_SZ 3
#define GX_BP_INDTEVSTAGE7_WRAPS_MASK (((1 << 3) - 1) << 31 - 18)
#define GX_BP_GET_INDTEVSTAGE7_WRAPS(reg) GX_BITGET(reg, 16, 3)
#define GX_BP_SET_INDTEVSTAGE7_WRAPS(reg, x) ((reg) = GX_BITSET(reg, 16, 3, x))
// WRAPT [13:15] (3) - T component wrap factor (stage 7)
#define GX_BP_INDTEVSTAGE7_WRAPT_ST 13
#define GX_BP_INDTEVSTAGE7_WRAPT_END 15
#define GX_BP_INDTEVSTAGE7_WRAPT_SZ 3
#define GX_BP_INDTEVSTAGE7_WRAPT_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_INDTEVSTAGE7_WRAPT(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_INDTEVSTAGE7_WRAPT(reg, x) ((reg) = GX_BITSET(reg, 13, 3, x))
// UTCLOD [12:12] (1) - Whether to use unmodified texcoords for mipmaps (stage
// 7)
#define GX_BP_INDTEVSTAGE7_UTCLOD_ST 12
#define GX_BP_INDTEVSTAGE7_UTCLOD_END 12
#define GX_BP_INDTEVSTAGE7_UTCLOD_SZ 1
#define GX_BP_INDTEVSTAGE7_UTCLOD_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_INDTEVSTAGE7_UTCLOD(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_INDTEVSTAGE7_UTCLOD(reg, x) ((reg) = GX_BITSET(reg, 12, 1, x))
// ADDPREV [11:11] (1) - Whether to add in results from previous tev stage
// (stage 7)
#define GX_BP_INDTEVSTAGE7_ADDPREV_ST 11
#define GX_BP_INDTEVSTAGE7_ADDPREV_END 11
#define GX_BP_INDTEVSTAGE7_ADDPREV_SZ 1
#define GX_BP_INDTEVSTAGE7_ADDPREV_MASK (((1 << 1) - 1) << 31 - 11)
#define GX_BP_GET_INDTEVSTAGE7_ADDPREV(reg) GX_BITGET(reg, 11, 1)
#define GX_BP_SET_INDTEVSTAGE7_ADDPREV(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 11, 1, x))

/**
 * BP register 0x18 - IndTevStage8
 */
// STAGE [30:31] (2) - Indirect texture stage ID (stage 8)
#define GX_BP_INDTEVSTAGE8_STAGE_ST 30
#define GX_BP_INDTEVSTAGE8_STAGE_END 31
#define GX_BP_INDTEVSTAGE8_STAGE_SZ 2
#define GX_BP_INDTEVSTAGE8_STAGE_MASK (((1 << 2) - 1) << 31 - 31)
#define GX_BP_GET_INDTEVSTAGE8_STAGE(reg) GX_BITGET(reg, 30, 2)
#define GX_BP_SET_INDTEVSTAGE8_STAGE(reg, x) ((reg) = GX_BITSET(reg, 30, 2, x))
// FMT [28:29] (2) - Indirect texture format (stage 8)
#define GX_BP_INDTEVSTAGE8_FMT_ST 28
#define GX_BP_INDTEVSTAGE8_FMT_END 29
#define GX_BP_INDTEVSTAGE8_FMT_SZ 2
#define GX_BP_INDTEVSTAGE8_FMT_MASK (((1 << 2) - 1) << 31 - 29)
#define GX_BP_GET_INDTEVSTAGE8_FMT(reg) GX_BITGET(reg, 28, 2)
#define GX_BP_SET_INDTEVSTAGE8_FMT(reg, x) ((reg) = GX_BITSET(reg, 28, 2, x))
// BIAS [25:27] (3) - Indirect texture bias (stage 8)
#define GX_BP_INDTEVSTAGE8_BIAS_ST 25
#define GX_BP_INDTEVSTAGE8_BIAS_END 27
#define GX_BP_INDTEVSTAGE8_BIAS_SZ 3
#define GX_BP_INDTEVSTAGE8_BIAS_MASK (((1 << 3) - 1) << 31 - 27)
#define GX_BP_GET_INDTEVSTAGE8_BIAS(reg) GX_BITGET(reg, 25, 3)
#define GX_BP_SET_INDTEVSTAGE8_BIAS(reg, x) ((reg) = GX_BITSET(reg, 25, 3, x))
// ALPHA [23:24] (2) - Indirect texture alpha (stage 8)
#define GX_BP_INDTEVSTAGE8_ALPHA_ST 23
#define GX_BP_INDTEVSTAGE8_ALPHA_END 24
#define GX_BP_INDTEVSTAGE8_ALPHA_SZ 2
#define GX_BP_INDTEVSTAGE8_ALPHA_MASK (((1 << 2) - 1) << 31 - 24)
#define GX_BP_GET_INDTEVSTAGE8_ALPHA(reg) GX_BITGET(reg, 23, 2)
#define GX_BP_SET_INDTEVSTAGE8_ALPHA(reg, x) ((reg) = GX_BITSET(reg, 23, 2, x))
// MTX [19:22] (4) - Indirect texture matrix (stage 8)
#define GX_BP_INDTEVSTAGE8_MTX_ST 19
#define GX_BP_INDTEVSTAGE8_MTX_END 22
#define GX_BP_INDTEVSTAGE8_MTX_SZ 4
#define GX_BP_INDTEVSTAGE8_MTX_MASK (((1 << 4) - 1) << 31 - 22)
#define GX_BP_GET_INDTEVSTAGE8_MTX(reg) GX_BITGET(reg, 19, 4)
#define GX_BP_SET_INDTEVSTAGE8_MTX(reg, x) ((reg) = GX_BITSET(reg, 19, 4, x))
// WRAPS [16:18] (3) - S component wrap factor (stage 8)
#define GX_BP_INDTEVSTAGE8_WRAPS_ST 16
#define GX_BP_INDTEVSTAGE8_WRAPS_END 18
#define GX_BP_INDTEVSTAGE8_WRAPS_SZ 3
#define GX_BP_INDTEVSTAGE8_WRAPS_MASK (((1 << 3) - 1) << 31 - 18)
#define GX_BP_GET_INDTEVSTAGE8_WRAPS(reg) GX_BITGET(reg, 16, 3)
#define GX_BP_SET_INDTEVSTAGE8_WRAPS(reg, x) ((reg) = GX_BITSET(reg, 16, 3, x))
// WRAPT [13:15] (3) - T component wrap factor (stage 8)
#define GX_BP_INDTEVSTAGE8_WRAPT_ST 13
#define GX_BP_INDTEVSTAGE8_WRAPT_END 15
#define GX_BP_INDTEVSTAGE8_WRAPT_SZ 3
#define GX_BP_INDTEVSTAGE8_WRAPT_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_INDTEVSTAGE8_WRAPT(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_INDTEVSTAGE8_WRAPT(reg, x) ((reg) = GX_BITSET(reg, 13, 3, x))
// UTCLOD [12:12] (1) - Whether to use unmodified texcoords for mipmaps (stage
// 8)
#define GX_BP_INDTEVSTAGE8_UTCLOD_ST 12
#define GX_BP_INDTEVSTAGE8_UTCLOD_END 12
#define GX_BP_INDTEVSTAGE8_UTCLOD_SZ 1
#define GX_BP_INDTEVSTAGE8_UTCLOD_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_INDTEVSTAGE8_UTCLOD(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_INDTEVSTAGE8_UTCLOD(reg, x) ((reg) = GX_BITSET(reg, 12, 1, x))
// ADDPREV [11:11] (1) - Whether to add in results from previous tev stage
// (stage 8)
#define GX_BP_INDTEVSTAGE8_ADDPREV_ST 11
#define GX_BP_INDTEVSTAGE8_ADDPREV_END 11
#define GX_BP_INDTEVSTAGE8_ADDPREV_SZ 1
#define GX_BP_INDTEVSTAGE8_ADDPREV_MASK (((1 << 1) - 1) << 31 - 11)
#define GX_BP_GET_INDTEVSTAGE8_ADDPREV(reg) GX_BITGET(reg, 11, 1)
#define GX_BP_SET_INDTEVSTAGE8_ADDPREV(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 11, 1, x))

/**
 * BP register 0x19 - IndTevStage9
 */
// STAGE [30:31] (2) - Indirect texture stage ID (stage 9)
#define GX_BP_INDTEVSTAGE9_STAGE_ST 30
#define GX_BP_INDTEVSTAGE9_STAGE_END 31
#define GX_BP_INDTEVSTAGE9_STAGE_SZ 2
#define GX_BP_INDTEVSTAGE9_STAGE_MASK (((1 << 2) - 1) << 31 - 31)
#define GX_BP_GET_INDTEVSTAGE9_STAGE(reg) GX_BITGET(reg, 30, 2)
#define GX_BP_SET_INDTEVSTAGE9_STAGE(reg, x) ((reg) = GX_BITSET(reg, 30, 2, x))
// FMT [28:29] (2) - Indirect texture format (stage 9)
#define GX_BP_INDTEVSTAGE9_FMT_ST 28
#define GX_BP_INDTEVSTAGE9_FMT_END 29
#define GX_BP_INDTEVSTAGE9_FMT_SZ 2
#define GX_BP_INDTEVSTAGE9_FMT_MASK (((1 << 2) - 1) << 31 - 29)
#define GX_BP_GET_INDTEVSTAGE9_FMT(reg) GX_BITGET(reg, 28, 2)
#define GX_BP_SET_INDTEVSTAGE9_FMT(reg, x) ((reg) = GX_BITSET(reg, 28, 2, x))
// BIAS [25:27] (3) - Indirect texture bias (stage 9)
#define GX_BP_INDTEVSTAGE9_BIAS_ST 25
#define GX_BP_INDTEVSTAGE9_BIAS_END 27
#define GX_BP_INDTEVSTAGE9_BIAS_SZ 3
#define GX_BP_INDTEVSTAGE9_BIAS_MASK (((1 << 3) - 1) << 31 - 27)
#define GX_BP_GET_INDTEVSTAGE9_BIAS(reg) GX_BITGET(reg, 25, 3)
#define GX_BP_SET_INDTEVSTAGE9_BIAS(reg, x) ((reg) = GX_BITSET(reg, 25, 3, x))
// ALPHA [23:24] (2) - Indirect texture alpha (stage 9)
#define GX_BP_INDTEVSTAGE9_ALPHA_ST 23
#define GX_BP_INDTEVSTAGE9_ALPHA_END 24
#define GX_BP_INDTEVSTAGE9_ALPHA_SZ 2
#define GX_BP_INDTEVSTAGE9_ALPHA_MASK (((1 << 2) - 1) << 31 - 24)
#define GX_BP_GET_INDTEVSTAGE9_ALPHA(reg) GX_BITGET(reg, 23, 2)
#define GX_BP_SET_INDTEVSTAGE9_ALPHA(reg, x) ((reg) = GX_BITSET(reg, 23, 2, x))
// MTX [19:22] (4) - Indirect texture matrix (stage 9)
#define GX_BP_INDTEVSTAGE9_MTX_ST 19
#define GX_BP_INDTEVSTAGE9_MTX_END 22
#define GX_BP_INDTEVSTAGE9_MTX_SZ 4
#define GX_BP_INDTEVSTAGE9_MTX_MASK (((1 << 4) - 1) << 31 - 22)
#define GX_BP_GET_INDTEVSTAGE9_MTX(reg) GX_BITGET(reg, 19, 4)
#define GX_BP_SET_INDTEVSTAGE9_MTX(reg, x) ((reg) = GX_BITSET(reg, 19, 4, x))
// WRAPS [16:18] (3) - S component wrap factor (stage 9)
#define GX_BP_INDTEVSTAGE9_WRAPS_ST 16
#define GX_BP_INDTEVSTAGE9_WRAPS_END 18
#define GX_BP_INDTEVSTAGE9_WRAPS_SZ 3
#define GX_BP_INDTEVSTAGE9_WRAPS_MASK (((1 << 3) - 1) << 31 - 18)
#define GX_BP_GET_INDTEVSTAGE9_WRAPS(reg) GX_BITGET(reg, 16, 3)
#define GX_BP_SET_INDTEVSTAGE9_WRAPS(reg, x) ((reg) = GX_BITSET(reg, 16, 3, x))
// WRAPT [13:15] (3) - T component wrap factor (stage 9)
#define GX_BP_INDTEVSTAGE9_WRAPT_ST 13
#define GX_BP_INDTEVSTAGE9_WRAPT_END 15
#define GX_BP_INDTEVSTAGE9_WRAPT_SZ 3
#define GX_BP_INDTEVSTAGE9_WRAPT_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_INDTEVSTAGE9_WRAPT(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_INDTEVSTAGE9_WRAPT(reg, x) ((reg) = GX_BITSET(reg, 13, 3, x))
// UTCLOD [12:12] (1) - Whether to use unmodified texcoords for mipmaps (stage
// 9)
#define GX_BP_INDTEVSTAGE9_UTCLOD_ST 12
#define GX_BP_INDTEVSTAGE9_UTCLOD_END 12
#define GX_BP_INDTEVSTAGE9_UTCLOD_SZ 1
#define GX_BP_INDTEVSTAGE9_UTCLOD_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_INDTEVSTAGE9_UTCLOD(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_INDTEVSTAGE9_UTCLOD(reg, x) ((reg) = GX_BITSET(reg, 12, 1, x))
// ADDPREV [11:11] (1) - Whether to add in results from previous tev stage
// (stage 9)
#define GX_BP_INDTEVSTAGE9_ADDPREV_ST 11
#define GX_BP_INDTEVSTAGE9_ADDPREV_END 11
#define GX_BP_INDTEVSTAGE9_ADDPREV_SZ 1
#define GX_BP_INDTEVSTAGE9_ADDPREV_MASK (((1 << 1) - 1) << 31 - 11)
#define GX_BP_GET_INDTEVSTAGE9_ADDPREV(reg) GX_BITGET(reg, 11, 1)
#define GX_BP_SET_INDTEVSTAGE9_ADDPREV(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 11, 1, x))

/**
 * BP register 0x1A - IndTevStage10
 */
// STAGE [30:31] (2) - Indirect texture stage ID (stage 10)
#define GX_BP_INDTEVSTAGE10_STAGE_ST 30
#define GX_BP_INDTEVSTAGE10_STAGE_END 31
#define GX_BP_INDTEVSTAGE10_STAGE_SZ 2
#define GX_BP_INDTEVSTAGE10_STAGE_MASK (((1 << 2) - 1) << 31 - 31)
#define GX_BP_GET_INDTEVSTAGE10_STAGE(reg) GX_BITGET(reg, 30, 2)
#define GX_BP_SET_INDTEVSTAGE10_STAGE(reg, x) ((reg) = GX_BITSET(reg, 30, 2, x))
// FMT [28:29] (2) - Indirect texture format (stage 10)
#define GX_BP_INDTEVSTAGE10_FMT_ST 28
#define GX_BP_INDTEVSTAGE10_FMT_END 29
#define GX_BP_INDTEVSTAGE10_FMT_SZ 2
#define GX_BP_INDTEVSTAGE10_FMT_MASK (((1 << 2) - 1) << 31 - 29)
#define GX_BP_GET_INDTEVSTAGE10_FMT(reg) GX_BITGET(reg, 28, 2)
#define GX_BP_SET_INDTEVSTAGE10_FMT(reg, x) ((reg) = GX_BITSET(reg, 28, 2, x))
// BIAS [25:27] (3) - Indirect texture bias (stage 10)
#define GX_BP_INDTEVSTAGE10_BIAS_ST 25
#define GX_BP_INDTEVSTAGE10_BIAS_END 27
#define GX_BP_INDTEVSTAGE10_BIAS_SZ 3
#define GX_BP_INDTEVSTAGE10_BIAS_MASK (((1 << 3) - 1) << 31 - 27)
#define GX_BP_GET_INDTEVSTAGE10_BIAS(reg) GX_BITGET(reg, 25, 3)
#define GX_BP_SET_INDTEVSTAGE10_BIAS(reg, x) ((reg) = GX_BITSET(reg, 25, 3, x))
// ALPHA [23:24] (2) - Indirect texture alpha (stage 10)
#define GX_BP_INDTEVSTAGE10_ALPHA_ST 23
#define GX_BP_INDTEVSTAGE10_ALPHA_END 24
#define GX_BP_INDTEVSTAGE10_ALPHA_SZ 2
#define GX_BP_INDTEVSTAGE10_ALPHA_MASK (((1 << 2) - 1) << 31 - 24)
#define GX_BP_GET_INDTEVSTAGE10_ALPHA(reg) GX_BITGET(reg, 23, 2)
#define GX_BP_SET_INDTEVSTAGE10_ALPHA(reg, x) ((reg) = GX_BITSET(reg, 23, 2, x))
// MTX [19:22] (4) - Indirect texture matrix (stage 10)
#define GX_BP_INDTEVSTAGE10_MTX_ST 19
#define GX_BP_INDTEVSTAGE10_MTX_END 22
#define GX_BP_INDTEVSTAGE10_MTX_SZ 4
#define GX_BP_INDTEVSTAGE10_MTX_MASK (((1 << 4) - 1) << 31 - 22)
#define GX_BP_GET_INDTEVSTAGE10_MTX(reg) GX_BITGET(reg, 19, 4)
#define GX_BP_SET_INDTEVSTAGE10_MTX(reg, x) ((reg) = GX_BITSET(reg, 19, 4, x))
// WRAPS [16:18] (3) - S component wrap factor (stage 10)
#define GX_BP_INDTEVSTAGE10_WRAPS_ST 16
#define GX_BP_INDTEVSTAGE10_WRAPS_END 18
#define GX_BP_INDTEVSTAGE10_WRAPS_SZ 3
#define GX_BP_INDTEVSTAGE10_WRAPS_MASK (((1 << 3) - 1) << 31 - 18)
#define GX_BP_GET_INDTEVSTAGE10_WRAPS(reg) GX_BITGET(reg, 16, 3)
#define GX_BP_SET_INDTEVSTAGE10_WRAPS(reg, x) ((reg) = GX_BITSET(reg, 16, 3, x))
// WRAPT [13:15] (3) - T component wrap factor (stage 10)
#define GX_BP_INDTEVSTAGE10_WRAPT_ST 13
#define GX_BP_INDTEVSTAGE10_WRAPT_END 15
#define GX_BP_INDTEVSTAGE10_WRAPT_SZ 3
#define GX_BP_INDTEVSTAGE10_WRAPT_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_INDTEVSTAGE10_WRAPT(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_INDTEVSTAGE10_WRAPT(reg, x) ((reg) = GX_BITSET(reg, 13, 3, x))
// UTCLOD [12:12] (1) - Whether to use unmodified texcoords for mipmaps (stage
// 10)
#define GX_BP_INDTEVSTAGE10_UTCLOD_ST 12
#define GX_BP_INDTEVSTAGE10_UTCLOD_END 12
#define GX_BP_INDTEVSTAGE10_UTCLOD_SZ 1
#define GX_BP_INDTEVSTAGE10_UTCLOD_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_INDTEVSTAGE10_UTCLOD(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_INDTEVSTAGE10_UTCLOD(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 12, 1, x))
// ADDPREV [11:11] (1) - Whether to add in results from previous tev stage
// (stage 10)
#define GX_BP_INDTEVSTAGE10_ADDPREV_ST 11
#define GX_BP_INDTEVSTAGE10_ADDPREV_END 11
#define GX_BP_INDTEVSTAGE10_ADDPREV_SZ 1
#define GX_BP_INDTEVSTAGE10_ADDPREV_MASK (((1 << 1) - 1) << 31 - 11)
#define GX_BP_GET_INDTEVSTAGE10_ADDPREV(reg) GX_BITGET(reg, 11, 1)
#define GX_BP_SET_INDTEVSTAGE10_ADDPREV(reg, x)                                \
    ((reg) = GX_BITSET(reg, 11, 1, x))

/**
 * BP register 0x1B - IndTevStage11
 */
// STAGE [30:31] (2) - Indirect texture stage ID (stage 11)
#define GX_BP_INDTEVSTAGE11_STAGE_ST 30
#define GX_BP_INDTEVSTAGE11_STAGE_END 31
#define GX_BP_INDTEVSTAGE11_STAGE_SZ 2
#define GX_BP_INDTEVSTAGE11_STAGE_MASK (((1 << 2) - 1) << 31 - 31)
#define GX_BP_GET_INDTEVSTAGE11_STAGE(reg) GX_BITGET(reg, 30, 2)
#define GX_BP_SET_INDTEVSTAGE11_STAGE(reg, x) ((reg) = GX_BITSET(reg, 30, 2, x))
// FMT [28:29] (2) - Indirect texture format (stage 11)
#define GX_BP_INDTEVSTAGE11_FMT_ST 28
#define GX_BP_INDTEVSTAGE11_FMT_END 29
#define GX_BP_INDTEVSTAGE11_FMT_SZ 2
#define GX_BP_INDTEVSTAGE11_FMT_MASK (((1 << 2) - 1) << 31 - 29)
#define GX_BP_GET_INDTEVSTAGE11_FMT(reg) GX_BITGET(reg, 28, 2)
#define GX_BP_SET_INDTEVSTAGE11_FMT(reg, x) ((reg) = GX_BITSET(reg, 28, 2, x))
// BIAS [25:27] (3) - Indirect texture bias (stage 11)
#define GX_BP_INDTEVSTAGE11_BIAS_ST 25
#define GX_BP_INDTEVSTAGE11_BIAS_END 27
#define GX_BP_INDTEVSTAGE11_BIAS_SZ 3
#define GX_BP_INDTEVSTAGE11_BIAS_MASK (((1 << 3) - 1) << 31 - 27)
#define GX_BP_GET_INDTEVSTAGE11_BIAS(reg) GX_BITGET(reg, 25, 3)
#define GX_BP_SET_INDTEVSTAGE11_BIAS(reg, x) ((reg) = GX_BITSET(reg, 25, 3, x))
// ALPHA [23:24] (2) - Indirect texture alpha (stage 11)
#define GX_BP_INDTEVSTAGE11_ALPHA_ST 23
#define GX_BP_INDTEVSTAGE11_ALPHA_END 24
#define GX_BP_INDTEVSTAGE11_ALPHA_SZ 2
#define GX_BP_INDTEVSTAGE11_ALPHA_MASK (((1 << 2) - 1) << 31 - 24)
#define GX_BP_GET_INDTEVSTAGE11_ALPHA(reg) GX_BITGET(reg, 23, 2)
#define GX_BP_SET_INDTEVSTAGE11_ALPHA(reg, x) ((reg) = GX_BITSET(reg, 23, 2, x))
// MTX [19:22] (4) - Indirect texture matrix (stage 11)
#define GX_BP_INDTEVSTAGE11_MTX_ST 19
#define GX_BP_INDTEVSTAGE11_MTX_END 22
#define GX_BP_INDTEVSTAGE11_MTX_SZ 4
#define GX_BP_INDTEVSTAGE11_MTX_MASK (((1 << 4) - 1) << 31 - 22)
#define GX_BP_GET_INDTEVSTAGE11_MTX(reg) GX_BITGET(reg, 19, 4)
#define GX_BP_SET_INDTEVSTAGE11_MTX(reg, x) ((reg) = GX_BITSET(reg, 19, 4, x))
// WRAPS [16:18] (3) - S component wrap factor (stage 11)
#define GX_BP_INDTEVSTAGE11_WRAPS_ST 16
#define GX_BP_INDTEVSTAGE11_WRAPS_END 18
#define GX_BP_INDTEVSTAGE11_WRAPS_SZ 3
#define GX_BP_INDTEVSTAGE11_WRAPS_MASK (((1 << 3) - 1) << 31 - 18)
#define GX_BP_GET_INDTEVSTAGE11_WRAPS(reg) GX_BITGET(reg, 16, 3)
#define GX_BP_SET_INDTEVSTAGE11_WRAPS(reg, x) ((reg) = GX_BITSET(reg, 16, 3, x))
// WRAPT [13:15] (3) - T component wrap factor (stage 11)
#define GX_BP_INDTEVSTAGE11_WRAPT_ST 13
#define GX_BP_INDTEVSTAGE11_WRAPT_END 15
#define GX_BP_INDTEVSTAGE11_WRAPT_SZ 3
#define GX_BP_INDTEVSTAGE11_WRAPT_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_INDTEVSTAGE11_WRAPT(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_INDTEVSTAGE11_WRAPT(reg, x) ((reg) = GX_BITSET(reg, 13, 3, x))
// UTCLOD [12:12] (1) - Whether to use unmodified texcoords for mipmaps (stage
// 11)
#define GX_BP_INDTEVSTAGE11_UTCLOD_ST 12
#define GX_BP_INDTEVSTAGE11_UTCLOD_END 12
#define GX_BP_INDTEVSTAGE11_UTCLOD_SZ 1
#define GX_BP_INDTEVSTAGE11_UTCLOD_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_INDTEVSTAGE11_UTCLOD(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_INDTEVSTAGE11_UTCLOD(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 12, 1, x))
// ADDPREV [11:11] (1) - Whether to add in results from previous tev stage
// (stage 11)
#define GX_BP_INDTEVSTAGE11_ADDPREV_ST 11
#define GX_BP_INDTEVSTAGE11_ADDPREV_END 11
#define GX_BP_INDTEVSTAGE11_ADDPREV_SZ 1
#define GX_BP_INDTEVSTAGE11_ADDPREV_MASK (((1 << 1) - 1) << 31 - 11)
#define GX_BP_GET_INDTEVSTAGE11_ADDPREV(reg) GX_BITGET(reg, 11, 1)
#define GX_BP_SET_INDTEVSTAGE11_ADDPREV(reg, x)                                \
    ((reg) = GX_BITSET(reg, 11, 1, x))

/**
 * BP register 0x1C - IndTevStage12
 */
// STAGE [30:31] (2) - Indirect texture stage ID (stage 12)
#define GX_BP_INDTEVSTAGE12_STAGE_ST 30
#define GX_BP_INDTEVSTAGE12_STAGE_END 31
#define GX_BP_INDTEVSTAGE12_STAGE_SZ 2
#define GX_BP_INDTEVSTAGE12_STAGE_MASK (((1 << 2) - 1) << 31 - 31)
#define GX_BP_GET_INDTEVSTAGE12_STAGE(reg) GX_BITGET(reg, 30, 2)
#define GX_BP_SET_INDTEVSTAGE12_STAGE(reg, x) ((reg) = GX_BITSET(reg, 30, 2, x))
// FMT [28:29] (2) - Indirect texture format (stage 12)
#define GX_BP_INDTEVSTAGE12_FMT_ST 28
#define GX_BP_INDTEVSTAGE12_FMT_END 29
#define GX_BP_INDTEVSTAGE12_FMT_SZ 2
#define GX_BP_INDTEVSTAGE12_FMT_MASK (((1 << 2) - 1) << 31 - 29)
#define GX_BP_GET_INDTEVSTAGE12_FMT(reg) GX_BITGET(reg, 28, 2)
#define GX_BP_SET_INDTEVSTAGE12_FMT(reg, x) ((reg) = GX_BITSET(reg, 28, 2, x))
// BIAS [25:27] (3) - Indirect texture bias (stage 12)
#define GX_BP_INDTEVSTAGE12_BIAS_ST 25
#define GX_BP_INDTEVSTAGE12_BIAS_END 27
#define GX_BP_INDTEVSTAGE12_BIAS_SZ 3
#define GX_BP_INDTEVSTAGE12_BIAS_MASK (((1 << 3) - 1) << 31 - 27)
#define GX_BP_GET_INDTEVSTAGE12_BIAS(reg) GX_BITGET(reg, 25, 3)
#define GX_BP_SET_INDTEVSTAGE12_BIAS(reg, x) ((reg) = GX_BITSET(reg, 25, 3, x))
// ALPHA [23:24] (2) - Indirect texture alpha (stage 12)
#define GX_BP_INDTEVSTAGE12_ALPHA_ST 23
#define GX_BP_INDTEVSTAGE12_ALPHA_END 24
#define GX_BP_INDTEVSTAGE12_ALPHA_SZ 2
#define GX_BP_INDTEVSTAGE12_ALPHA_MASK (((1 << 2) - 1) << 31 - 24)
#define GX_BP_GET_INDTEVSTAGE12_ALPHA(reg) GX_BITGET(reg, 23, 2)
#define GX_BP_SET_INDTEVSTAGE12_ALPHA(reg, x) ((reg) = GX_BITSET(reg, 23, 2, x))
// MTX [19:22] (4) - Indirect texture matrix (stage 12)
#define GX_BP_INDTEVSTAGE12_MTX_ST 19
#define GX_BP_INDTEVSTAGE12_MTX_END 22
#define GX_BP_INDTEVSTAGE12_MTX_SZ 4
#define GX_BP_INDTEVSTAGE12_MTX_MASK (((1 << 4) - 1) << 31 - 22)
#define GX_BP_GET_INDTEVSTAGE12_MTX(reg) GX_BITGET(reg, 19, 4)
#define GX_BP_SET_INDTEVSTAGE12_MTX(reg, x) ((reg) = GX_BITSET(reg, 19, 4, x))
// WRAPS [16:18] (3) - S component wrap factor (stage 12)
#define GX_BP_INDTEVSTAGE12_WRAPS_ST 16
#define GX_BP_INDTEVSTAGE12_WRAPS_END 18
#define GX_BP_INDTEVSTAGE12_WRAPS_SZ 3
#define GX_BP_INDTEVSTAGE12_WRAPS_MASK (((1 << 3) - 1) << 31 - 18)
#define GX_BP_GET_INDTEVSTAGE12_WRAPS(reg) GX_BITGET(reg, 16, 3)
#define GX_BP_SET_INDTEVSTAGE12_WRAPS(reg, x) ((reg) = GX_BITSET(reg, 16, 3, x))
// WRAPT [13:15] (3) - T component wrap factor (stage 12)
#define GX_BP_INDTEVSTAGE12_WRAPT_ST 13
#define GX_BP_INDTEVSTAGE12_WRAPT_END 15
#define GX_BP_INDTEVSTAGE12_WRAPT_SZ 3
#define GX_BP_INDTEVSTAGE12_WRAPT_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_INDTEVSTAGE12_WRAPT(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_INDTEVSTAGE12_WRAPT(reg, x) ((reg) = GX_BITSET(reg, 13, 3, x))
// UTCLOD [12:12] (1) - Whether to use unmodified texcoords for mipmaps (stage
// 12)
#define GX_BP_INDTEVSTAGE12_UTCLOD_ST 12
#define GX_BP_INDTEVSTAGE12_UTCLOD_END 12
#define GX_BP_INDTEVSTAGE12_UTCLOD_SZ 1
#define GX_BP_INDTEVSTAGE12_UTCLOD_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_INDTEVSTAGE12_UTCLOD(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_INDTEVSTAGE12_UTCLOD(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 12, 1, x))
// ADDPREV [11:11] (1) - Whether to add in results from previous tev stage
// (stage 12)
#define GX_BP_INDTEVSTAGE12_ADDPREV_ST 11
#define GX_BP_INDTEVSTAGE12_ADDPREV_END 11
#define GX_BP_INDTEVSTAGE12_ADDPREV_SZ 1
#define GX_BP_INDTEVSTAGE12_ADDPREV_MASK (((1 << 1) - 1) << 31 - 11)
#define GX_BP_GET_INDTEVSTAGE12_ADDPREV(reg) GX_BITGET(reg, 11, 1)
#define GX_BP_SET_INDTEVSTAGE12_ADDPREV(reg, x)                                \
    ((reg) = GX_BITSET(reg, 11, 1, x))

/**
 * BP register 0x1D - IndTevStage13
 */
// STAGE [30:31] (2) - Indirect texture stage ID (stage 13)
#define GX_BP_INDTEVSTAGE13_STAGE_ST 30
#define GX_BP_INDTEVSTAGE13_STAGE_END 31
#define GX_BP_INDTEVSTAGE13_STAGE_SZ 2
#define GX_BP_INDTEVSTAGE13_STAGE_MASK (((1 << 2) - 1) << 31 - 31)
#define GX_BP_GET_INDTEVSTAGE13_STAGE(reg) GX_BITGET(reg, 30, 2)
#define GX_BP_SET_INDTEVSTAGE13_STAGE(reg, x) ((reg) = GX_BITSET(reg, 30, 2, x))
// FMT [28:29] (2) - Indirect texture format (stage 13)
#define GX_BP_INDTEVSTAGE13_FMT_ST 28
#define GX_BP_INDTEVSTAGE13_FMT_END 29
#define GX_BP_INDTEVSTAGE13_FMT_SZ 2
#define GX_BP_INDTEVSTAGE13_FMT_MASK (((1 << 2) - 1) << 31 - 29)
#define GX_BP_GET_INDTEVSTAGE13_FMT(reg) GX_BITGET(reg, 28, 2)
#define GX_BP_SET_INDTEVSTAGE13_FMT(reg, x) ((reg) = GX_BITSET(reg, 28, 2, x))
// BIAS [25:27] (3) - Indirect texture bias (stage 13)
#define GX_BP_INDTEVSTAGE13_BIAS_ST 25
#define GX_BP_INDTEVSTAGE13_BIAS_END 27
#define GX_BP_INDTEVSTAGE13_BIAS_SZ 3
#define GX_BP_INDTEVSTAGE13_BIAS_MASK (((1 << 3) - 1) << 31 - 27)
#define GX_BP_GET_INDTEVSTAGE13_BIAS(reg) GX_BITGET(reg, 25, 3)
#define GX_BP_SET_INDTEVSTAGE13_BIAS(reg, x) ((reg) = GX_BITSET(reg, 25, 3, x))
// ALPHA [23:24] (2) - Indirect texture alpha (stage 13)
#define GX_BP_INDTEVSTAGE13_ALPHA_ST 23
#define GX_BP_INDTEVSTAGE13_ALPHA_END 24
#define GX_BP_INDTEVSTAGE13_ALPHA_SZ 2
#define GX_BP_INDTEVSTAGE13_ALPHA_MASK (((1 << 2) - 1) << 31 - 24)
#define GX_BP_GET_INDTEVSTAGE13_ALPHA(reg) GX_BITGET(reg, 23, 2)
#define GX_BP_SET_INDTEVSTAGE13_ALPHA(reg, x) ((reg) = GX_BITSET(reg, 23, 2, x))
// MTX [19:22] (4) - Indirect texture matrix (stage 13)
#define GX_BP_INDTEVSTAGE13_MTX_ST 19
#define GX_BP_INDTEVSTAGE13_MTX_END 22
#define GX_BP_INDTEVSTAGE13_MTX_SZ 4
#define GX_BP_INDTEVSTAGE13_MTX_MASK (((1 << 4) - 1) << 31 - 22)
#define GX_BP_GET_INDTEVSTAGE13_MTX(reg) GX_BITGET(reg, 19, 4)
#define GX_BP_SET_INDTEVSTAGE13_MTX(reg, x) ((reg) = GX_BITSET(reg, 19, 4, x))
// WRAPS [16:18] (3) - S component wrap factor (stage 13)
#define GX_BP_INDTEVSTAGE13_WRAPS_ST 16
#define GX_BP_INDTEVSTAGE13_WRAPS_END 18
#define GX_BP_INDTEVSTAGE13_WRAPS_SZ 3
#define GX_BP_INDTEVSTAGE13_WRAPS_MASK (((1 << 3) - 1) << 31 - 18)
#define GX_BP_GET_INDTEVSTAGE13_WRAPS(reg) GX_BITGET(reg, 16, 3)
#define GX_BP_SET_INDTEVSTAGE13_WRAPS(reg, x) ((reg) = GX_BITSET(reg, 16, 3, x))
// WRAPT [13:15] (3) - T component wrap factor (stage 13)
#define GX_BP_INDTEVSTAGE13_WRAPT_ST 13
#define GX_BP_INDTEVSTAGE13_WRAPT_END 15
#define GX_BP_INDTEVSTAGE13_WRAPT_SZ 3
#define GX_BP_INDTEVSTAGE13_WRAPT_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_INDTEVSTAGE13_WRAPT(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_INDTEVSTAGE13_WRAPT(reg, x) ((reg) = GX_BITSET(reg, 13, 3, x))
// UTCLOD [12:12] (1) - Whether to use unmodified texcoords for mipmaps (stage
// 13)
#define GX_BP_INDTEVSTAGE13_UTCLOD_ST 12
#define GX_BP_INDTEVSTAGE13_UTCLOD_END 12
#define GX_BP_INDTEVSTAGE13_UTCLOD_SZ 1
#define GX_BP_INDTEVSTAGE13_UTCLOD_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_INDTEVSTAGE13_UTCLOD(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_INDTEVSTAGE13_UTCLOD(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 12, 1, x))
// ADDPREV [11:11] (1) - Whether to add in results from previous tev stage
// (stage 13)
#define GX_BP_INDTEVSTAGE13_ADDPREV_ST 11
#define GX_BP_INDTEVSTAGE13_ADDPREV_END 11
#define GX_BP_INDTEVSTAGE13_ADDPREV_SZ 1
#define GX_BP_INDTEVSTAGE13_ADDPREV_MASK (((1 << 1) - 1) << 31 - 11)
#define GX_BP_GET_INDTEVSTAGE13_ADDPREV(reg) GX_BITGET(reg, 11, 1)
#define GX_BP_SET_INDTEVSTAGE13_ADDPREV(reg, x)                                \
    ((reg) = GX_BITSET(reg, 11, 1, x))

/**
 * BP register 0x1E - IndTevStage14
 */
// STAGE [30:31] (2) - Indirect texture stage ID (stage 14)
#define GX_BP_INDTEVSTAGE14_STAGE_ST 30
#define GX_BP_INDTEVSTAGE14_STAGE_END 31
#define GX_BP_INDTEVSTAGE14_STAGE_SZ 2
#define GX_BP_INDTEVSTAGE14_STAGE_MASK (((1 << 2) - 1) << 31 - 31)
#define GX_BP_GET_INDTEVSTAGE14_STAGE(reg) GX_BITGET(reg, 30, 2)
#define GX_BP_SET_INDTEVSTAGE14_STAGE(reg, x) ((reg) = GX_BITSET(reg, 30, 2, x))
// FMT [28:29] (2) - Indirect texture format (stage 14)
#define GX_BP_INDTEVSTAGE14_FMT_ST 28
#define GX_BP_INDTEVSTAGE14_FMT_END 29
#define GX_BP_INDTEVSTAGE14_FMT_SZ 2
#define GX_BP_INDTEVSTAGE14_FMT_MASK (((1 << 2) - 1) << 31 - 29)
#define GX_BP_GET_INDTEVSTAGE14_FMT(reg) GX_BITGET(reg, 28, 2)
#define GX_BP_SET_INDTEVSTAGE14_FMT(reg, x) ((reg) = GX_BITSET(reg, 28, 2, x))
// BIAS [25:27] (3) - Indirect texture bias (stage 14)
#define GX_BP_INDTEVSTAGE14_BIAS_ST 25
#define GX_BP_INDTEVSTAGE14_BIAS_END 27
#define GX_BP_INDTEVSTAGE14_BIAS_SZ 3
#define GX_BP_INDTEVSTAGE14_BIAS_MASK (((1 << 3) - 1) << 31 - 27)
#define GX_BP_GET_INDTEVSTAGE14_BIAS(reg) GX_BITGET(reg, 25, 3)
#define GX_BP_SET_INDTEVSTAGE14_BIAS(reg, x) ((reg) = GX_BITSET(reg, 25, 3, x))
// ALPHA [23:24] (2) - Indirect texture alpha (stage 14)
#define GX_BP_INDTEVSTAGE14_ALPHA_ST 23
#define GX_BP_INDTEVSTAGE14_ALPHA_END 24
#define GX_BP_INDTEVSTAGE14_ALPHA_SZ 2
#define GX_BP_INDTEVSTAGE14_ALPHA_MASK (((1 << 2) - 1) << 31 - 24)
#define GX_BP_GET_INDTEVSTAGE14_ALPHA(reg) GX_BITGET(reg, 23, 2)
#define GX_BP_SET_INDTEVSTAGE14_ALPHA(reg, x) ((reg) = GX_BITSET(reg, 23, 2, x))
// MTX [19:22] (4) - Indirect texture matrix (stage 14)
#define GX_BP_INDTEVSTAGE14_MTX_ST 19
#define GX_BP_INDTEVSTAGE14_MTX_END 22
#define GX_BP_INDTEVSTAGE14_MTX_SZ 4
#define GX_BP_INDTEVSTAGE14_MTX_MASK (((1 << 4) - 1) << 31 - 22)
#define GX_BP_GET_INDTEVSTAGE14_MTX(reg) GX_BITGET(reg, 19, 4)
#define GX_BP_SET_INDTEVSTAGE14_MTX(reg, x) ((reg) = GX_BITSET(reg, 19, 4, x))
// WRAPS [16:18] (3) - S component wrap factor (stage 14)
#define GX_BP_INDTEVSTAGE14_WRAPS_ST 16
#define GX_BP_INDTEVSTAGE14_WRAPS_END 18
#define GX_BP_INDTEVSTAGE14_WRAPS_SZ 3
#define GX_BP_INDTEVSTAGE14_WRAPS_MASK (((1 << 3) - 1) << 31 - 18)
#define GX_BP_GET_INDTEVSTAGE14_WRAPS(reg) GX_BITGET(reg, 16, 3)
#define GX_BP_SET_INDTEVSTAGE14_WRAPS(reg, x) ((reg) = GX_BITSET(reg, 16, 3, x))
// WRAPT [13:15] (3) - T component wrap factor (stage 14)
#define GX_BP_INDTEVSTAGE14_WRAPT_ST 13
#define GX_BP_INDTEVSTAGE14_WRAPT_END 15
#define GX_BP_INDTEVSTAGE14_WRAPT_SZ 3
#define GX_BP_INDTEVSTAGE14_WRAPT_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_INDTEVSTAGE14_WRAPT(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_INDTEVSTAGE14_WRAPT(reg, x) ((reg) = GX_BITSET(reg, 13, 3, x))
// UTCLOD [12:12] (1) - Whether to use unmodified texcoords for mipmaps (stage
// 14)
#define GX_BP_INDTEVSTAGE14_UTCLOD_ST 12
#define GX_BP_INDTEVSTAGE14_UTCLOD_END 12
#define GX_BP_INDTEVSTAGE14_UTCLOD_SZ 1
#define GX_BP_INDTEVSTAGE14_UTCLOD_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_INDTEVSTAGE14_UTCLOD(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_INDTEVSTAGE14_UTCLOD(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 12, 1, x))
// ADDPREV [11:11] (1) - Whether to add in results from previous tev stage
// (stage 14)
#define GX_BP_INDTEVSTAGE14_ADDPREV_ST 11
#define GX_BP_INDTEVSTAGE14_ADDPREV_END 11
#define GX_BP_INDTEVSTAGE14_ADDPREV_SZ 1
#define GX_BP_INDTEVSTAGE14_ADDPREV_MASK (((1 << 1) - 1) << 31 - 11)
#define GX_BP_GET_INDTEVSTAGE14_ADDPREV(reg) GX_BITGET(reg, 11, 1)
#define GX_BP_SET_INDTEVSTAGE14_ADDPREV(reg, x)                                \
    ((reg) = GX_BITSET(reg, 11, 1, x))

/**
 * BP register 0x1F - IndTevStage15
 */
// STAGE [30:31] (2) - Indirect texture stage ID (stage 15)
#define GX_BP_INDTEVSTAGE15_STAGE_ST 30
#define GX_BP_INDTEVSTAGE15_STAGE_END 31
#define GX_BP_INDTEVSTAGE15_STAGE_SZ 2
#define GX_BP_INDTEVSTAGE15_STAGE_MASK (((1 << 2) - 1) << 31 - 31)
#define GX_BP_GET_INDTEVSTAGE15_STAGE(reg) GX_BITGET(reg, 30, 2)
#define GX_BP_SET_INDTEVSTAGE15_STAGE(reg, x) ((reg) = GX_BITSET(reg, 30, 2, x))
// FMT [28:29] (2) - Indirect texture format (stage 15)
#define GX_BP_INDTEVSTAGE15_FMT_ST 28
#define GX_BP_INDTEVSTAGE15_FMT_END 29
#define GX_BP_INDTEVSTAGE15_FMT_SZ 2
#define GX_BP_INDTEVSTAGE15_FMT_MASK (((1 << 2) - 1) << 31 - 29)
#define GX_BP_GET_INDTEVSTAGE15_FMT(reg) GX_BITGET(reg, 28, 2)
#define GX_BP_SET_INDTEVSTAGE15_FMT(reg, x) ((reg) = GX_BITSET(reg, 28, 2, x))
// BIAS [25:27] (3) - Indirect texture bias (stage 15)
#define GX_BP_INDTEVSTAGE15_BIAS_ST 25
#define GX_BP_INDTEVSTAGE15_BIAS_END 27
#define GX_BP_INDTEVSTAGE15_BIAS_SZ 3
#define GX_BP_INDTEVSTAGE15_BIAS_MASK (((1 << 3) - 1) << 31 - 27)
#define GX_BP_GET_INDTEVSTAGE15_BIAS(reg) GX_BITGET(reg, 25, 3)
#define GX_BP_SET_INDTEVSTAGE15_BIAS(reg, x) ((reg) = GX_BITSET(reg, 25, 3, x))
// ALPHA [23:24] (2) - Indirect texture alpha (stage 15)
#define GX_BP_INDTEVSTAGE15_ALPHA_ST 23
#define GX_BP_INDTEVSTAGE15_ALPHA_END 24
#define GX_BP_INDTEVSTAGE15_ALPHA_SZ 2
#define GX_BP_INDTEVSTAGE15_ALPHA_MASK (((1 << 2) - 1) << 31 - 24)
#define GX_BP_GET_INDTEVSTAGE15_ALPHA(reg) GX_BITGET(reg, 23, 2)
#define GX_BP_SET_INDTEVSTAGE15_ALPHA(reg, x) ((reg) = GX_BITSET(reg, 23, 2, x))
// MTX [19:22] (4) - Indirect texture matrix (stage 15)
#define GX_BP_INDTEVSTAGE15_MTX_ST 19
#define GX_BP_INDTEVSTAGE15_MTX_END 22
#define GX_BP_INDTEVSTAGE15_MTX_SZ 4
#define GX_BP_INDTEVSTAGE15_MTX_MASK (((1 << 4) - 1) << 31 - 22)
#define GX_BP_GET_INDTEVSTAGE15_MTX(reg) GX_BITGET(reg, 19, 4)
#define GX_BP_SET_INDTEVSTAGE15_MTX(reg, x) ((reg) = GX_BITSET(reg, 19, 4, x))
// WRAPS [16:18] (3) - S component wrap factor (stage 15)
#define GX_BP_INDTEVSTAGE15_WRAPS_ST 16
#define GX_BP_INDTEVSTAGE15_WRAPS_END 18
#define GX_BP_INDTEVSTAGE15_WRAPS_SZ 3
#define GX_BP_INDTEVSTAGE15_WRAPS_MASK (((1 << 3) - 1) << 31 - 18)
#define GX_BP_GET_INDTEVSTAGE15_WRAPS(reg) GX_BITGET(reg, 16, 3)
#define GX_BP_SET_INDTEVSTAGE15_WRAPS(reg, x) ((reg) = GX_BITSET(reg, 16, 3, x))
// WRAPT [13:15] (3) - T component wrap factor (stage 15)
#define GX_BP_INDTEVSTAGE15_WRAPT_ST 13
#define GX_BP_INDTEVSTAGE15_WRAPT_END 15
#define GX_BP_INDTEVSTAGE15_WRAPT_SZ 3
#define GX_BP_INDTEVSTAGE15_WRAPT_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_INDTEVSTAGE15_WRAPT(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_INDTEVSTAGE15_WRAPT(reg, x) ((reg) = GX_BITSET(reg, 13, 3, x))
// UTCLOD [12:12] (1) - Whether to use unmodified texcoords for mipmaps (stage
// 15)
#define GX_BP_INDTEVSTAGE15_UTCLOD_ST 12
#define GX_BP_INDTEVSTAGE15_UTCLOD_END 12
#define GX_BP_INDTEVSTAGE15_UTCLOD_SZ 1
#define GX_BP_INDTEVSTAGE15_UTCLOD_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_INDTEVSTAGE15_UTCLOD(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_INDTEVSTAGE15_UTCLOD(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 12, 1, x))
// ADDPREV [11:11] (1) - Whether to add in results from previous tev stage
// (stage 15)
#define GX_BP_INDTEVSTAGE15_ADDPREV_ST 11
#define GX_BP_INDTEVSTAGE15_ADDPREV_END 11
#define GX_BP_INDTEVSTAGE15_ADDPREV_SZ 1
#define GX_BP_INDTEVSTAGE15_ADDPREV_MASK (((1 << 1) - 1) << 31 - 11)
#define GX_BP_GET_INDTEVSTAGE15_ADDPREV(reg) GX_BITGET(reg, 11, 1)
#define GX_BP_SET_INDTEVSTAGE15_ADDPREV(reg, x)                                \
    ((reg) = GX_BITSET(reg, 11, 1, x))

/**
 * BP register 0x20 - scissorTL
 */
// TOP [21:31] (11) - Top component
#define GX_BP_SCISSORTL_TOP_ST 21
#define GX_BP_SCISSORTL_TOP_END 31
#define GX_BP_SCISSORTL_TOP_SZ 11
#define GX_BP_SCISSORTL_TOP_MASK (((1 << 11) - 1) << 31 - 31)
#define GX_BP_GET_SCISSORTL_TOP(reg) GX_BITGET(reg, 21, 11)
#define GX_BP_SET_SCISSORTL_TOP(reg, x) ((reg) = GX_BITSET(reg, 21, 11, x))
// LEFT [9:19] (11) - Left component
#define GX_BP_SCISSORTL_LEFT_ST 9
#define GX_BP_SCISSORTL_LEFT_END 19
#define GX_BP_SCISSORTL_LEFT_SZ 11
#define GX_BP_SCISSORTL_LEFT_MASK (((1 << 11) - 1) << 31 - 19)
#define GX_BP_GET_SCISSORTL_LEFT(reg) GX_BITGET(reg, 9, 11)
#define GX_BP_SET_SCISSORTL_LEFT(reg, x) ((reg) = GX_BITSET(reg, 9, 11, x))

/**
 * BP register 0x21 - scissorBR
 */
// BOT [21:31] (11) - Bottom component
#define GX_BP_SCISSORBR_BOT_ST 21
#define GX_BP_SCISSORBR_BOT_END 31
#define GX_BP_SCISSORBR_BOT_SZ 11
#define GX_BP_SCISSORBR_BOT_MASK (((1 << 11) - 1) << 31 - 31)
#define GX_BP_GET_SCISSORBR_BOT(reg) GX_BITGET(reg, 21, 11)
#define GX_BP_SET_SCISSORBR_BOT(reg, x) ((reg) = GX_BITSET(reg, 21, 11, x))
// RIGHT [9:19] (11) - Right component
#define GX_BP_SCISSORBR_RIGHT_ST 9
#define GX_BP_SCISSORBR_RIGHT_END 19
#define GX_BP_SCISSORBR_RIGHT_SZ 11
#define GX_BP_SCISSORBR_RIGHT_MASK (((1 << 11) - 1) << 31 - 19)
#define GX_BP_GET_SCISSORBR_RIGHT(reg) GX_BITGET(reg, 9, 11)
#define GX_BP_SET_SCISSORBR_RIGHT(reg, x) ((reg) = GX_BITSET(reg, 9, 11, x))

/**
 * BP register 0x22 - linePtWidth
 */
// LINESZ [24:31] (8) - Line size/width
#define GX_BP_LINEPTWIDTH_LINESZ_ST 24
#define GX_BP_LINEPTWIDTH_LINESZ_END 31
#define GX_BP_LINEPTWIDTH_LINESZ_SZ 8
#define GX_BP_LINEPTWIDTH_LINESZ_MASK (((1 << 8) - 1) << 31 - 31)
#define GX_BP_GET_LINEPTWIDTH_LINESZ(reg) GX_BITGET(reg, 24, 8)
#define GX_BP_SET_LINEPTWIDTH_LINESZ(reg, x) ((reg) = GX_BITSET(reg, 24, 8, x))
// POINTSZ [16:23] (8) - Point size
#define GX_BP_LINEPTWIDTH_POINTSZ_ST 16
#define GX_BP_LINEPTWIDTH_POINTSZ_END 23
#define GX_BP_LINEPTWIDTH_POINTSZ_SZ 8
#define GX_BP_LINEPTWIDTH_POINTSZ_MASK (((1 << 8) - 1) << 31 - 23)
#define GX_BP_GET_LINEPTWIDTH_POINTSZ(reg) GX_BITGET(reg, 16, 8)
#define GX_BP_SET_LINEPTWIDTH_POINTSZ(reg, x) ((reg) = GX_BITSET(reg, 16, 8, x))
// LINEOFS [13:15] (3) - Line offset
#define GX_BP_LINEPTWIDTH_LINEOFS_ST 13
#define GX_BP_LINEPTWIDTH_LINEOFS_END 15
#define GX_BP_LINEPTWIDTH_LINEOFS_SZ 3
#define GX_BP_LINEPTWIDTH_LINEOFS_MASK (((1 << 3) - 1) << 31 - 15)
#define GX_BP_GET_LINEPTWIDTH_LINEOFS(reg) GX_BITGET(reg, 13, 3)
#define GX_BP_SET_LINEPTWIDTH_LINEOFS(reg, x) ((reg) = GX_BITSET(reg, 13, 3, x))
// POINTOFS [10:12] (3) - Point offset
#define GX_BP_LINEPTWIDTH_POINTOFS_ST 10
#define GX_BP_LINEPTWIDTH_POINTOFS_END 12
#define GX_BP_LINEPTWIDTH_POINTOFS_SZ 3
#define GX_BP_LINEPTWIDTH_POINTOFS_MASK (((1 << 3) - 1) << 31 - 12)
#define GX_BP_GET_LINEPTWIDTH_POINTOFS(reg) GX_BITGET(reg, 10, 3)
#define GX_BP_SET_LINEPTWIDTH_POINTOFS(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 10, 3, x))

/**
 * BP register 0x25 - ras1_ss0
 */
// S0 [28:31] (4) - S-component scale (stage 0)
#define GX_BP_RAS1_SS0_S0_ST 28
#define GX_BP_RAS1_SS0_S0_END 31
#define GX_BP_RAS1_SS0_S0_SZ 4
#define GX_BP_RAS1_SS0_S0_MASK (((1 << 4) - 1) << 31 - 31)
#define GX_BP_GET_RAS1_SS0_S0(reg) GX_BITGET(reg, 28, 4)
#define GX_BP_SET_RAS1_SS0_S0(reg, x) ((reg) = GX_BITSET(reg, 28, 4, x))
// T0 [24:27] (4) - T-component scale (stage 0)
#define GX_BP_RAS1_SS0_T0_ST 24
#define GX_BP_RAS1_SS0_T0_END 27
#define GX_BP_RAS1_SS0_T0_SZ 4
#define GX_BP_RAS1_SS0_T0_MASK (((1 << 4) - 1) << 31 - 27)
#define GX_BP_GET_RAS1_SS0_T0(reg) GX_BITGET(reg, 24, 4)
#define GX_BP_SET_RAS1_SS0_T0(reg, x) ((reg) = GX_BITSET(reg, 24, 4, x))
// S1 [20:23] (4) - S-component scale (stage 1)
#define GX_BP_RAS1_SS0_S1_ST 20
#define GX_BP_RAS1_SS0_S1_END 23
#define GX_BP_RAS1_SS0_S1_SZ 4
#define GX_BP_RAS1_SS0_S1_MASK (((1 << 4) - 1) << 31 - 23)
#define GX_BP_GET_RAS1_SS0_S1(reg) GX_BITGET(reg, 20, 4)
#define GX_BP_SET_RAS1_SS0_S1(reg, x) ((reg) = GX_BITSET(reg, 20, 4, x))
// T1 [16:19] (4) - T-component scale (stage 1)
#define GX_BP_RAS1_SS0_T1_ST 16
#define GX_BP_RAS1_SS0_T1_END 19
#define GX_BP_RAS1_SS0_T1_SZ 4
#define GX_BP_RAS1_SS0_T1_MASK (((1 << 4) - 1) << 31 - 19)
#define GX_BP_GET_RAS1_SS0_T1(reg) GX_BITGET(reg, 16, 4)
#define GX_BP_SET_RAS1_SS0_T1(reg, x) ((reg) = GX_BITSET(reg, 16, 4, x))

/**
 * BP register 0x26 - ras1_ss1
 */
// S2 [28:31] (4) - S-component scale (stage 2)
#define GX_BP_RAS1_SS1_S2_ST 28
#define GX_BP_RAS1_SS1_S2_END 31
#define GX_BP_RAS1_SS1_S2_SZ 4
#define GX_BP_RAS1_SS1_S2_MASK (((1 << 4) - 1) << 31 - 31)
#define GX_BP_GET_RAS1_SS1_S2(reg) GX_BITGET(reg, 28, 4)
#define GX_BP_SET_RAS1_SS1_S2(reg, x) ((reg) = GX_BITSET(reg, 28, 4, x))
// T2 [24:27] (4) - T-component scale (stage 2)
#define GX_BP_RAS1_SS1_T2_ST 24
#define GX_BP_RAS1_SS1_T2_END 27
#define GX_BP_RAS1_SS1_T2_SZ 4
#define GX_BP_RAS1_SS1_T2_MASK (((1 << 4) - 1) << 31 - 27)
#define GX_BP_GET_RAS1_SS1_T2(reg) GX_BITGET(reg, 24, 4)
#define GX_BP_SET_RAS1_SS1_T2(reg, x) ((reg) = GX_BITSET(reg, 24, 4, x))
// S3 [20:23] (4) - S-component scale (stage 3)
#define GX_BP_RAS1_SS1_S3_ST 20
#define GX_BP_RAS1_SS1_S3_END 23
#define GX_BP_RAS1_SS1_S3_SZ 4
#define GX_BP_RAS1_SS1_S3_MASK (((1 << 4) - 1) << 31 - 23)
#define GX_BP_GET_RAS1_SS1_S3(reg) GX_BITGET(reg, 20, 4)
#define GX_BP_SET_RAS1_SS1_S3(reg, x) ((reg) = GX_BITSET(reg, 20, 4, x))
// T3 [16:19] (4) - T-component scale (stage 3)
#define GX_BP_RAS1_SS1_T3_ST 16
#define GX_BP_RAS1_SS1_T3_END 19
#define GX_BP_RAS1_SS1_T3_SZ 4
#define GX_BP_RAS1_SS1_T3_MASK (((1 << 4) - 1) << 31 - 19)
#define GX_BP_GET_RAS1_SS1_T3(reg) GX_BITGET(reg, 16, 4)
#define GX_BP_SET_RAS1_SS1_T3(reg, x) ((reg) = GX_BITSET(reg, 16, 4, x))

/**
 * BP register 0x27 - ras1_iref
 */
// MAP0 [29:31] (3) - Texmap id (stage 0)
#define GX_BP_RAS1_IREF_MAP0_ST 29
#define GX_BP_RAS1_IREF_MAP0_END 31
#define GX_BP_RAS1_IREF_MAP0_SZ 3
#define GX_BP_RAS1_IREF_MAP0_MASK (((1 << 3) - 1) << 31 - 31)
#define GX_BP_GET_RAS1_IREF_MAP0(reg) GX_BITGET(reg, 29, 3)
#define GX_BP_SET_RAS1_IREF_MAP0(reg, x) ((reg) = GX_BITSET(reg, 29, 3, x))
// TXC0 [26:28] (3) - Texcoord ID (stage 0)
#define GX_BP_RAS1_IREF_TXC0_ST 26
#define GX_BP_RAS1_IREF_TXC0_END 28
#define GX_BP_RAS1_IREF_TXC0_SZ 3
#define GX_BP_RAS1_IREF_TXC0_MASK (((1 << 3) - 1) << 31 - 28)
#define GX_BP_GET_RAS1_IREF_TXC0(reg) GX_BITGET(reg, 26, 3)
#define GX_BP_SET_RAS1_IREF_TXC0(reg, x) ((reg) = GX_BITSET(reg, 26, 3, x))
// MAP1 [23:25] (3) - Texmap id (stage 1)
#define GX_BP_RAS1_IREF_MAP1_ST 23
#define GX_BP_RAS1_IREF_MAP1_END 25
#define GX_BP_RAS1_IREF_MAP1_SZ 3
#define GX_BP_RAS1_IREF_MAP1_MASK (((1 << 3) - 1) << 31 - 25)
#define GX_BP_GET_RAS1_IREF_MAP1(reg) GX_BITGET(reg, 23, 3)
#define GX_BP_SET_RAS1_IREF_MAP1(reg, x) ((reg) = GX_BITSET(reg, 23, 3, x))
// TXC1 [20:22] (3) - Texcoord ID (stage 1)
#define GX_BP_RAS1_IREF_TXC1_ST 20
#define GX_BP_RAS1_IREF_TXC1_END 22
#define GX_BP_RAS1_IREF_TXC1_SZ 3
#define GX_BP_RAS1_IREF_TXC1_MASK (((1 << 3) - 1) << 31 - 22)
#define GX_BP_GET_RAS1_IREF_TXC1(reg) GX_BITGET(reg, 20, 3)
#define GX_BP_SET_RAS1_IREF_TXC1(reg, x) ((reg) = GX_BITSET(reg, 20, 3, x))
// MAP2 [17:19] (3) - Texmap id (stage 2)
#define GX_BP_RAS1_IREF_MAP2_ST 17
#define GX_BP_RAS1_IREF_MAP2_END 19
#define GX_BP_RAS1_IREF_MAP2_SZ 3
#define GX_BP_RAS1_IREF_MAP2_MASK (((1 << 3) - 1) << 31 - 19)
#define GX_BP_GET_RAS1_IREF_MAP2(reg) GX_BITGET(reg, 17, 3)
#define GX_BP_SET_RAS1_IREF_MAP2(reg, x) ((reg) = GX_BITSET(reg, 17, 3, x))
// TXC2 [14:16] (3) - Texcoord ID (stage 2)
#define GX_BP_RAS1_IREF_TXC2_ST 14
#define GX_BP_RAS1_IREF_TXC2_END 16
#define GX_BP_RAS1_IREF_TXC2_SZ 3
#define GX_BP_RAS1_IREF_TXC2_MASK (((1 << 3) - 1) << 31 - 16)
#define GX_BP_GET_RAS1_IREF_TXC2(reg) GX_BITGET(reg, 14, 3)
#define GX_BP_SET_RAS1_IREF_TXC2(reg, x) ((reg) = GX_BITSET(reg, 14, 3, x))
// MAP3 [11:13] (3) - Texmap id (stage 3)
#define GX_BP_RAS1_IREF_MAP3_ST 11
#define GX_BP_RAS1_IREF_MAP3_END 13
#define GX_BP_RAS1_IREF_MAP3_SZ 3
#define GX_BP_RAS1_IREF_MAP3_MASK (((1 << 3) - 1) << 31 - 13)
#define GX_BP_GET_RAS1_IREF_MAP3(reg) GX_BITGET(reg, 11, 3)
#define GX_BP_SET_RAS1_IREF_MAP3(reg, x) ((reg) = GX_BITSET(reg, 11, 3, x))
// TXC3 [8:10] (3) - Texcoord ID (stage 3)
#define GX_BP_RAS1_IREF_TXC3_ST 8
#define GX_BP_RAS1_IREF_TXC3_END 10
#define GX_BP_RAS1_IREF_TXC3_SZ 3
#define GX_BP_RAS1_IREF_TXC3_MASK (((1 << 3) - 1) << 31 - 10)
#define GX_BP_GET_RAS1_IREF_TXC3(reg) GX_BITGET(reg, 8, 3)
#define GX_BP_SET_RAS1_IREF_TXC3(reg, x) ((reg) = GX_BITSET(reg, 8, 3, x))

/**
 * BP register 0x30 - su_ssize0
 */
// USELINEOFS [13:13] (1)
#define GX_BP_SU_SSIZE0_USELINEOFS_ST 13
#define GX_BP_SU_SSIZE0_USELINEOFS_END 13
#define GX_BP_SU_SSIZE0_USELINEOFS_SZ 1
#define GX_BP_SU_SSIZE0_USELINEOFS_MASK (((1 << 1) - 1) << 31 - 13)
#define GX_BP_GET_SU_SSIZE0_USELINEOFS(reg) GX_BITGET(reg, 13, 1)
#define GX_BP_SET_SU_SSIZE0_USELINEOFS(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 13, 1, x))
// USEPOINTOFS [12:12] (1)
#define GX_BP_SU_SSIZE0_USEPOINTOFS_ST 12
#define GX_BP_SU_SSIZE0_USEPOINTOFS_END 12
#define GX_BP_SU_SSIZE0_USEPOINTOFS_SZ 1
#define GX_BP_SU_SSIZE0_USEPOINTOFS_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_SU_SSIZE0_USEPOINTOFS(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_SU_SSIZE0_USEPOINTOFS(reg, x)                                \
    ((reg) = GX_BITSET(reg, 12, 1, x))

/**
 * BP register 0x32 - su_ssize1
 */
// USELINEOFS [13:13] (1)
#define GX_BP_SU_SSIZE1_USELINEOFS_ST 13
#define GX_BP_SU_SSIZE1_USELINEOFS_END 13
#define GX_BP_SU_SSIZE1_USELINEOFS_SZ 1
#define GX_BP_SU_SSIZE1_USELINEOFS_MASK (((1 << 1) - 1) << 31 - 13)
#define GX_BP_GET_SU_SSIZE1_USELINEOFS(reg) GX_BITGET(reg, 13, 1)
#define GX_BP_SET_SU_SSIZE1_USELINEOFS(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 13, 1, x))
// USEPOINTOFS [12:12] (1)
#define GX_BP_SU_SSIZE1_USEPOINTOFS_ST 12
#define GX_BP_SU_SSIZE1_USEPOINTOFS_END 12
#define GX_BP_SU_SSIZE1_USEPOINTOFS_SZ 1
#define GX_BP_SU_SSIZE1_USEPOINTOFS_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_SU_SSIZE1_USEPOINTOFS(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_SU_SSIZE1_USEPOINTOFS(reg, x)                                \
    ((reg) = GX_BITSET(reg, 12, 1, x))

/**
 * BP register 0x34 - su_ssize2
 */
// USELINEOFS [13:13] (1)
#define GX_BP_SU_SSIZE2_USELINEOFS_ST 13
#define GX_BP_SU_SSIZE2_USELINEOFS_END 13
#define GX_BP_SU_SSIZE2_USELINEOFS_SZ 1
#define GX_BP_SU_SSIZE2_USELINEOFS_MASK (((1 << 1) - 1) << 31 - 13)
#define GX_BP_GET_SU_SSIZE2_USELINEOFS(reg) GX_BITGET(reg, 13, 1)
#define GX_BP_SET_SU_SSIZE2_USELINEOFS(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 13, 1, x))
// USEPOINTOFS [12:12] (1)
#define GX_BP_SU_SSIZE2_USEPOINTOFS_ST 12
#define GX_BP_SU_SSIZE2_USEPOINTOFS_END 12
#define GX_BP_SU_SSIZE2_USEPOINTOFS_SZ 1
#define GX_BP_SU_SSIZE2_USEPOINTOFS_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_SU_SSIZE2_USEPOINTOFS(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_SU_SSIZE2_USEPOINTOFS(reg, x)                                \
    ((reg) = GX_BITSET(reg, 12, 1, x))

/**
 * BP register 0x36 - su_ssize3
 */
// USELINEOFS [13:13] (1)
#define GX_BP_SU_SSIZE3_USELINEOFS_ST 13
#define GX_BP_SU_SSIZE3_USELINEOFS_END 13
#define GX_BP_SU_SSIZE3_USELINEOFS_SZ 1
#define GX_BP_SU_SSIZE3_USELINEOFS_MASK (((1 << 1) - 1) << 31 - 13)
#define GX_BP_GET_SU_SSIZE3_USELINEOFS(reg) GX_BITGET(reg, 13, 1)
#define GX_BP_SET_SU_SSIZE3_USELINEOFS(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 13, 1, x))
// USEPOINTOFS [12:12] (1)
#define GX_BP_SU_SSIZE3_USEPOINTOFS_ST 12
#define GX_BP_SU_SSIZE3_USEPOINTOFS_END 12
#define GX_BP_SU_SSIZE3_USEPOINTOFS_SZ 1
#define GX_BP_SU_SSIZE3_USEPOINTOFS_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_SU_SSIZE3_USEPOINTOFS(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_SU_SSIZE3_USEPOINTOFS(reg, x)                                \
    ((reg) = GX_BITSET(reg, 12, 1, x))

/**
 * BP register 0x38 - su_ssize4
 */
// USELINEOFS [13:13] (1)
#define GX_BP_SU_SSIZE4_USELINEOFS_ST 13
#define GX_BP_SU_SSIZE4_USELINEOFS_END 13
#define GX_BP_SU_SSIZE4_USELINEOFS_SZ 1
#define GX_BP_SU_SSIZE4_USELINEOFS_MASK (((1 << 1) - 1) << 31 - 13)
#define GX_BP_GET_SU_SSIZE4_USELINEOFS(reg) GX_BITGET(reg, 13, 1)
#define GX_BP_SET_SU_SSIZE4_USELINEOFS(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 13, 1, x))
// USEPOINTOFS [12:12] (1)
#define GX_BP_SU_SSIZE4_USEPOINTOFS_ST 12
#define GX_BP_SU_SSIZE4_USEPOINTOFS_END 12
#define GX_BP_SU_SSIZE4_USEPOINTOFS_SZ 1
#define GX_BP_SU_SSIZE4_USEPOINTOFS_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_SU_SSIZE4_USEPOINTOFS(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_SU_SSIZE4_USEPOINTOFS(reg, x)                                \
    ((reg) = GX_BITSET(reg, 12, 1, x))

/**
 * BP register 0x3A - su_ssize5
 */
// USELINEOFS [13:13] (1)
#define GX_BP_SU_SSIZE5_USELINEOFS_ST 13
#define GX_BP_SU_SSIZE5_USELINEOFS_END 13
#define GX_BP_SU_SSIZE5_USELINEOFS_SZ 1
#define GX_BP_SU_SSIZE5_USELINEOFS_MASK (((1 << 1) - 1) << 31 - 13)
#define GX_BP_GET_SU_SSIZE5_USELINEOFS(reg) GX_BITGET(reg, 13, 1)
#define GX_BP_SET_SU_SSIZE5_USELINEOFS(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 13, 1, x))
// USEPOINTOFS [12:12] (1)
#define GX_BP_SU_SSIZE5_USEPOINTOFS_ST 12
#define GX_BP_SU_SSIZE5_USEPOINTOFS_END 12
#define GX_BP_SU_SSIZE5_USEPOINTOFS_SZ 1
#define GX_BP_SU_SSIZE5_USEPOINTOFS_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_SU_SSIZE5_USEPOINTOFS(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_SU_SSIZE5_USEPOINTOFS(reg, x)                                \
    ((reg) = GX_BITSET(reg, 12, 1, x))

/**
 * BP register 0x3C - su_ssize6
 */
// USELINEOFS [13:13] (1)
#define GX_BP_SU_SSIZE6_USELINEOFS_ST 13
#define GX_BP_SU_SSIZE6_USELINEOFS_END 13
#define GX_BP_SU_SSIZE6_USELINEOFS_SZ 1
#define GX_BP_SU_SSIZE6_USELINEOFS_MASK (((1 << 1) - 1) << 31 - 13)
#define GX_BP_GET_SU_SSIZE6_USELINEOFS(reg) GX_BITGET(reg, 13, 1)
#define GX_BP_SET_SU_SSIZE6_USELINEOFS(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 13, 1, x))
// USEPOINTOFS [12:12] (1)
#define GX_BP_SU_SSIZE6_USEPOINTOFS_ST 12
#define GX_BP_SU_SSIZE6_USEPOINTOFS_END 12
#define GX_BP_SU_SSIZE6_USEPOINTOFS_SZ 1
#define GX_BP_SU_SSIZE6_USEPOINTOFS_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_SU_SSIZE6_USEPOINTOFS(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_SU_SSIZE6_USEPOINTOFS(reg, x)                                \
    ((reg) = GX_BITSET(reg, 12, 1, x))

/**
 * BP register 0x3E - su_ssize7
 */
// USELINEOFS [13:13] (1)
#define GX_BP_SU_SSIZE7_USELINEOFS_ST 13
#define GX_BP_SU_SSIZE7_USELINEOFS_END 13
#define GX_BP_SU_SSIZE7_USELINEOFS_SZ 1
#define GX_BP_SU_SSIZE7_USELINEOFS_MASK (((1 << 1) - 1) << 31 - 13)
#define GX_BP_GET_SU_SSIZE7_USELINEOFS(reg) GX_BITGET(reg, 13, 1)
#define GX_BP_SET_SU_SSIZE7_USELINEOFS(reg, x)                                 \
    ((reg) = GX_BITSET(reg, 13, 1, x))
// USEPOINTOFS [12:12] (1)
#define GX_BP_SU_SSIZE7_USEPOINTOFS_ST 12
#define GX_BP_SU_SSIZE7_USEPOINTOFS_END 12
#define GX_BP_SU_SSIZE7_USEPOINTOFS_SZ 1
#define GX_BP_SU_SSIZE7_USEPOINTOFS_MASK (((1 << 1) - 1) << 31 - 12)
#define GX_BP_GET_SU_SSIZE7_USEPOINTOFS(reg) GX_BITGET(reg, 12, 1)
#define GX_BP_SET_SU_SSIZE7_USEPOINTOFS(reg, x)                                \
    ((reg) = GX_BITSET(reg, 12, 1, x))

/**
 * BP register 0x59 - ScissorOffset
 */
// OX [22:31] (10)
#define GX_BP_SCISSOROFFSET_OX_ST 22
#define GX_BP_SCISSOROFFSET_OX_END 31
#define GX_BP_SCISSOROFFSET_OX_SZ 10
#define GX_BP_SCISSOROFFSET_OX_MASK (((1 << 10) - 1) << 31 - 31)
#define GX_BP_GET_SCISSOROFFSET_OX(reg) GX_BITGET(reg, 22, 10)
#define GX_BP_SET_SCISSOROFFSET_OX(reg, x) ((reg) = GX_BITSET(reg, 22, 10, x))
// OY [12:21] (10)
#define GX_BP_SCISSOROFFSET_OY_ST 12
#define GX_BP_SCISSOROFFSET_OY_END 21
#define GX_BP_SCISSOROFFSET_OY_SZ 10
#define GX_BP_SCISSOROFFSET_OY_MASK (((1 << 10) - 1) << 31 - 21)
#define GX_BP_GET_SCISSOROFFSET_OY(reg) GX_BITGET(reg, 12, 10)
#define GX_BP_SET_SCISSOROFFSET_OY(reg, x) ((reg) = GX_BITSET(reg, 12, 10, x))

#ifdef __cplusplus
}
#endif
#endif

#ifndef RVL_SDK_GX_HARDWARE_H
#define RVL_SDK_GX_HARDWARE_H
#include <revolution/GX/GXTypes.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Documentation from:
 * https://www.gc-forever.com/yagcd/chap8.html#sec8
 * https://www.gc-forever.com/yagcd/chap5.html#sec5
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/BPMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/XFMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/OpcodeDecoding.h
 */

/**
 * GX FIFO
 */
extern volatile union {
    // 1-byte
    char c;
    unsigned char uc;
    // 2-byte
    short s;
    unsigned short us;
    // 4-byte
    int i;
    unsigned int ui;
    void* p;
    float f;
} WGPIPE : 0xcc008000;

/**
 * GX FIFO commands
 */
typedef enum {
    GX_FIFO_NO_OP = 0x00,

    GX_FIFO_LOAD_BP_REG = 0x61,
    GX_FIFO_LOAD_CP_REG = 0x08,
    GX_FIFO_LOAD_XF_REG = 0x10,

    GX_FIFO_LOAD_INDX_A = 0x20,
    GX_FIFO_LOAD_INDX_B = 0x28,
    GX_FIFO_LOAD_INDX_C = 0x30,
    GX_FIFO_LOAD_INDX_D = 0x38,

    GX_FIFO_CALL_DL = 0x40,
    GX_FIFO_INVAL_VTX = 0x48,
} GXFifoCmd;

/**
 * GX FIFO BP commands
 */
typedef enum {
    GX_FIFO_BP_GENMODE = 0x00,
    GX_FIFO_BP_DISPLAYCOPYFILTER = 0x01, // 0x01 + 4
    GX_FIFO_BP_IND_MTXA = 0x06,          // 0x06 + (3 * 3)
    GX_FIFO_BP_IND_MTXB = 0x07,          // 0x07 + (3 * 3)
    GX_FIFO_BP_IND_MTXC = 0x08,          // 0x08 + (3 * 3)
    GX_FIFO_BP_IND_IMASK = 0x0F,
    GX_FIFO_BP_IND_CMD = 0x10, // 0x10 + 16
    GX_FIFO_BP_SCISSORTL = 0x20,
    GX_FIFO_BP_SCISSORBR = 0x21,
    GX_FIFO_BP_LINEPTWIDTH = 0x22,
    GX_FIFO_BP_PERF0_TRI = 0x23,
    GX_FIFO_BP_PERF0_QUAD = 0x24,
    GX_FIFO_BP_RAS1_SS0 = 0x25,
    GX_FIFO_BP_RAS1_SS1 = 0x26,
    GX_FIFO_BP_IREF = 0x27,
    GX_FIFO_BP_TREF = 0x28,     // 0x28 + 8
    GX_FIFO_BP_SU_SSIZE = 0x30, // 0x30 + (2 * 8)
    GX_FIFO_BP_SU_TSIZE = 0x31, // 0x31 + (2 * 8)
    GX_FIFO_BP_ZMODE = 0x40,
    GX_FIFO_BP_BLENDMODE = 0x41,
    GX_FIFO_BP_CONSTANTALPHA = 0x42,
    GX_FIFO_BP_ZCOMPARE = 0x43,
    GX_FIFO_BP_FIELDMASK = 0x44,
    GX_FIFO_BP_SETDRAWDONE = 0x45,
    GX_FIFO_BP_BUSCLOCK0 = 0x46,
    GX_FIFO_BP_PE_TOKEN_ID = 0x47,
    GX_FIFO_BP_PE_TOKEN_INT_ID = 0x48,
    GX_FIFO_BP_EFB_TL = 0x49,
    GX_FIFO_BP_EFB_WH = 0x4A,
    GX_FIFO_BP_EFB_ADDR = 0x4B,
    GX_FIFO_BP_MIPMAP_STRIDE = 0x4D,
    GX_FIFO_BP_COPYYSCALE = 0x4E,
    GX_FIFO_BP_CLEAR_AR = 0x4F,
    GX_FIFO_BP_CLEAR_GB = 0x50,
    GX_FIFO_BP_CLEAR_Z = 0x51,
    GX_FIFO_BP_TRIGGER_EFB_COPY = 0x52,
    GX_FIFO_BP_COPYFILTER0 = 0x53,
    GX_FIFO_BP_COPYFILTER1 = 0x54,
    GX_FIFO_BP_CLEARBBOX1 = 0x55,
    GX_FIFO_BP_CLEARBBOX2 = 0x56,
    GX_FIFO_BP_CLEAR_PIXEL_PERF = 0x57,
    GX_FIFO_BP_REVBITS = 0x58,
    GX_FIFO_BP_SCISSOROFFSET = 0x59,
    GX_FIFO_BP_PRELOAD_ADDR = 0x60,
    GX_FIFO_BP_PRELOAD_TMEMEVEN = 0x61,
    GX_FIFO_BP_PRELOAD_TMEMODD = 0x62,
    GX_FIFO_BP_PRELOAD_MODE = 0x63,
    GX_FIFO_BP_LOADTLUT0 = 0x64,
    GX_FIFO_BP_LOADTLUT1 = 0x65,
    GX_FIFO_BP_TEXINVALIDATE = 0x66,
    GX_FIFO_BP_PERF1 = 0x67,
    GX_FIFO_BP_FIELDMODE = 0x68,
    GX_FIFO_BP_BUSCLOCK1 = 0x69,
    GX_FIFO_BP_TX_SETMODE0 = 0x80,    // 0x80 + 4
    GX_FIFO_BP_TX_SETMODE1 = 0x84,    // 0x84 + 4
    GX_FIFO_BP_TX_SETIMAGE0 = 0x88,   // 0x88 + 4
    GX_FIFO_BP_TX_SETIMAGE1 = 0x8C,   // 0x8C + 4
    GX_FIFO_BP_TX_SETIMAGE2 = 0x90,   // 0x90 + 4
    GX_FIFO_BP_TX_SETIMAGE3 = 0x94,   // 0x94 + 4
    GX_FIFO_BP_TX_SETTLUT = 0x98,     // 0x98 + 4
    GX_FIFO_BP_TX_SETMODE0_4 = 0xA0,  // 0xA0 + 4
    GX_FIFO_BP_TX_SETMODE1_4 = 0xA4,  // 0xA4 + 4
    GX_FIFO_BP_TX_SETIMAGE0_4 = 0xA8, // 0xA8 + 4
    GX_FIFO_BP_TX_SETIMAGE1_4 = 0xAC, // 0xA4 + 4
    GX_FIFO_BP_TX_SETIMAGE2_4 = 0xB0, // 0xB0 + 4
    GX_FIFO_BP_TX_SETIMAGE3_4 = 0xB4, // 0xB4 + 4
    GX_FIFO_BP_TX_SETTLUT_4 = 0xB8,   // 0xB8 + 4
    GX_FIFO_BP_TEV_COLOR_ENV = 0xC0,  // 0xC0 + (2 * 16)
    GX_FIFO_BP_TEV_ALPHA_ENV = 0xC1,  // 0xC1 + (2 * 16)
    GX_FIFO_BP_TEV_COLOR_RA = 0xE0,   // 0xE0 + (2 * 4)
    GX_FIFO_BP_TEV_COLOR_BG = 0xE1,   // 0xE1 + (2 * 4)
    GX_FIFO_BP_FOGRANGE = 0xE8,       // 0xE8 + 6
    GX_FIFO_BP_FOGPARAM0 = 0xEE,
    GX_FIFO_BP_FOGBMAGNITUDE = 0xEF,
    GX_FIFO_BP_FOGBEXPONENT = 0xF0,
    GX_FIFO_BP_FOGPARAM3 = 0xF1,
    GX_FIFO_BP_FOGCOLOR = 0xF2,
    GX_FIFO_BP_ALPHACOMPARE = 0xF3,
    GX_FIFO_BP_BIAS = 0xF4,
    GX_FIFO_BP_ZTEX2 = 0xF5,
    GX_FIFO_BP_TEV_KSEL = 0xF6, // 0xF6 + 8
    GX_FIFO_BP_BP_MASK = 0xFE,
} GXBpCmd;

/**
 * GX FIFO XF commands
 */
typedef enum {
    GX_FIFO_XF_ERROR = 0x1000,
    GX_FIFO_XF_DIAG = 0x1001,
    GX_FIFO_XF_STATE0 = 0x1002,
    GX_FIFO_XF_STATE1 = 0x1003,
    GX_FIFO_XF_CLOCK = 0x1004,
    GX_FIFO_XF_CLIPDISABLE = 0x1005,
    GX_FIFO_XF_SETGPMETRIC = 0x1006,  // Perf0 according to YAGCD
    GX_FIFO_XF_UNKNOWN_1007 = 0x1007, // Perf1 according to YAGCD
    GX_FIFO_XF_VTXSPECS = 0x1008,
    GX_FIFO_XF_SETNUMCHAN = 0x1009,
    GX_FIFO_XF_SETCHAN0_AMBCOLOR = 0x100A,
    GX_FIFO_XF_SETCHAN1_AMBCOLOR = 0x100B,
    GX_FIFO_XF_SETCHAN0_MATCOLOR = 0x100C,
    GX_FIFO_XF_SETCHAN1_MATCOLOR = 0x100D,
    GX_FIFO_XF_SETCHAN0_COLOR = 0x100E,
    GX_FIFO_XF_SETCHAN1_COLOR = 0x100F,
    GX_FIFO_XF_SETCHAN0_ALPHA = 0x1010,
    GX_FIFO_XF_SETCHAN1_ALPHA = 0x1011,
    GX_FIFO_XF_DUALTEX = 0x1012,
    GX_FIFO_XF_SETMATRIXINDA = 0x1018,
    GX_FIFO_XF_SETMATRIXINDB = 0x1019,
    GX_FIFO_XF_SETVIEWPORT = 0x101A,
    GX_FIFO_XF_SETZSCALE = 0x101C,
    GX_FIFO_XF_SETZOFFSET = 0x101F,
    GX_FIFO_XF_SETPROJECTION = 0x1020,
    GX_FIFO_XF_SETNUMTEXGENS = 0x103F,
    GX_FIFO_XF_SETTEXMTXINFO = 0x1040,
    GX_FIFO_XF_SETPOSTMTXINFO = 0x1050,
    GX_FIFO_XF_REGISTERS_END = 0x1058,
} GXXfCmd;

/**
 * Write BP command to GX FIFO
 */
#define GX_WRITE_BP_CMD(data)                                                  \
    WGPIPE.c = GX_FIFO_LOAD_BP_REG;                                            \
    WGPIPE.i = (data);

/**
 * Write CP command to GX FIFO
 */
#define GX_WRITE_CP_CMD(addr, data)                                            \
    WGPIPE.c = GX_FIFO_LOAD_CP_REG;                                            \
    WGPIPE.i = (addr);                                                         \
    WGPIPE.i = (data);

/**
 * Write XF command to GX FIFO
 */
#define GX_WRITE_XF_CMD(addr, data)                                            \
    WGPIPE.c = GX_FIFO_LOAD_XF_REG;                                            \
    WGPIPE.i = (addr);                                                         \
    WGPIPE.i = (data);

#ifdef __cplusplus
}
#endif
#endif

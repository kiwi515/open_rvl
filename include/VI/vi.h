#ifndef RVL_SDK_VI_H
#define RVL_SDK_VI_H
#include <GX/GXFrameBuf.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    VI_TV_FMT_NTSC,
    VI_TV_FMT_PAL,
    VI_TV_FMT_MPAL,
    VI_TV_FMT_DEBUG,
    VI_TV_FMT_DEBUG_PAL,
    VI_TV_FMT_EURGB60
} VITvFormat;

typedef void (*VIRetraceCallback)(u32);

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback);
VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback);

void VIInit(void);

void VIConfigure(const GXRenderModeObj*);
void VIConfigurePan(u16, u16, u16, u16);
void VIFlush(void);
void VISetNextFrameBuffer(void*);

void VISetBlack(BOOL);
s32 VIGetRetraceCount(void);

VITvFormat VIGetTvFormat(void);

#ifdef __cplusplus
}
#endif
#endif

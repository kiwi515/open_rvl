#ifndef RVL_SDK_VI_H
#define RVL_SDK_VI_H
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

#ifdef __cplusplus
}
#endif
#endif

#ifndef RVL_SDK_GX_TEXTURE_H
#define RVL_SDK_GX_TEXTURE_H
#include "GX.h"
#include "GXTev.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum _GXTexCoord {
    GX_TEX_COORD_0,
} GXTexCoord;

typedef enum _GXTexMapID {
    GX_TEXMAP_0,
    GX_TEXMAP_1,
    GX_TEXMAP_2,
    GX_TEXMAP_3,
    GX_TEXMAP_4,
    GX_TEXMAP_5,
    GX_TEXMAP_6,
    GX_TEXMAP_7,

    GX_MAX_TEXMAP
} GXTexMapID;

void __GXSetSUTexRegs(void);

#ifdef __cplusplus
}
#endif
#endif
